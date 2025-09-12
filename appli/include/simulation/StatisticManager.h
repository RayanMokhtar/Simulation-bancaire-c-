#ifndef STATISTIC_MANAGER_H
#define STATISTIC_MANAGER_H

#include <vector>
#include <memory>
#include <map>        // Ajouté pour std::map
#include <string>     // Ajouté pour std::string
#include "client/AbstractClient.h"
#include "persistence/DatabaseManager.h"  // Ajouté pour DatabaseManager
#include "bank/Cashier.h"                 // Ajouté pour Cashier

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
    std::map<std::string, std::pair<int, double>> getOperationStats() const;
    void saveToDatabase(int simulationId, DatabaseManager& dbManager, const std::vector<std::unique_ptr<Cashier>>& cashiers, const SimulationEntry& entry);  // Supprime StatisticManager::
};
#endif // STATISTIC_MANAGER_H