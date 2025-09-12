#include <iostream>
#include "simulation/Simulation.h"
#include "simulation/SimulationEntry.h"
#include "ihm/ihm.h"
#include "persistence/DatabaseManager.h"
#include "persistence/DatabaseConnection.h"

int main() {
    const int CLIENT_ARRIVAL_INTERVAL = 1;
    const int MAX_SERVICE_TIME = 20;
    const int MIN_SERVICE_TIME = 10;
    const int CASHIER_COUNT = 3;
    const int SIMULATION_DURATION = 1000;
    const double PRIORITY_CLIENT_RATE = 0.9;
    const int CLIENT_PATIENCE_TIME = 7;
    const std::string MODE_AFFICHAGE = "db_graph";  // Nouveau mode pour DB + graphiques

    SimulationEntry entry(SIMULATION_DURATION, CASHIER_COUNT, MIN_SERVICE_TIME, MAX_SERVICE_TIME,
                          CLIENT_ARRIVAL_INTERVAL, PRIORITY_CLIENT_RATE, CLIENT_PATIENCE_TIME);

    if (MODE_AFFICHAGE == "ihm") {
        IHM ihm(entry);
        ihm.afficher();  // Lance l'interface graphique
    } else if (MODE_AFFICHAGE == "simulation") {
        Simulation simulation(entry);
        simulation.simulate();
        std::cout << simulation.simulationResults() << std::endl;
    } else if (MODE_AFFICHAGE == "db_graph") {
    Simulation simulation(entry);
    simulation.simulate();
    std::cout << "Simulation terminée. Affichage camembert (stats actuelles)..." << std::endl;

    // Camembert avec stats actuelles (pas DB)
    int served = simulation.getStatisticManager().servedClientCount();
    int departed = simulation.getStatisticManager().nonServedClientCount();
    double servedPercent = (served + departed > 0) ? (served * 100.0 / (served + departed)) : 0;
    std::cout << "Camembert - Servis: " << servedPercent << "%, Partis: " << (100 - servedPercent) << "%" << std::endl;

    // Sauvegarde en DB
    DatabaseConnection db;  // Crée une instance
    db.connect();  // Initialise la connexion
    DatabaseManager dbManager(db);
    int simId = dbManager.getNextSimulationId();
    simulation.getStatisticManager().saveToDatabase(simId, dbManager,  simulation.getCashiers(), entry);
    std::cout << "Sauvegardé en DB (ID: " << simId << ")." << std::endl;

    // Graphique occupation par caissier (depuis DB)
    auto occupancy = dbManager.loadCashierOccupancyBySimulationId(simId);
    std::cout << "Occupation par caissier (axe X: caissier, Y: taux):" << std::endl;
    for (auto& data : occupancy) {
        std::cout << "Caissier " << data.first << ": " << data.second << "%" << std::endl;
    }
    IHM ihm(entry);
    ihm.displayGraphs(served, departed, occupancy);
}

    return 0;
}
