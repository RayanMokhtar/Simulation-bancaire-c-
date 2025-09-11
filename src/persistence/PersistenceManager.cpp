#include "persistence/PersistenceManager.h"
#include "persistence/DatabaseConfig.h"
#include <iostream>
#include <fstream>
#include <sstream>

PersistenceManager::PersistenceManager() : initialized(false) {}

PersistenceManager::~PersistenceManager() {
    if (connection) {
        connection->disconnect();
    }
}

bool PersistenceManager::initialize(const std::string& host,
                                   const std::string& username,
                                   const std::string& password,
                                   const std::string& database,
                                   unsigned int port) {
    try {
        connection = std::make_unique<DatabaseConnection>(host, username, password, database, port);
        
        if (!connection->connect()) {
            setError("Failed to connect to database");
            return false;
        }
        
        repository = std::make_unique<SimulationRepository>(*connection);
        
        if (!repository->testConnection()) {
            setError("Failed to test database connection");
            return false;
        }
        
        initialized = true;
        std::cout << "PersistenceManager initialized successfully" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        setError("Initialization error: " + std::string(e.what()));
        return false;
    }
}

// Nouvelle méthode pour initialiser à partir du fichier de config
bool PersistenceManager::initializeFromConfig(const std::string& configFile, const std::string& section) {
    try {
        DatabaseConfig config(configFile);
        if (!config.loadConfig()) {
            setError("Failed to load database configuration: " + config.getConfigError());
            return false;
        }
        
        return initialize(
            config.getHost(section),
            config.getUsername(section),
            config.getPassword(section),
            config.getDatabase(section),
            config.getPort(section)
        );
        
    } catch (const std::exception& e) {
        setError("Configuration initialization error: " + std::string(e.what()));
        return false;
    }
}

int PersistenceManager::saveSimulationConfiguration(const std::string& name, const SimulationEntry& entry) {
    if (!initialized) {
        setError("PersistenceManager not initialized");
        return -1;
    }
    
    try {
        int configId = repository->saveConfiguration(name, entry);
        std::cout << "Configuration '" << name << "' saved with ID: " << configId << std::endl;
        return configId;
    } catch (const std::exception& e) {
        setError("Failed to save configuration: " + std::string(e.what()));
        return -1;
    }
}

int PersistenceManager::runAndSaveSimulation(int configId) {
    if (!initialized) {
        setError("PersistenceManager not initialized");
        return -1;
    }
    
    try {
        // Récupérer la configuration
        ConfigurationSummary config = repository->getConfiguration(configId);
        
        // Créer et exécuter la simulation
        SimulationNew simulation(config.entry);
        simulation.simulate();
        
        // Sauvegarder les résultats
        int simulationId = repository->saveSimulation(configId, &simulation);
        
        std::cout << "Simulation executed and saved with ID: " << simulationId << std::endl;
        return simulationId;
        
    } catch (const std::exception& e) {
        setError("Failed to run and save simulation: " + std::string(e.what()));
        return -1;
    }
}

int PersistenceManager::runAndSaveSimulation(const std::string& configName) {
    if (!initialized) {
        setError("PersistenceManager not initialized");
        return -1;
    }
    
    try {
        // Récupérer la configuration par nom
        ConfigurationSummary config = repository->getConfigurationByName(configName);
        return runAndSaveSimulation(config.id);
        
    } catch (const std::exception& e) {
        setError("Failed to run and save simulation by name: " + std::string(e.what()));
        return -1;
    }
}

std::vector<ConfigurationSummary> PersistenceManager::listConfigurations() {
    if (!initialized) {
        setError("PersistenceManager not initialized");
        return {};
    }
    
    try {
        return repository->getAllConfigurations();
    } catch (const std::exception& e) {
        setError("Failed to list configurations: " + std::string(e.what()));
        return {};
    }
}

std::vector<SimulationResult> PersistenceManager::getSimulationHistory(int configId) {
    if (!initialized) {
        setError("PersistenceManager not initialized");
        return {};
    }
    
    try {
        return repository->getSimulationsByConfiguration(configId);
    } catch (const std::exception& e) {
        setError("Failed to get simulation history: " + std::string(e.what()));
        return {};
    }
}

void PersistenceManager::generateReport(int simulationId, const std::string& outputFile) {
    if (!initialized) {
        setError("PersistenceManager not initialized");
        return;
    }
    
    try {
        // Récupérer les résultats de simulation
        std::vector<SimulationResult> results = repository->getRecentSimulations(1);
        if (results.empty()) {
            setError("No simulation found with ID: " + std::to_string(simulationId));
            return;
        }
        
        // Générer le rapport
        std::ofstream file(outputFile);
        if (!file.is_open()) {
            setError("Failed to open output file: " + outputFile);
            return;
        }
        
        SimulationResult result = results[0];
        
        file << "=== RAPPORT DE SIMULATION ===" << std::endl;
        file << "ID de simulation: " << result.simulationId << std::endl;
        file << "Date d'exécution: " << result.executionDate << std::endl;
        file << "Durée effective: " << result.actualDuration << std::endl;
        file << "Clients servis: " << result.servedClientCount << std::endl;
        file << "Clients non servis: " << result.nonServedClientCount << std::endl;
        file << "Temps d'attente moyen: " << result.averageWaitingTime << std::endl;
        file << "Temps de service moyen: " << result.averageServiceTime << std::endl;
        file << "Taux d'occupation des caissiers: " << result.cashierOccupationRate << "%" << std::endl;
        file << "Taux de satisfaction client: " << result.clientSatisfactionRate << "%" << std::endl;
        
        file.close();
        std::cout << "Report generated: " << outputFile << std::endl;
        
    } catch (const std::exception& e) {
        setError("Failed to generate report: " + std::string(e.what()));
    }
}
