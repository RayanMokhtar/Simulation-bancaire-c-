#ifndef STATISTIC_MANAGER_H
#define STATISTIC_MANAGER_H

#include "client/AbstractClient.h"
#include <vector>
#include <memory>

// Forward declaration pour persistance MySQL
class SimulationRepository;

class StatisticManager {
private:
    std::vector<std::shared_ptr<AbstractClient>> servedClients;
    std::vector<std::shared_ptr<AbstractClient>> nonServedClients;
    int simulationDurationCounter;
    int cashierOccupationCounter;
    
    // Support persistance MySQL
    SimulationRepository* persistenceRepository = nullptr;
    int simulationId = -1;
    bool persistenceEnabled = false;

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
    
    // Méthodes de persistance MySQL
    void enablePersistence(SimulationRepository* repository, int simId);
    void disablePersistence();
    bool isPersistenceEnabled() const { return persistenceEnabled; }
    
    // Accès aux données pour persistance
    const std::vector<std::shared_ptr<AbstractClient>>& getServedClients() const { return servedClients; }
    const std::vector<std::shared_ptr<AbstractClient>>& getNonServedClients() const { return nonServedClients; }
};

#endif
