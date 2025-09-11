#include "simulation/SimulationNew.h"
#include "simulation/SimulationUtility.h"
#include "client/ClientNew.h"
#include "client/VIPClient.h"
#include "persistence/SimulationRepository.h"
#include <iostream>
#include <sstream>

SimulationNew::SimulationNew(const SimulationEntry& entry) 
    : simulationEntry(entry) {
    int cashierCount = simulationEntry.getCashierCount();
    bank = std::make_unique<Bank>(cashierCount);
    statisticManager = std::make_unique<StatisticManager>();
}

void SimulationNew::simulate() {
    while(!step()) {}
}

bool SimulationNew::step() {
    if (finished) return true;
    int simulationDuration = simulationEntry.getSimulationDuration();
    int clientArrivalInterval = simulationEntry.getClientArrivalInterval();

    if (currentSystemTime < simulationDuration) {
        statisticManager->simulationDurationRecord();
        updateBank(currentSystemTime);
        bool clientArrives = newClientArrival(clientArrivalInterval, currentSystemTime);
        if (clientArrives) {
            int serviceTime = generateRandomServiceTime();
            auto operation = SimulationUtility::getRandomOperation(serviceTime);
            std::shared_ptr<AbstractClient> client;
            if (SimulationUtility::isPriorityClient(simulationEntry.getPriorityClientRate())) {
                client = std::make_shared<VIPClient>(currentSystemTime, std::move(operation), simulationEntry.getClientPatienceTime());
            } else {
                client = std::make_shared<ClientNew>(currentSystemTime, std::move(operation), simulationEntry.getClientPatienceTime());
            }
            Cashier* freeCashier = bank->getFreeCashier();
            if (freeCashier == nullptr) {
                Queue* queue = bank->getQueue();
                queue->addQueueLast(client);
            } else {
                serveClient(currentSystemTime, freeCashier, client);
            }
        }
        currentSystemTime++;
        return false; // pas fini
    }

    // Phase de vidage (apres duree): on laisse terminer les clients en service et on vide la file
    if (!finalUpdateDone) {
        // Continuer a faire travailler les caissiers et servir la file jusqu a ce que plus rien ne reste
        Queue* queue = bank->getQueue();
        bool workRemaining = true;
        int safety = 0;
        while(workRemaining && safety < 10000) { // garde-fou
            workRemaining = false;
            auto cashiers = bank->getCashiers();
            for (Cashier* c : cashiers) {
                if (!c->isFree()) {
                    statisticManager->cashierOccupationRecord();
                }
                c->work();
                if (c->serviceFinished()) {
                    auto leavingClient = c->getServingClient();
                    leavingClient->setDepartureTime(currentSystemTime);
                    statisticManager->registerServedClient(leavingClient);
                    c->setServingClient(nullptr);
                }
                if (c->isFree() && !queue->isEmpty()) {
                    std::shared_ptr<AbstractClient> nextClient = queue->findPriorityClient();
                    if (!nextClient) nextClient = queue->getQueueFirst();
                    else queue->removePriorityClient(nextClient);
                    if (nextClient) {
                        serveClient(currentSystemTime, c, nextClient);
                        workRemaining = true;
                    }
                }
                if (!c->isFree()) workRemaining = true; // encore du travail en cours
            }
            // patience restante apres fin: on ne decremente plus pour ne pas fausser stats; on considere que ceux qui restent seront servis ou restent dans la file -> non servis
            safety++;
            currentSystemTime++;
        }
        // Tous les clients restants dans la file sont non servis
        while(!bank->getQueue()->isEmpty()) {
            auto cl = bank->getQueue()->getQueueFirst();
            if (cl) {
                cl->setDepartureTime(currentSystemTime);
                statisticManager->registerNonServedClient(cl);
            }
        }
        finalUpdateDone = true;
    }
    finished = true;
    return true;
}

void SimulationNew::updateBank(int currentSystemTime) {
    auto cashiers = bank->getCashiers();
    Queue* queue = bank->getQueue();
    for (Cashier* cashier : cashiers) {
        if (!cashier->isFree()) {
            statisticManager->cashierOccupationRecord();
        }
        cashier->work();
        if (cashier->serviceFinished()) {
            auto leavingClient = cashier->getServingClient();
            leavingClient->setDepartureTime(currentSystemTime);
            statisticManager->registerServedClient(leavingClient);
            cashier->setServingClient(nullptr);
            if (!queue->isEmpty()) {
                std::shared_ptr<AbstractClient> nextClient = queue->findPriorityClient();
                if (nextClient == nullptr) nextClient = queue->getQueueFirst();
                else queue->removePriorityClient(nextClient);
                serveClient(currentSystemTime, cashier, nextClient);
            }
        }
    }
    queue->updateClientPatience();
    auto impatientClients = queue->removeImpatientClients();
    for (auto& client : impatientClients) {
        client->setDepartureTime(currentSystemTime);
        statisticManager->registerNonServedClient(client);
    }
}

void SimulationNew::serveClient(int currentSystemTime, Cashier* cashier, std::shared_ptr<AbstractClient> client) {
    client->setServiceStartTime(currentSystemTime);
    cashier->serve(client);
}

int SimulationNew::generateRandomServiceTime() {
    int minServiceTime = simulationEntry.getMinServiceTime();
    int maxServiceTime = simulationEntry.getMaxServiceTime();
    return SimulationUtility::getRandomNumber(minServiceTime, maxServiceTime);
}

bool SimulationNew::newClientArrival(int clientArrivalInterval, int simIter) {
    return simIter % clientArrivalInterval == 0;
}

std::string SimulationNew::simulationResults() {
    std::stringstream results;
    results << "########## Simulation results : #####################\n";
    results << "Simulation Duration : " << simulationEntry.getSimulationDuration() << "\n";
    results << "Served client count : " << statisticManager->servedClientCount() << "\n";
    results << "Average client waiting time : " << statisticManager->calculateAverageClientWaitingTime() << " \n";
    results << "Average client service time : " << statisticManager->calculateAverageClientServiceTime() << " \n";
    results << "Cashier occupation rate : " 
            << statisticManager->calculateAverageCashierOccupationRate(simulationEntry.getCashierCount()) << " % \n";
    results << "Non-Served client count : " << statisticManager->nonServedClientCount() << "\n";
    results << "Client satisfaction rate : " << statisticManager->calculateClientSatisfactionRate() << " %";
    return results.str();
}

void SimulationNew::enablePersistence(SimulationRepository* repository, int simId) {
    persistenceRepository = repository;
    simulationId = simId;
    persistenceEnabled = true;
}

void SimulationNew::disablePersistence() {
    persistenceRepository = nullptr;
    simulationId = -1;
    persistenceEnabled = false;
}
