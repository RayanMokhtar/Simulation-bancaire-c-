#ifndef STATISTIC_MANAGER_H
#define STATISTIC_MANAGER_H

#include <vector>
#include <memory>  // Ajouté
#include "AbstractClient.h"

class StatisticManager {
private:
    std::vector<std::shared_ptr<AbstractClient>> servedClients;  // Changé en shared_ptr
    std::vector<std::shared_ptr<AbstractClient>> nonServedClients;  // Changé en shared_ptr
    int simulationDuration = 0;
    int occupiedCashier = 0;

public:
    void registerServedClient(std::shared_ptr<AbstractClient> client);
    void registerNonServedClient(std::shared_ptr<AbstractClient> client);
    void simulationDurationRecord();
    void cashierOccupationRecord();
    double calculateAverageCashierOccupationRate(int cashierCount) const;
    double calculateAverageClientWaitingTime() const;
    double calculateAverageClientServiceTime() const;
    int servedClientCount() const;
    int nonServedClientCount() const;
    double calculateClientSatisfactionRate() const;
    ~StatisticManager() = default;  // Pas de delete manuel
};

#endif // STATISTIC_MANAGER_H