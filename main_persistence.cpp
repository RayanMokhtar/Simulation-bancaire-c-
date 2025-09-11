#include "persistence/PersistenceManager.h"
#include "simulation/SimulationEntry.h"
#include "simulation/SimulationNew.h"
#include <iostream>

int main() {
    std::cout << "=== DÉMONSTRATION AVEC PERSISTANCE MYSQL ===" << std::endl;
    
    // Initialiser le gestionnaire de persistance
    PersistenceManager persistence;
    
    // Essayer d'initialiser à partir du fichier de configuration
    if (!persistence.initializeFromConfig()) {
        std::cerr << "Erreur d'initialisation de la persistance: " << persistence.getLastError() << std::endl;
        std::cout << "\nVérifiez votre configuration dans config/database.conf" << std::endl;
        std::cout << "Assurez-vous que MySQL est installé et que la base 'simulation_bancaire' existe." << std::endl;
        std::cout << "Vous pouvez créer la base avec le script: scripts/create_database.sql" << std::endl;
        return 1;
    }
    
    std::cout << "Persistance initialisée avec succès!" << std::endl;
    
    try {
        // Créer une configuration de simulation
        SimulationEntry config(
            50,     // durée
            3,      // caissiers
            4,      // intervalle arrivée
            2,      // service min
            6,      // service max
            0.25,   // taux VIP
            12      // patience
        );
        
        // Sauvegarder la configuration
        int configId = persistence.saveSimulationConfiguration("Demo avec persistance", config);
        if (configId <= 0) {
            std::cerr << "Erreur lors de la sauvegarde de la configuration" << std::endl;
            return 1;
        }
        
        std::cout << "Configuration sauvegardée avec l'ID: " << configId << std::endl;
        
        // Exécuter et sauvegarder une simulation
        int simulationId = persistence.runAndSaveSimulation(configId);
        if (simulationId <= 0) {
            std::cerr << "Erreur lors de l'exécution de la simulation" << std::endl;
            return 1;
        }
        
        std::cout << "Simulation exécutée et sauvegardée avec l'ID: " << simulationId << std::endl;
        
        // Lister toutes les configurations
        std::cout << "\n=== CONFIGURATIONS DISPONIBLES ===" << std::endl;
        auto configurations = persistence.listConfigurations();
        for (const auto& conf : configurations) {
            std::cout << "ID: " << conf.id 
                      << ", Nom: " << conf.name
                      << ", Exécutions: " << conf.executionCount
                      << ", Satisfaction moyenne: " << conf.avgSatisfactionRate << "%" << std::endl;
        }
        
        // Afficher l'historique des simulations pour cette configuration
        std::cout << "\n=== HISTORIQUE DES SIMULATIONS ===" << std::endl;
        auto history = persistence.getSimulationHistory(configId);
        for (const auto& sim : history) {
            std::cout << "Simulation ID: " << sim.simulationId
                      << ", Date: " << sim.executionDate
                      << ", Clients servis: " << sim.servedClientCount
                      << ", Satisfaction: " << sim.clientSatisfactionRate << "%" << std::endl;
        }
        
        // Générer un rapport
        std::string reportFile = "rapport_simulation_" + std::to_string(simulationId) + ".txt";
        persistence.generateReport(simulationId, reportFile);
        std::cout << "\nRapport généré: " << reportFile << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Erreur durant l'exécution: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n=== DÉMONSTRATION TERMINÉE ===" << std::endl;
    std::cout << "✓ Configuration sauvegardée en base de données" << std::endl;
    std::cout << "✓ Simulation exécutée et résultats persistés" << std::endl;
    std::cout << "✓ Historique des simulations accessible" << std::endl;
    std::cout << "✓ Rapport généré" << std::endl;
    
    return 0;
}
