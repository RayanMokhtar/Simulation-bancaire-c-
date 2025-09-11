#include "simulation/StatisticManager.h"
#include "persistence/SimulationRepository.h"
#include <iostream>

StatisticManager::StatisticManager() 
    : simulationDurationCounter(0), cashierOccupationCounter(0) {
}

void StatisticManager::registerServedClient(std::shared_ptr<AbstractClient> client) {
    servedClients.push_back(client);
    
    // Persistance automatique MySQL si activée
    if (persistenceEnabled && persistenceRepository && simulationId > 0) {
        try {
            persistenceRepository->saveClient(simulationId, client, true);
        } catch (const std::exception& e) {
            std::cerr << "Error saving served client to database: " << e.what() << std::endl;
        }
    }
}

void StatisticManager::registerNonServedClient(std::shared_ptr<AbstractClient> client) {
    nonServedClients.push_back(client);
    
    // Persistance automatique MySQL si activée
    if (persistenceEnabled && persistenceRepository && simulationId > 0) {
        try {
            persistenceRepository->saveClient(simulationId, client, false);
        } catch (const std::exception& e) {
            std::cerr << "Error saving non-served client to database: " << e.what() << std::endl;
        }
    }
}

double StatisticManager::calculateAverageClientWaitingTime() const {
    if (servedClients.empty()) return 0.0;
    
    double totalWaitingTime = 0.0;
    for (const auto& client : servedClients) {
        int waitingTime = client->getServiceStartTime() - client->getArrivalTime();
        totalWaitingTime += waitingTime;
    }
    return totalWaitingTime / servedClients.size();
}

double StatisticManager::calculateAverageClientServiceTime() const {
    if (servedClients.empty()) return 0.0;
    
    double totalServiceTime = 0.0;
    for (const auto& client : servedClients) {
        int serviceTime = client->getDepartureTime() - client->getServiceStartTime();
        totalServiceTime += serviceTime;
    }
    return totalServiceTime / servedClients.size();
}

double StatisticManager::calculateAverageCashierOccupationRate(int cashierCount) const {
    if (simulationDurationCounter == 0 || cashierCount == 0) return 0.0;
    
    double totalPossibleOccupation = simulationDurationCounter * cashierCount;
    return (static_cast<double>(cashierOccupationCounter) / totalPossibleOccupation) * 100.0;
}

double StatisticManager::calculateClientSatisfactionRate() const {
    int totalClients = servedClients.size() + nonServedClients.size();
    if (totalClients == 0) return 0.0;
    
    return (static_cast<double>(servedClients.size()) / totalClients) * 100.0;
}

void StatisticManager::enablePersistence(SimulationRepository* repository, int simId) {
    persistenceRepository = repository;
    simulationId = simId;
    persistenceEnabled = true;
}

void StatisticManager::disablePersistence() {
    persistenceRepository = nullptr;
    simulationId = -1;
    persistenceEnabled = false;
}
