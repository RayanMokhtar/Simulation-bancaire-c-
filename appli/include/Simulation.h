#ifndef SIMULATION_H
#define SIMULATION_H

#include <memory>
#include <string>
#include "Bank.h"
#include "StatisticManager.h"
#include "SimulationEntry.h"

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
    bool newClientArrival(int clientArrivalInterval, int simIter);
    std::string simulationResults();
};

#endif // SIMULATION_H