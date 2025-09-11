#include "persistence/PersistenceManager.h"
#include "simulation/SimulationEntry.h"
#include <iostream>
#include <vector>

int main() {
    std::cout << "=== DÉMONSTRATION SIMULATION BANCAIRE AVEC PERSISTANCE MYSQL ===" << std::endl;
    
    // Initialiser le gestionnaire de persistance
    PersistenceManager persistenceManager;
    
    // Configuration de connexion (à adapter selon votre configuration MySQL)
    std::string host = "localhost";
    std::string username = "root";
    std::string password = ""; // Mot de passe de votre base MySQL
    std::string database = "simulation_bancaire";
    unsigned int port = 3306;
    
    std::cout << "Connexion à la base de données MySQL..." << std::endl;
    if (!persistenceManager.initialize(host, username, password, database, port)) {
        std::cerr << "Erreur d'initialisation: " << persistenceManager.getLastError() << std::endl;
        std::cout << "Vérifiez que:" << std::endl;
        std::cout << "1. MySQL est installé et démarré" << std::endl;
        std::cout << "2. La base 'simulation_bancaire' existe (exécutez scripts/create_database.sql)" << std::endl;
        std::cout << "3. Les paramètres de connexion sont corrects" << std::endl;
        std::cout << "4. Les bibliothèques MySQL C++ sont installées" << std::endl;
        return 1;
    }
    
    std::cout << "Connexion réussie !" << std::endl << std::endl;
    
    try {
        // === CRÉATION DE CONFIGURATIONS ===
        std::cout << "=== CRÉATION DE CONFIGURATIONS DE SIMULATION ===" << std::endl;
        
        // Configuration 1: Charge normale
        SimulationEntry config1(100, 3, 5, 3, 8, 0.2, 15);
        int configId1 = persistenceManager.saveSimulationConfiguration("Charge Normale", config1);
        std::cout << "Configuration 'Charge Normale' créée avec ID: " << configId1 << std::endl;
        
        // Configuration 2: Haute charge
        SimulationEntry config2(150, 2, 3, 2, 6, 0.3, 10);
        int configId2 = persistenceManager.saveSimulationConfiguration("Haute Charge", config2);
        std::cout << "Configuration 'Haute Charge' créée avec ID: " << configId2 << std::endl;
        
        // Configuration 3: Service rapide
        SimulationEntry config3(120, 4, 4, 1, 4, 0.25, 20);
        int configId3 = persistenceManager.saveSimulationConfiguration("Service Rapide", config3);
        std::cout << "Configuration 'Service Rapide' créée avec ID: " << configId3 << std::endl;
        
        std::cout << std::endl;
        
        // === EXÉCUTION DE SIMULATIONS ===
        std::cout << "=== EXÉCUTION DE SIMULATIONS ===" << std::endl;
        
        // Exécuter plusieurs simulations pour chaque configuration
        std::vector<int> configIds = {configId1, configId2, configId3};
        std::vector<std::string> configNames = {"Charge Normale", "Haute Charge", "Service Rapide"};
        
        for (size_t i = 0; i < configIds.size(); ++i) {
            std::cout << "Exécution de simulations pour '" << configNames[i] << "'..." << std::endl;
            
            // Exécuter 3 simulations pour chaque configuration
            for (int run = 1; run <= 3; ++run) {
                std::cout << "  Simulation " << run << "/3... ";
                int simulationId = persistenceManager.runAndSaveSimulation(configIds[i]);
                if (simulationId > 0) {
                    std::cout << "OK (ID: " << simulationId << ")" << std::endl;
                } else {
                    std::cout << "ERREUR: " << persistenceManager.getLastError() << std::endl;
                }
            }
            std::cout << std::endl;
        }
        
        // === CONSULTATION DES RÉSULTATS ===
        std::cout << "=== CONSULTATION DES RÉSULTATS ===" << std::endl;
        
        // Lister toutes les configurations
        auto configurations = persistenceManager.listConfigurations();
        std::cout << "Configurations disponibles:" << std::endl;
        for (const auto& config : configurations) {
            std::cout << "  ID: " << config.id << " | Nom: " << config.name 
                      << " | Exécutions: " << config.executionCount 
                      << " | Satisfaction moy: " << config.avgSatisfactionRate << "%" << std::endl;
        }
        std::cout << std::endl;
        
        // Afficher l'historique pour chaque configuration
        for (const auto& config : configurations) {
            std::cout << "Historique pour '" << config.name << "':" << std::endl;
            auto history = persistenceManager.getSimulationHistory(config.id);
            
            for (const auto& result : history) {
                std::cout << "  Date: " << result.executionDate 
                          << " | Clients servis: " << result.servedClientCount
                          << " | Satisfaction: " << result.clientSatisfactionRate << "%"
                          << " | Attente moy: " << result.averageWaitingTime << std::endl;
            }
            std::cout << std::endl;
        }
        
        // === GÉNÉRATION DE RAPPORT ===
        if (!configurations.empty()) {
            auto recentSimulations = persistenceManager.getRepository()->getRecentSimulations(1);
            if (!recentSimulations.empty()) {
                std::cout << "Génération du rapport pour la simulation la plus récente..." << std::endl;
                persistenceManager.generateReport(recentSimulations[0].simulationId, "rapport_simulation.txt");
                std::cout << "Rapport généré: rapport_simulation.txt" << std::endl;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Erreur durant l'exécution: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << std::endl << "=== DÉMONSTRATION TERMINÉE ===" << std::endl;
    std::cout << "Fonctionnalités de persistance testées:" << std::endl;
    std::cout << "✓ Connexion à la base de données MySQL" << std::endl;
    std::cout << "✓ Sauvegarde des configurations de simulation" << std::endl;
    std::cout << "✓ Exécution et sauvegarde automatique des résultats" << std::endl;
    std::cout << "✓ Consultation de l'historique des simulations" << std::endl;
    std::cout << "✓ Génération de rapports" << std::endl;
    std::cout << "✓ Gestion des statistiques agrégées" << std::endl;
    
    return 0;
}
