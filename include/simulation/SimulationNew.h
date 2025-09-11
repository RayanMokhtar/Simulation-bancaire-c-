#ifndef SIMULATION_NEW_H
#define SIMULATION_NEW_H

#include "bank/Bank.h"
#include "simulation/StatisticManager.h"
#include "simulation/SimulationEntry.h"
#include <memory>

// Forward declaration pour éviter la dépendance circulaire
class SimulationRepository;

class SimulationNew {
private:
    std::unique_ptr<Bank> bank;
    std::unique_ptr<StatisticManager> statisticManager;
    SimulationEntry simulationEntry;
    // Nouveau: support pas-a-pas
    int currentSystemTime = 0;
    bool finished = false;
    bool finalUpdateDone = false;
    
    // Support persistance MySQL
    SimulationRepository* persistenceRepository = nullptr;
    int simulationId = -1;
    bool persistenceEnabled = false;

public:
    SimulationNew(const SimulationEntry& entry);
    
    void simulate(); // simulation complète (boucle sur step())
    bool step();     // exécute 1 tick; retourne true si simulation terminée après cet appel
    int getCurrentTime() const { return currentSystemTime; }
    bool isFinished() const { return finished; }
    std::string simulationResults();
    
    // Méthodes de persistance MySQL
    void enablePersistence(SimulationRepository* repository, int simId = -1);
    void disablePersistence();
    bool isPersistenceEnabled() const { return persistenceEnabled; }
    int getSimulationId() const { return simulationId; }

    // --- Nouveaux getters statistiques ---
    int getServedClientCount() const { return statisticManager ? statisticManager->servedClientCount() : 0; }
    int getNonServedClientCount() const { return statisticManager ? statisticManager->nonServedClientCount() : 0; }
    double getAverageWaitingTime() const { return statisticManager ? statisticManager->calculateAverageClientWaitingTime() : 0.0; }
    double getAverageServiceTime() const { return statisticManager ? statisticManager->calculateAverageClientServiceTime() : 0.0; }
    double getCashierOccupationRate() const { return statisticManager ? statisticManager->calculateAverageCashierOccupationRate(simulationEntry.getCashierCount()) : 0.0; }
    double getClientSatisfactionRate() const { return statisticManager ? statisticManager->calculateClientSatisfactionRate() : 0.0; }
    int getConfiguredSimulationDuration() const { return simulationEntry.getSimulationDuration(); }

private:
    void updateBank(int currentSystemTime);
    void serveClient(int currentSystemTime, Cashier* cashier, std::shared_ptr<AbstractClient> client);
    int generateRandomServiceTime();
    bool newClientArrival(int clientArrivalInterval, int simIter);
};

#endif
