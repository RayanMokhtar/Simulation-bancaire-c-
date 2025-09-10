#ifndef STATISTIC_MANAGER_H
#define STATISTIC_MANAGER_H

#include "AbstractClient.h"
#include <vector>
#include <memory>

class StatisticManager {
private:
    std::vector<std::shared_ptr<AbstractClient>> servedClients;
    std::vector<std::shared_ptr<AbstractClient>> nonServedClients;
    int simulationDurationCounter;
    int cashierOccupationCounter;

public:
    StatisticManager();
    
    // Recording methods
    void simulationDurationRecord() { simulationDurationCounter++; }
    void cashierOccupationRecord() { cashierOccupationCounter++; }
    void registerServedClient(std::shared_ptr<AbstractClient> client);
    void registerNonServedClient(std::shared_ptr<AbstractClient> client);
    
    // Calculation methods
    int servedClientCount() const { return static_cast<int>(servedClients.size()); }
    int nonServedClientCount() const { return static_cast<int>(nonServedClients.size()); }
    double calculateAverageClientWaitingTime() const;
    double calculateAverageClientServiceTime() const;
    double calculateAverageCashierOccupationRate(int cashierCount) const;
    double calculateClientSatisfactionRate() const;
};

#endif
