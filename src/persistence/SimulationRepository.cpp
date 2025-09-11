#include "persistence/SimulationRepository.h"
#include "client/ClientNew.h"
#include "client/VIPClient.h"
#include "client/Consultation.h"
#include "client/Transfer.h"
#include "client/Withdraw.h"
#include <sstream>
#include <iostream>

SimulationRepository::SimulationRepository(DatabaseConnection& database) : db(database) {}

int SimulationRepository::saveConfiguration(const std::string& name, const SimulationEntry& entry) {
    if (!db.isConnected()) {
        throw DatabaseException("Database not connected");
    }
    
    std::stringstream query;
    query << "INSERT INTO simulation_configurations "
          << "(name, simulation_duration, cashier_count, client_arrival_interval, "
          << "min_service_time, max_service_time, priority_client_rate, client_patience_time) "
          << "VALUES ('"
          << db.escapeString(name) << "', "
          << entry.getSimulationDuration() << ", "
          << entry.getCashierCount() << ", "
          << entry.getClientArrivalInterval() << ", "
          << entry.getMinServiceTime() << ", "
          << entry.getMaxServiceTime() << ", "
          << entry.getPriorityClientRate() << ", "
          << entry.getClientPatienceTime() << ")";
    
    if (db.executeQuery(query.str())) {
        return static_cast<int>(db.getLastInsertId());
    }
    return -1;
}

bool SimulationRepository::updateConfiguration(int configId, const std::string& name, const SimulationEntry& entry) {
    std::stringstream query;
    query << "UPDATE simulation_configurations SET "
          << "name = '" << db.escapeString(name) << "', "
          << "simulation_duration = " << entry.getSimulationDuration() << ", "
          << "cashier_count = " << entry.getCashierCount() << ", "
          << "client_arrival_interval = " << entry.getClientArrivalInterval() << ", "
          << "min_service_time = " << entry.getMinServiceTime() << ", "
          << "max_service_time = " << entry.getMaxServiceTime() << ", "
          << "priority_client_rate = " << entry.getPriorityClientRate() << ", "
          << "client_patience_time = " << entry.getClientPatienceTime() << " "
          << "WHERE id = " << configId;
    
    return db.executeQuery(query.str());
}

bool SimulationRepository::deleteConfiguration(int configId) {
    std::stringstream query;
    query << "DELETE FROM simulation_configurations WHERE id = " << configId;
    return db.executeQuery(query.str());
}

std::vector<ConfigurationSummary> SimulationRepository::getAllConfigurations() {
    std::vector<ConfigurationSummary> configurations;
    
    std::string query = "SELECT sc.id, sc.name, sc.simulation_duration, sc.cashier_count, "
                       "sc.client_arrival_interval, sc.min_service_time, sc.max_service_time, "
                       "sc.priority_client_rate, sc.client_patience_time, sc.created_at, "
                       "COUNT(s.id) as execution_count, "
                       "COALESCE(AVG(s.client_satisfaction_rate), 0) as avg_satisfaction_rate, "
                       "COALESCE(AVG(s.average_waiting_time), 0) as avg_waiting_time, "
                       "COALESCE(AVG(s.cashier_occupation_rate), 0) as avg_occupation_rate "
                       "FROM simulation_configurations sc "
                       "LEFT JOIN simulations s ON sc.id = s.configuration_id "
                       "GROUP BY sc.id ORDER BY sc.created_at DESC";
    
    MYSQL_RES* result = db.executeSelect(query);
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            ConfigurationSummary config;
            config.id = std::atoi(row[0]);
            config.name = row[1] ? row[1] : "";
            
            // Créer SimulationEntry
            config.entry = SimulationEntry(
                std::atoi(row[2]),      // simulation_duration
                std::atoi(row[3]),      // cashier_count
                std::atoi(row[4]),      // client_arrival_interval
                std::atoi(row[5]),      // min_service_time
                std::atoi(row[6]),      // max_service_time
                std::atof(row[7]),      // priority_client_rate
                std::atoi(row[8])       // client_patience_time
            );
            
            config.createdAt = row[9] ? row[9] : "";
            config.executionCount = std::atoi(row[10]);
            config.avgSatisfactionRate = std::atof(row[11]);
            config.avgWaitingTime = std::atof(row[12]);
            config.avgOccupationRate = std::atof(row[13]);
            
            configurations.push_back(config);
        }
        mysql_free_result(result);
    }
    
    return configurations;
}

ConfigurationSummary SimulationRepository::getConfiguration(int configId) {
    std::stringstream query;
    query << "SELECT sc.id, sc.name, sc.simulation_duration, sc.cashier_count, "
          << "sc.client_arrival_interval, sc.min_service_time, sc.max_service_time, "
          << "sc.priority_client_rate, sc.client_patience_time, sc.created_at, "
          << "COUNT(s.id) as execution_count, "
          << "COALESCE(AVG(s.client_satisfaction_rate), 0) as avg_satisfaction_rate, "
          << "COALESCE(AVG(s.average_waiting_time), 0) as avg_waiting_time, "
          << "COALESCE(AVG(s.cashier_occupation_rate), 0) as avg_occupation_rate "
          << "FROM simulation_configurations sc "
          << "LEFT JOIN simulations s ON sc.id = s.configuration_id "
          << "WHERE sc.id = " << configId << " GROUP BY sc.id";
    
    MYSQL_RES* result = db.executeSelect(query.str());
    if (result) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            ConfigurationSummary config;
            config.id = std::atoi(row[0]);
            config.name = row[1] ? row[1] : "";
            config.entry = SimulationEntry(
                std::atoi(row[2]), std::atoi(row[3]), std::atoi(row[4]),
                std::atoi(row[5]), std::atoi(row[6]), std::atof(row[7]), std::atoi(row[8])
            );
            config.createdAt = row[9] ? row[9] : "";
            config.executionCount = std::atoi(row[10]);
            config.avgSatisfactionRate = std::atof(row[11]);
            config.avgWaitingTime = std::atof(row[12]);
            config.avgOccupationRate = std::atof(row[13]);
            
            mysql_free_result(result);
            return config;
        }
        mysql_free_result(result);
    }
    
    throw DatabaseException("Configuration not found with ID: " + std::to_string(configId));
}

ConfigurationSummary SimulationRepository::getConfigurationByName(const std::string& name) {
    std::stringstream query;
    query << "SELECT sc.id, sc.name, sc.simulation_duration, sc.cashier_count, "
          << "sc.client_arrival_interval, sc.min_service_time, sc.max_service_time, "
          << "sc.priority_client_rate, sc.client_patience_time, sc.created_at, "
          << "COUNT(s.id) as execution_count, "
          << "COALESCE(AVG(s.client_satisfaction_rate), 0) as avg_satisfaction_rate, "
          << "COALESCE(AVG(s.average_waiting_time), 0) as avg_waiting_time, "
          << "COALESCE(AVG(s.cashier_occupation_rate), 0) as avg_occupation_rate "
          << "FROM simulation_configurations sc "
          << "LEFT JOIN simulations s ON sc.id = s.configuration_id "
          << "WHERE sc.name = '" << db.escapeString(name) << "' GROUP BY sc.id";
    
    MYSQL_RES* result = db.executeSelect(query.str());
    if (result) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            ConfigurationSummary config;
            config.id = std::atoi(row[0]);
            config.name = row[1] ? row[1] : "";
            config.entry = SimulationEntry(
                std::atoi(row[2]), std::atoi(row[3]), std::atoi(row[4]),
                std::atoi(row[5]), std::atoi(row[6]), std::atof(row[7]), std::atoi(row[8])
            );
            config.createdAt = row[9] ? row[9] : "";
            config.executionCount = std::atoi(row[10]);
            config.avgSatisfactionRate = std::atof(row[11]);
            config.avgWaitingTime = std::atof(row[12]);
            config.avgOccupationRate = std::atof(row[13]);
            
            mysql_free_result(result);
            return config;
        }
        mysql_free_result(result);
    }
    
    throw DatabaseException("Configuration not found with name: " + name);
}

int SimulationRepository::saveSimulation(int configId, SimulationNew* simulation) {
    if (!simulation) {
        throw DatabaseException("Simulation pointer is null");
    }
    
    db.beginTransaction();
    
    try {
        // Sauvegarder les résultats de simulation
        std::stringstream query;
        query << "INSERT INTO simulations "
              << "(configuration_id, actual_duration, served_client_count, non_served_client_count, "
              << "average_waiting_time, average_service_time, cashier_occupation_rate, "
              << "client_satisfaction_rate, simulation_duration_counter, cashier_occupation_counter) "
              << "VALUES ("
              << configId << ", "
              << simulation->getCurrentTime() << ", "
              << simulation->getServedClientCount() << ", "
              << simulation->getNonServedClientCount() << ", "
              << simulation->getAverageWaitingTime() << ", "
              << simulation->getAverageServiceTime() << ", "
              << simulation->getCashierOccupationRate() << ", "
              << simulation->getClientSatisfactionRate() << ", "
              << simulation->getConfiguredSimulationDuration() << ", "
              << "0)"; // cashier_occupation_counter - à calculer si nécessaire
        
        if (!db.executeQuery(query.str())) {
            db.rollback();
            return -1;
        }
        
        int simulationId = static_cast<int>(db.getLastInsertId());
        
        db.commit();
        std::cout << "Simulation saved with ID: " << simulationId << std::endl;
        return simulationId;
        
    } catch (const std::exception& e) {
        db.rollback();
        throw DatabaseException("Failed to save simulation: " + std::string(e.what()));
    }
}

bool SimulationRepository::deleteSimulation(int simulationId) {
    std::stringstream query;
    query << "DELETE FROM simulations WHERE id = " << simulationId;
    return db.executeQuery(query.str());
}

std::vector<SimulationResult> SimulationRepository::getSimulationsByConfiguration(int configId) {
    std::vector<SimulationResult> simulations;
    
    std::stringstream query;
    query << "SELECT id, configuration_id, execution_date, actual_duration, "
          << "served_client_count, non_served_client_count, average_waiting_time, "
          << "average_service_time, cashier_occupation_rate, client_satisfaction_rate, "
          << "simulation_duration_counter, cashier_occupation_counter "
          << "FROM simulations WHERE configuration_id = " << configId
          << " ORDER BY execution_date DESC";
    
    MYSQL_RES* result = db.executeSelect(query.str());
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            SimulationResult sim;
            sim.simulationId = std::atoi(row[0]);
            sim.configurationId = std::atoi(row[1]);
            sim.executionDate = row[2] ? row[2] : "";
            sim.actualDuration = std::atoi(row[3]);
            sim.servedClientCount = std::atoi(row[4]);
            sim.nonServedClientCount = std::atoi(row[5]);
            sim.averageWaitingTime = std::atof(row[6]);
            sim.averageServiceTime = std::atof(row[7]);
            sim.cashierOccupationRate = std::atof(row[8]);
            sim.clientSatisfactionRate = std::atof(row[9]);
            sim.simulationDurationCounter = std::atoi(row[10]);
            sim.cashierOccupationCounter = std::atoi(row[11]);
            
            simulations.push_back(sim);
        }
        mysql_free_result(result);
    }
    
    return simulations;
}

std::vector<SimulationResult> SimulationRepository::getRecentSimulations(int limit) {
    std::vector<SimulationResult> simulations;
    
    std::stringstream query;
    query << "SELECT id, configuration_id, execution_date, actual_duration, "
          << "served_client_count, non_served_client_count, average_waiting_time, "
          << "average_service_time, cashier_occupation_rate, client_satisfaction_rate, "
          << "simulation_duration_counter, cashier_occupation_counter "
          << "FROM simulations ORDER BY execution_date DESC LIMIT " << limit;
    
    MYSQL_RES* result = db.executeSelect(query.str());
    if (result) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            SimulationResult sim;
            sim.simulationId = std::atoi(row[0]);
            sim.configurationId = std::atoi(row[1]);
            sim.executionDate = row[2] ? row[2] : "";
            sim.actualDuration = std::atoi(row[3]);
            sim.servedClientCount = std::atoi(row[4]);
            sim.nonServedClientCount = std::atoi(row[5]);
            sim.averageWaitingTime = std::atof(row[6]);
            sim.averageServiceTime = std::atof(row[7]);
            sim.cashierOccupationRate = std::atof(row[8]);
            sim.clientSatisfactionRate = std::atof(row[9]);
            sim.simulationDurationCounter = std::atoi(row[10]);
            sim.cashierOccupationCounter = std::atoi(row[11]);
            
            simulations.push_back(sim);
        }
        mysql_free_result(result);
    }
    
    return simulations;
}

bool SimulationRepository::saveClient(int simulationId, const std::shared_ptr<AbstractClient>& client, bool wasServed) {
    std::stringstream query;
    query << "INSERT INTO clients "
          << "(simulation_id, client_type, arrival_time, service_start_time, departure_time, "
          << "patience_time, operation_type, operation_service_time, is_urgent, is_priority, "
          << "was_served, waiting_time, actual_service_time) VALUES ("
          << simulationId << ", '"
          << getClientType(client.get()) << "', "
          << client->getArrivalTime() << ", ";
    
    if (client->getServiceStartTime() >= 0) {
        query << client->getServiceStartTime();
    } else {
        query << "NULL";
    }
    
    query << ", " << client->getDepartureTime() << ", "
          << client->getPatienceTime() << ", '"
          << getOperationType(client.get()) << "', "
          << client->getOperation()->getServiceTime() << ", "
          << (isUrgentOperation(client.get()) ? "1" : "0") << ", "
          << (isPriorityClient(client.get()) ? "1" : "0") << ", "
          << (wasServed ? "1" : "0") << ", ";
    
    // Waiting time
    if (wasServed && client->getServiceStartTime() >= 0) {
        query << (client->getServiceStartTime() - client->getArrivalTime());
    } else {
        query << "NULL";
    }
    
    query << ", ";
    
    // Actual service time
    if (wasServed && client->getServiceStartTime() >= 0 && client->getDepartureTime() >= 0) {
        query << (client->getDepartureTime() - client->getServiceStartTime());
    } else {
        query << "NULL";
    }
    
    query << ")";
    
    return db.executeQuery(query.str());
}

bool SimulationRepository::testConnection() {
    std::string query = "SELECT 1";
    MYSQL_RES* result = db.executeSelect(query);
    if (result) {
        mysql_free_result(result);
        return true;
    }
    return false;
}

void SimulationRepository::cleanup() {
    // Nettoyer les anciennes données si nécessaire
    std::string query = "DELETE FROM simulations WHERE execution_date < DATE_SUB(NOW(), INTERVAL 1 YEAR)";
    db.executeQuery(query);
}

std::string SimulationRepository::getOperationType(AbstractClient* client) {
    if (dynamic_cast<Consultation*>(client->getOperation())) {
        return "Consultation";
    } else if (dynamic_cast<Transfer*>(client->getOperation())) {
        return "Transfer";
    } else if (dynamic_cast<Withdraw*>(client->getOperation())) {
        return "Withdraw";
    }
    return "Unknown";
}

std::string SimulationRepository::getClientType(AbstractClient* client) {
    if (dynamic_cast<VIPClient*>(client)) {
        return "VIPClient";
    } else if (dynamic_cast<ClientNew*>(client)) {
        return "ClientNew";
    }
    return "Unknown";
}

bool SimulationRepository::isUrgentOperation(AbstractClient* client) {
    return client->getOperation()->isUrgent();
}

bool SimulationRepository::isPriorityClient(AbstractClient* client) {
    return client->isPriority();
}
