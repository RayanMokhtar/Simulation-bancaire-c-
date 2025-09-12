#include "simulation/Simulation.h"
#include "client/Client.h"
#include "client/VIPClient.h"
#include "simulation/SimulationUtility.h"
#include <iostream>
#include <algorithm>

Simulation::Simulation(const SimulationEntry& simulationEntry) : simulationEntry(simulationEntry) {
    int cashierCount = simulationEntry.getCashierCount();
    bank = std::make_unique<Bank>(cashierCount);
    statisticManager = std::make_unique<StatisticManager>();
}

void Simulation::simulate() {
    int simulationDuration = simulationEntry.getSimulationDuration();
    int clientArrivalInterval = simulationEntry.getClientArrivalInterval();
    for (int currentSystemTime = 0; currentSystemTime <= simulationDuration; currentSystemTime++) {
        statisticManager->simulationDurationRecord();
        printBankStat(currentSystemTime, *bank);
        updateBank(currentSystemTime);
        bool newClientArrival = this->newClientArrival(clientArrivalInterval, currentSystemTime);
        if (newClientArrival) {
            int serviceTime = generateRandomServiceTime();
            std::unique_ptr<AbstractOperation> operation = getRandomOperation(serviceTime);
            std::shared_ptr<AbstractClient> client;
            double priorityClientRate = simulationEntry.getPriorityClientRate();
            if (isPriorityClient(priorityClientRate)) {
                client = std::make_shared<VIPClient>(currentSystemTime, std::move(operation), simulationEntry.getClientPatienceTime());
            } else {
                client = std::make_shared<Client>(currentSystemTime, std::move(operation), simulationEntry.getClientPatienceTime());
            }
            Cashier* freeCashier = bank->getFreeCashier();
            if (freeCashier == nullptr) {
                printClientArrival(currentSystemTime, false);
                Queue& queue = bank->getQueue();
                queue.addQueueLast(client);
            } else {
                printClientArrival(currentSystemTime, true);
                serveClient(currentSystemTime, freeCashier, client);
            }
        }
    }
}

void Simulation::updateBank(int currentSystemTime) {
    const auto& cashiers = bank->getCashiers();
    Queue& queue = bank->getQueue();
    for (const auto& cashier : cashiers) {
        if (!cashier->isFree()) {
            statisticManager->cashierOccupationRecord();
        }
        cashier->work();
        if (cashier->serviceFinished()) {
            auto leavingClient = cashier->getServingClient();
            leavingClient->setDepartureTime(currentSystemTime);
            statisticManager->registerServedClient(leavingClient);
            cashier->setServingClient(nullptr);
            if (!queue.isEmpty()) {
                auto nextClient = queue.findPriorityClient();
                if (nextClient == nullptr) nextClient = queue.getQueueFirst();
                else queue.removePriorityClient(nextClient);
                serveClient(currentSystemTime, cashier.get(), nextClient);
            }
        }
    }
    queue.updateClientPatience();
    auto impatientClients = queue.removeImpatientClients();
    for (auto& client : impatientClients) {
        client->setDepartureTime(currentSystemTime);
        statisticManager->registerNonServedClient(client);
        printClientDepartureWithoutBeingServed(currentSystemTime);
    }
}

void Simulation::serveClient(int currentSystemTime, Cashier* cashier, std::shared_ptr<AbstractClient> client) {
    client->setServiceStartTime(currentSystemTime);
    cashier->serve(client);
}

int Simulation::generateRandomServiceTime() {
    int minServiceTime = simulationEntry.getMinServiceTime();
    int maxServiceTime = simulationEntry.getMaxServiceTime();
    int randomServiceTime = getRandomNumber(minServiceTime, maxServiceTime);
    return randomServiceTime;
}

bool Simulation::newClientArrival(int clientArrivalInterval, int simIter) {
    return simIter % clientArrivalInterval == 0;
}

StatisticManager& Simulation::getStatisticManager() {
    return *statisticManager;  
}

std::string Simulation::simulationResults() {
    std::string results = "########## Simulation results : #####################\n";
    results += "Simulation Duration : " + std::to_string(simulationEntry.getSimulationDuration()) + "\n";
    results += "Served client count : " + std::to_string(statisticManager->servedClientCount()) + "\n";
    results += "Average client waiting time : " + std::to_string(statisticManager->calculateAverageClientWaitingTime()) + " \n";
    results += "Average client service time : " + std::to_string(statisticManager->calculateAverageClientServiceTime()) + " \n";
    results += "Cashier occupation rate : " + std::to_string(statisticManager->calculateAverageCashierOccupationRate(simulationEntry.getCashierCount())) + " % \n";
    results += "Non-Served client count : " + std::to_string(statisticManager->nonServedClientCount()) + "\n";
    results += "Client satisfaction rate : " + std::to_string(statisticManager->calculateClientSatisfactionRate()) + " %";
    return results;
}

const std::vector<std::unique_ptr<Cashier>>& Simulation::getCashiers() const {
    return bank->getCashiers();  // Accède aux caissiers via bank
}