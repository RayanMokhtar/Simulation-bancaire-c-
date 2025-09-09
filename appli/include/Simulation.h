#ifndef SIMULATION_H
#define SIMULATION_H

#include <memory>
#include <string>
#include "Bank.h"
#include "StatisticManager.h"
#include "SimulationEntry.h"

namespace bank2::extension::simulation {

   
class Simulation {
private:
    Bank bank;
    StatisticManager statisticManager;
    const SimulationEntry simulationEntry;

public:
    Simulation(const SimulationEntry& entry);
    void simulate();
    void updateBank(int time);  // Ajout
    void serveClient(int time,  bank2::extension::bank::Cashier& cashier, std::unique_ptr<AbstractClient> client);  // Ajout
    int generateRandomServiceTime();  // Ajout
    bool newClientArrival(int interval, int simIter);  // Ajout
    std::string simulationResults() const;
};

} // namespace bank2::extension::simulation

#endif // SIMULATION_H