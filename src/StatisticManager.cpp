#include "../include/StatisticManager.h"

StatisticManager::StatisticManager() 
    : simulationDurationCounter(0), cashierOccupationCounter(0) {
}

void StatisticManager::registerServedClient(std::shared_ptr<AbstractClient> client) {
    servedClients.push_back(client);
}

void StatisticManager::registerNonServedClient(std::shared_ptr<AbstractClient> client) {
    nonServedClients.push_back(client);
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
