#include <iostream>
#include <chrono>
#include <thread>

#ifdef WITH_PERSISTENCE
#include "persistence/DatabaseConnection.h"
#include "persistence/SimulationRepository.h"
#include "persistence/PersistenceManager.h"
#endif

#include "simulation/SimulationNew.h"
#include "simulation/SimulationEntry.h"

int main() {
    std::cout << "Test de persistance avec MySQL..." << std::endl;

#ifdef WITH_PERSISTENCE
    try {
        std::cout << "Initialisation de la connexion à la base de données..." << std::endl;
        
        // Initialiser la connexion à la base de données
        DatabaseConnection& db = DatabaseConnection::getInstance();
        std::cout << "Connexion à la base de données réussie." << std::endl;

        // Créer le repository
        SimulationRepository repository(db);
        std::cout << "Repository créé." << std::endl;
        std::cout << "État de la connexion: " << (db.isConnected() ? "connecté" : "déconnecté") << std::endl;

        // Créer une configuration de test
        SimulationEntry entry(60, 2, 5, 3, 7, 0.3, 15); // 60 sec, 2 caissiers, etc.
        std::cout << "Configuration créée: 60s, 2 caissiers" << std::endl;

        // Sauvegarder la configuration
        int configId = repository.saveConfiguration("Test Persistance Auto", entry);
        std::cout << "Configuration sauvegardée avec ID: " << configId << std::endl;

        // Créer une nouvelle simulation avec l'entry
        SimulationNew simulation(entry);
        std::cout << "Simulation configurée." << std::endl;

        // Activer la persistance sur la simulation (sans ID spécifique au début)
        simulation.enablePersistence(&repository);
        std::cout << "Persistance activée sur la simulation." << std::endl;

        // Lancer la simulation
        std::cout << "Lancement de la simulation..." << std::endl;
        simulation.simulate();

        // Sauvegarder la simulation complète
        int simId = repository.saveSimulation(configId, &simulation);
        std::cout << "Simulation sauvegardée avec ID: " << simId << std::endl;

        // Afficher les résultats
        std::cout << "\n=== Résultats ===" << std::endl;
        std::cout << simulation.simulationResults() << std::endl;

        std::cout << "\n=== Vérification dans la base ===" << std::endl;
        auto configs = repository.getAllConfigurations();
        std::cout << "Configurations dans la base: " << configs.size() << std::endl;
        for (const auto& config : configs) {
            std::cout << "- " << config.name << " (ID: " << config.id << ")" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }
#else
    std::cout << "WITH_PERSISTENCE non défini - persistance désactivée" << std::endl;
    return 1;
#endif

    std::cout << "Test terminé avec succès!" << std::endl;
    return 0;
}
