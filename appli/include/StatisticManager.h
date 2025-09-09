#ifndef STATISTIC_MANAGER_H
#define STATISTIC_MANAGER_H

#include <memory>
#include <vector>
#include "AbstractClient.h"

namespace bank2::extension::simulation {

using bank2::extension::client::AbstractClient;  // Ajoutez cette ligne

class StatisticManager {
private:
    std::vector<std::unique_ptr<AbstractClient>> servedClients;
    std::vector<std::unique_ptr<AbstractClient>> nonServedClients;
    int simulationDuration = 0;
    int occupiedCashier = 0;

public:
    void registerServedClient(std::unique_ptr<AbstractClient> client);
    void registerNonServedClient(std::unique_ptr<AbstractClient> client);
    void simulationDurationRecord();
    void cashierOccupationRecord();
    double calculateAverageCashierOccupationRate(int cashierCount) const;
    double calculateAverageClientWaitingTime() const;
    double calculateAverageClientServiceTime() const;
    int servedClientCount() const;
    int nonServedClientCount() const;
    double calculateClientSatisfactionRate() const;
};

} // namespace bank2::extension::simulation

#endif // STATISTIC_MANAGER_H