#include "StatisticManager.h"

void StatisticManager::registerServedClient(std::shared_ptr<AbstractClient> client) {
    servedClients.push_back(client);  // Maintenant compatible
}

void StatisticManager::registerNonServedClient(std::shared_ptr<AbstractClient> client) {
    nonServedClients.push_back(client);  // Maintenant compatible
}

void StatisticManager::simulationDurationRecord() { ++simulationDuration; }

void StatisticManager::cashierOccupationRecord() { ++occupiedCashier; }

double StatisticManager::calculateAverageCashierOccupationRate(int cashierCount) const {
    return (occupiedCashier * 100.0 / simulationDuration) / cashierCount;
}

double StatisticManager::calculateAverageClientWaitingTime() const {
    if (servedClients.empty()) return 0.0;
    int total = 0;
    for (const auto& client : servedClients) {
        total += client->getServiceStartTime() - client->getArrivalTime();
    }
    return total / static_cast<double>(servedClients.size());
}

double StatisticManager::calculateAverageClientServiceTime() const {
    if (servedClients.empty()) return 0.0;  // Évite division par zéro
    int total = 0;
    for (const auto& client : servedClients) {
        total += client->getDepartureTime() - client->getServiceStartTime();
    }
    return total / static_cast<double>(servedClients.size());
}

int StatisticManager::servedClientCount() const { return servedClients.size(); }

int StatisticManager::nonServedClientCount() const { return nonServedClients.size(); }

double StatisticManager::calculateClientSatisfactionRate() const {
    int total = servedClientCount() + nonServedClientCount();
    if (total == 0) return 0.0;
    return servedClientCount() * 100.0 / total;
}

