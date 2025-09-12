#include <iostream>

#include "simulation/Simulation.h"
#include "simulation/SimulationEntry.h"
#include "ihm/ihm.h"

int main() {
    const int CLIENT_ARRIVAL_INTERVAL = 1;
    const int MAX_SERVICE_TIME = 20;
    const int MIN_SERVICE_TIME = 10;
    const int CASHIER_COUNT = 3;
    const int SIMULATION_DURATION = 1000;
    const double PRIORITY_CLIENT_RATE = 0.9;
    const int CLIENT_PATIENCE_TIME = 7;
    const std::string MODE_AFFICHAGE = "ihm";  //Edit: modification choix du mode d'affichage ici directement .. 

    SimulationEntry entry(SIMULATION_DURATION, CASHIER_COUNT, MIN_SERVICE_TIME, MAX_SERVICE_TIME,
                          CLIENT_ARRIVAL_INTERVAL, PRIORITY_CLIENT_RATE, CLIENT_PATIENCE_TIME);

    if (MODE_AFFICHAGE == "ihm") {
        IHM ihm(entry);
        ihm.afficher();  //lance interface graphique
    } else if (MODE_AFFICHAGE == "simulation") {
        Simulation simulation(entry);
        simulation.simulate();
        std::cout << simulation.simulationResults() << std::endl;
    } else {
        std::cout << "Mode invalide !" << std::endl;
    }

    return 0;
}