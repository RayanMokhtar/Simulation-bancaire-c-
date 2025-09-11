#ifndef SIMULATION_REPOSITORY_H
#define SIMULATION_REPOSITORY_H

#ifdef WITH_PERSISTENCE
#include "persistence/DatabaseConnection.h"
#include "simulation/SimulationEntry.h"
#include "simulation/SimulationNew.h"
#include "client/AbstractClient.h"
#include <vector>
#include <memory>
#include <string>

struct SimulationResult {
    int simulationId;
    int configurationId;
    std::string executionDate;
    int actualDuration;
    int servedClientCount;
    int nonServedClientCount;
    double averageWaitingTime;
    double averageServiceTime;
    double cashierOccupationRate;
    double clientSatisfactionRate;
    int simulationDurationCounter;
    int cashierOccupationCounter;
};

struct ConfigurationSummary {
    int id;
    std::string name;
    SimulationEntry entry;
    std::string createdAt;
    int executionCount;
    double avgSatisfactionRate;
    double avgWaitingTime;
    double avgOccupationRate;
};

class SimulationRepository {
private:
    DatabaseConnection& db;

public:
    SimulationRepository(DatabaseConnection& database = DatabaseConnection::getInstance());
    
    // Gestion des configurations
    int saveConfiguration(const std::string& name, const SimulationEntry& entry);
    bool updateConfiguration(int configId, const std::string& name, const SimulationEntry& entry);
    bool deleteConfiguration(int configId);
    std::vector<ConfigurationSummary> getAllConfigurations();
    ConfigurationSummary getConfiguration(int configId);
    ConfigurationSummary getConfigurationByName(const std::string& name);
    
    // Gestion des simulations
    int saveSimulation(int configId, SimulationNew* simulation);
    bool deleteSimulation(int simulationId);
    std::vector<SimulationResult> getSimulationsByConfiguration(int configId);
    std::vector<SimulationResult> getRecentSimulations(int limit = 10);
    SimulationResult getSimulation(int simulationId);
    
    // Gestion des clients
    bool saveClient(int simulationId, const std::shared_ptr<AbstractClient>& client, bool wasServed);
    std::vector<std::shared_ptr<AbstractClient>> getClientsBySimulation(int simulationId);
    
    // Statistiques et rapports
    std::vector<SimulationResult> getSimulationHistory(const std::string& startDate, const std::string& endDate);
    double getAverageSatisfactionByConfiguration(int configId);
    std::vector<std::pair<std::string, double>> getPerformanceComparison();
    
    // Méthodes utilitaires
    bool testConnection();
    void cleanup();

private:
    std::string getOperationType(AbstractClient* client);
    std::string getClientType(AbstractClient* client);
    bool isUrgentOperation(AbstractClient* client);
    bool isPriorityClient(AbstractClient* client);
};

#else
// Version stub sans MySQL
#include <memory>
class AbstractClient;
class DatabaseConnection;

class SimulationRepository {
public:
    SimulationRepository(DatabaseConnection&) {}
    bool saveClient(int, const std::shared_ptr<AbstractClient>&, bool) { return false; }
};
#endif

#endif
