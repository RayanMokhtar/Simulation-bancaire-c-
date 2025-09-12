#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <vector>
#include <memory>
#include <string>
#include <map>
#include "persistence/DatabaseConnection.h"
#include "client/AbstractClient.h"
#include "bank/Cashier.h"
#include "simulation/SimulationEntry.h"

class DatabaseManager {
private:
    DatabaseConnection& dbConnection;
    std::string logFile;

public:
    DatabaseManager(DatabaseConnection& connection, const std::string& logFile = "db_log.txt");
    
    bool createTables();
    
    bool saveSimulation(int simulationId, const SimulationEntry& entry, int servedClients, int nonServedClients, 
                   double satisfactionRate, double avgWaitingTime, double avgServiceTime);
    bool saveCashierOccupancy(int simulationId, int cashierId, double occupationRate);

    // Récupération par simulationId
    std::tuple<int, int, int, int, int, double, int, int, int, double, double, double> loadSimulationById(int simulationId);  // Toute la ligne
    std::map<int, double> loadCashierOccupancyBySimulationId(int simulationId);  // (cashier_id -> occupation_rate)
        
    int getNextSimulationId();
    void log(const std::string& message);
};

#endif // DATABASE_MANAGER_H