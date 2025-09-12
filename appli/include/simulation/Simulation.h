#ifndef SIMULATION_H
#define SIMULATION_H

#include <memory>
#include <string>
#include "bank/Bank.h"
#include "simulation/StatisticManager.h"
#include "simulation/SimulationEntry.h"

class Simulation {
private:
    std::unique_ptr<Bank> bank;  // Changé en unique_ptr
    std::unique_ptr<StatisticManager> statisticManager;  // Changé en unique_ptr
    const SimulationEntry simulationEntry;

public:
    Simulation(const SimulationEntry& simulationEntry);
    void simulate();
    void updateBank(int currentSystemTime);
    void serveClient(int currentSystemTime, Cashier* cashier, std::shared_ptr<AbstractClient> client);  // Changé en shared_ptr
    int generateRandomServiceTime();
    StatisticManager& getStatisticManager();  // Retourne une référence au StatisticManager
    bool newClientArrival(int clientArrivalInterval, int simIter);
    std::string simulationResults();
    const std::vector<std::unique_ptr<Cashier>>& getCashiers() const;  // Méthode pour accéder aux caissiers
};

#endif // SIMULATION_H