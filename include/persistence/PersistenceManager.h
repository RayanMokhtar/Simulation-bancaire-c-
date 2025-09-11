#ifndef PERSISTENCE_MANAGER_H
#define PERSISTENCE_MANAGER_H

#include "persistence/DatabaseConnection.h"
#include "persistence/SimulationRepository.h"
#include "simulation/SimulationNew.h"
#include "simulation/SimulationEntry.h"
#include <memory>
#include <string>
#include <vector>

class PersistenceManager {
private:
    std::unique_ptr<DatabaseConnection> connection;
    std::unique_ptr<SimulationRepository> repository;
    bool initialized;

public:
    PersistenceManager();
    ~PersistenceManager();
    
    // Initialisation
    bool initialize(const std::string& host = "localhost",
                   const std::string& username = "root",
                   const std::string& password = "",
                   const std::string& database = "simulation_bancaire",
                   unsigned int port = 3306);
    
    // Initialisation à partir du fichier de configuration
    bool initializeFromConfig(const std::string& configFile = "config/database.conf", 
                             const std::string& section = "database");
    
    bool isInitialized() const { return initialized; }
    
    // Méthodes principales
    int saveSimulationConfiguration(const std::string& name, const SimulationEntry& entry);
    int runAndSaveSimulation(int configId);
    int runAndSaveSimulation(const std::string& configName);
    
    // Accès au repository
    SimulationRepository* getRepository() { return repository.get(); }
    
    // Méthodes de commodité
    std::vector<ConfigurationSummary> listConfigurations();
    std::vector<SimulationResult> getSimulationHistory(int configId);
    void generateReport(int simulationId, const std::string& outputFile);
    
    // Gestion des erreurs
    std::string getLastError() const { return lastError; }
    
private:
    std::string lastError;
    void setError(const std::string& error) { lastError = error; }
};

#endif
