#include "include/SimulationNew.h"
#include "include/SimulationEntry.h"
#include <iostream>

int main() {
    std::cout << "=== DÉMONSTRATION AUTOMATIQUE - SIMULATION BANCAIRE C++ ===" << std::endl;
    std::cout << "Version conforme à l'architecture Java de référence\n" << std::endl;
    
    // Démonstration avec paramètres par défaut
    std::cout << "=== SCÉNARIO DE DÉMONSTRATION ===" << std::endl;
    SimulationEntry params(
        30,     // simulationDuration (courte pour demo)
        2,      // cashierCount
        3,      // clientArrivalInterval
        2,      // minServiceTime
        5,      // maxServiceTime
        0.3,    // priorityClientRate (30% VIP)
        8       // clientPatienceTime
    );
    
    std::cout << "Paramètres:" << std::endl;
    std::cout << "- Durée: " << params.getSimulationDuration() << " unités" << std::endl;
    std::cout << "- Caissiers: " << params.getCashierCount() << std::endl;
    std::cout << "- Intervalle clients: " << params.getClientArrivalInterval() << std::endl;
    std::cout << "- Service: " << params.getMinServiceTime() << "-" << params.getMaxServiceTime() << " unités" << std::endl;
    std::cout << "- Taux VIP: " << (params.getPriorityClientRate() * 100) << "%" << std::endl;
    std::cout << "- Patience: " << params.getClientPatienceTime() << " unités" << std::endl;
    std::cout << std::endl;
    
    try {
        SimulationNew simulation;
        simulation.run(params);
    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n=== DÉMONSTRATION TERMINÉE ===" << std::endl;
    std::cout << "La simulation suit exactement l'architecture Java :" << std::endl;
    std::cout << "✓ AbstractClient → ClientNew/VIPClient" << std::endl;
    std::cout << "✓ AbstractOperation → Consultation/Transfer/Withdraw" << std::endl;
    std::cout << "✓ Priorité VIP dans la file d'attente" << std::endl;
    std::cout << "✓ Gestion de patience pour consultations" << std::endl;
    std::cout << "✓ Statistiques détaillées" << std::endl;
    
    return 0;
}
