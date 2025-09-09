#include "Simulation.h"
#include "Client.h"
#include "VIPClient.h"
#include "SimulationUtility.h"
#include <iostream>
using bank2::extension::client::AbstractClient;
using bank2::extension::bank::Bank;

namespace bank2::extension::simulation {

Simulation::Simulation(const SimulationEntry& entry) : bank(entry.getCashierCount()), simulationEntry(entry) {}

void Simulation::simulate() {
    for (int time = 0; time <= simulationEntry.getSimulationDuration(); ++time) {
        statisticManager.simulationDurationRecord();
        printBankStat(time, bank);
        updateBank(time);
        if (newClientArrival(simulationEntry.getClientArrivalInterval(), time)) {
            int serviceTime = generateRandomServiceTime();
            auto operation = getRandomOperation(serviceTime);
            std::unique_ptr<AbstractClient> client;
            if (isPriorityClient(simulationEntry.getPriorityClientRate())) {
                client = std::make_unique< bank2::extension::client::VIPClient>(time, std::move(operation), simulationEntry.getClientPatienceTime());
            } else {
                client = std::make_unique< bank2::extension::client::Client>(time, std::move(operation), simulationEntry.getClientPatienceTime());
            }
            auto freeCashier = bank.getFreeCashier();
            if (!freeCashier) {
                printClientArrival(time, false);
                bank.getQueue().addQueueLast(std::move(client));
            } else {
                printClientArrival(time, true);
                serveClient(time, *freeCashier, std::move(client));
            }
        }
    }
}

void Simulation::updateBank(int time) {
    for (auto& cashier : bank.getCashiers()) {
        if (!cashier->isFree()) statisticManager.cashierOccupationRecord();
        cashier->work();
        if (cashier->serviceFinished()) {
            auto leavingClient = cashier->getServingClient();
            leavingClient->setDepartureTime(time);
            printClientDeparture(time);
            statisticManager.registerServedClient(std::unique_ptr<AbstractClient>(leavingClient));
            cashier->setServingClient(nullptr);
            if (!bank.getQueue().isEmpty()) {
                auto nextClient = bank.getQueue().findPriorityClient();
                if (!nextClient) {
                    nextClient = bank.getQueue().getQueueFirst().get();
                    bank.getQueue().removePriorityClient(nextClient);
                } else {
                    bank.getQueue().removePriorityClient(nextClient);
                }
                serveClient(time, *cashier, std::unique_ptr<AbstractClient>(nextClient));
            }
        }
    }
    bank.getQueue().updateClientPatience();
    auto impatient = bank.getQueue().removeImpatientClients();
    for (auto& client : impatient) {
        client->setDepartureTime(time);
        statisticManager.registerNonServedClient(std::move(client));
        printClientDepartureWithoutBeingServed(time);
    }
}

void Simulation::serveClient(int time, bank2::extension::bank::Cashier& cashier, std::unique_ptr<AbstractClient> client) {
    client->setServiceStartTime(time);
    int serviceTime = client->getOperation().getServiceTime();  // Obtenir la valeur AVANT le déplacement.
    cashier.serve(std::move(client));  // Maintenant, déplacer en toute sécurité.
    printServiceTimeTrace(time, serviceTime);  // Utiliser la valeur stockée.
}

int Simulation::generateRandomServiceTime() {
    return getRandomNumber(simulationEntry.getMinServiceTime(), simulationEntry.getMaxServiceTime());
}

bool Simulation::newClientArrival(int interval, int simIter) { return simIter % interval == 0; }

std::string Simulation::simulationResults() const {
    std::string results = "########## Simulation results #####################\n";
    results += "Simulation Duration: " + std::to_string(simulationEntry.getSimulationDuration()) + "\n";
    results += "Served client count: " + std::to_string(statisticManager.servedClientCount()) + "\n";
    results += "Average client waiting time: " + std::to_string(statisticManager.calculateAverageClientWaitingTime()) + "\n";
    results += "Average client service time: " + std::to_string(statisticManager.calculateAverageClientServiceTime()) + "\n";
    results += "Cashier occupation rate: " + std::to_string(statisticManager.calculateAverageCashierOccupationRate(simulationEntry.getCashierCount())) + "%\n";
    results += "Non-Served client count: " + std::to_string(statisticManager.nonServedClientCount()) + "\n";
    results += "Client satisfaction rate: " + std::to_string(statisticManager.calculateClientSatisfactionRate()) + "%";
    return results;
}

} // namespace bank2::extension::simulation