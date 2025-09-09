#include "StatisticManager.h"

namespace bank2::extension::simulation {

void StatisticManager::registerServedClient(std::unique_ptr<AbstractClient> client) {
    servedClients.push_back(std::move(client));
}

void StatisticManager::registerNonServedClient(std::unique_ptr<AbstractClient> client) {
    nonServedClients.push_back(std::move(client));
}

void StatisticManager::simulationDurationRecord() { ++simulationDuration; }

void StatisticManager::cashierOccupationRecord() { ++occupiedCashier; }

double StatisticManager::calculateAverageCashierOccupationRate(int cashierCount) const {
    return (occupiedCashier * 100.0 / simulationDuration) / cashierCount;
}

double StatisticManager::calculateAverageClientWaitingTime() const {
    int total = 0;
    for (const auto& client : servedClients) {
        total += client->getServiceStartTime() - client->getArrivalTime();
    }
    return total / static_cast<double>(servedClients.size());
}

double StatisticManager::calculateAverageClientServiceTime() const {
    int total = 0;
    for (const auto& client : servedClients) {
        total += client->getDepartureTime() - client->getServiceStartTime();
    }
    return total / static_cast<double>(servedClients.size());
}

int StatisticManager::servedClientCount() const { return servedClients.size(); }

int StatisticManager::nonServedClientCount() const { return nonServedClients.size(); }

double StatisticManager::calculateClientSatisfactionRate() const {
    return servedClientCount() * 100.0 / (servedClientCount() + nonServedClientCount());
}

} // namespace bank2::extension::simulation