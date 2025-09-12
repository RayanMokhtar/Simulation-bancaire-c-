#include "persistence/DatabaseManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>

DatabaseManager::DatabaseManager(DatabaseConnection& connection, const std::string& logFile) 
    : dbConnection(connection), logFile(logFile) {}

bool DatabaseManager::saveSimulation(int simulationId, const SimulationEntry& entry, int servedClients, int nonServedClients, 
                                    double satisfactionRate, double avgWaitingTime, double avgServiceTime) {
    std::stringstream query;
    query << "INSERT INTO simulations (id, duration, num_cashiers, min_service_time, max_service_time, client_arrival_interval, priority_client_rate, client_patience_time, served_clients, non_served_clients, satisfaction_rate, avg_waiting_time, avg_service_time) VALUES ("
          << simulationId << ", " << entry.getSimulationDuration() << ", " << entry.getCashierCount() << ", "
          << entry.getMinServiceTime() << ", " << entry.getMaxServiceTime() << ", " << entry.getClientArrivalInterval() << ", "
          << entry.getPriorityClientRate() << ", " << entry.getClientPatienceTime() << ", "
          << servedClients << ", " << nonServedClients << ", " << satisfactionRate << ", " << avgWaitingTime << ", " << avgServiceTime << ")";

#ifdef WITH_PERSISTENCE
    return dbConnection.executeQuery(query.str());
#else
    std::cout << "[STUB] Simulating save: " << query.str() << std::endl;
    log("Simulated save: " + query.str());
    return true;
#endif
}

bool DatabaseManager::saveCashierOccupancy(int simulationId, int cashierId, double occupationRate) {
    std::stringstream query;
    query << "INSERT INTO cashier_occupancy (simulation_id, cashier_id, occupation_rate) VALUES ("
          << simulationId << ", " << cashierId << ", " << occupationRate << ")";

#ifdef WITH_PERSISTENCE
    return dbConnection.executeQuery(query.str());
#else
    std::cout << "[STUB] Simulating save: " << query.str() << std::endl;
    log("Simulated save: " + query.str());
    return true;
#endif
}

std::tuple<int, int, int, int, int, double, int, int, int, double, double, double> DatabaseManager::loadSimulationById(int simulationId) {
    std::tuple<int, int, int, int, int, double, int, int, int, double, double, double> result = {0,0,0,0,0,0.0,0,0,0,0.0,0.0,0.0};  // Valeurs par défaut

#ifdef WITH_PERSISTENCE
    std::stringstream query;
    query << "SELECT duration, num_cashiers, min_service_time, max_service_time, client_arrival_interval, priority_client_rate, client_patience_time, served_clients, non_served_clients, satisfaction_rate, avg_waiting_time, avg_service_time FROM simulations WHERE id = " << simulationId;
    MYSQL_RES* res = dbConnection.executeSelect(query.str());
    if (!res) return result;
    
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row) {
        result = std::make_tuple(
            std::stoi(row[0]), std::stoi(row[1]), std::stoi(row[2]), std::stoi(row[3]), std::stoi(row[4]),
            std::stod(row[5]), std::stoi(row[6]), std::stoi(row[7]), std::stoi(row[8]), std::stod(row[9]), std::stod(row[10]), std::stod(row[11])
        );
    }
    mysql_free_result(res);
#else
    // Simulation : Retourner des données fictives
    result = std::make_tuple(1000, 3, 10, 20, 1, 0.9, 7, 80, 20, 80.0, 5.5, 15.0);
    std::cout << "[STUB] Simulating load simulation by ID: " << simulationId << std::endl;
#endif

    return result;
}

std::map<int, double> DatabaseManager::loadCashierOccupancyBySimulationId(int simulationId) {
    std::map<int, double> results;

#ifdef WITH_PERSISTENCE
    std::stringstream query;
    query << "SELECT cashier_id, occupation_rate FROM cashier_occupancy WHERE simulation_id = " << simulationId;
    MYSQL_RES* res = dbConnection.executeSelect(query.str());
    if (!res) return results;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        int cashierId = std::stoi(row[0]);
        double occupation = std::stod(row[1]);
        results[cashierId] = occupation;
    }
    mysql_free_result(res);
#else
    // Simulation : Retourner des données fictives
    results = {{0, 75.0}, {1, 80.0}, {2, 70.0}};
    std::cout << "[STUB] Simulating load cashier occupancy by simulation ID: " << simulationId << std::endl;
#endif

    return results;
}

int DatabaseManager::getNextSimulationId() {
#ifdef WITH_PERSISTENCE
    MYSQL_RES* res = dbConnection.executeSelect("SELECT MAX(id) FROM simulations");
    if (!res) return 1;
    MYSQL_ROW row = mysql_fetch_row(res);
    int maxId = row[0] ? std::stoi(row[0]) : 0;
    mysql_free_result(res);
    return maxId + 1;
#else
    static int simId = 1;
    return simId++;
#endif
}

void DatabaseManager::log(const std::string& message) {
    std::ofstream logStream(logFile, std::ios::app);
    if (logStream.is_open()) {
        std::time_t now = std::time(nullptr);
        logStream << std::ctime(&now) << ": " << message << std::endl;
    }
}