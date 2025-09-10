#include "include/SimulationNew.h"
#include "include/SimulationEntry.h"
#include <iostream>
#include <limits>

void displayMenu() {
    std::cout << "\n=== BANK SIMULATION - JAVA-LIKE VERSION ===" << std::endl;
    std::cout << "1. Quick simulation (default parameters)" << std::endl;
    std::cout << "2. Custom simulation" << std::endl;
    std::cout << "3. Demonstration simulation" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choice: ";
}

SimulationEntry getDefaultParameters() {
    return SimulationEntry(
        100,    // simulationDuration
        3,      // cashierCount
        5,      // clientArrivalInterval
        3,      // minServiceTime
        8,      // maxServiceTime
        0.2,    // priorityClientRate (20%)
        15      // clientPatienceTime
    );
}

SimulationEntry getCustomParameters() {
    int duration, cashiers, interval, minService, maxService, patience;
    double vipRate;
    
    std::cout << "\n=== CUSTOM CONFIGURATION ===" << std::endl;
    
    std::cout << "Simulation duration (time units): ";
    std::cin >> duration;
    
    std::cout << "Number of cashiers: ";
    std::cin >> cashiers;
    
    std::cout << "Client arrival interval (units): ";
    std::cin >> interval;
    
    std::cout << "Minimum service time: ";
    std::cin >> minService;
    
    std::cout << "Maximum service time: ";
    std::cin >> maxService;
    
    std::cout << "VIP client rate (0.0 to 1.0): ";
    std::cin >> vipRate;
    
    std::cout << "Client patience time (units): ";
    std::cin >> patience;
    
    return SimulationEntry(duration, cashiers, interval, minService, maxService, vipRate, patience);
}

SimulationEntry getDemoParameters() {
    return SimulationEntry(
        50,     // simulationDuration (shorter for demo)
        2,      // cashierCount
        3,      // clientArrivalInterval (more frequent)
        2,      // minServiceTime
        5,      // maxServiceTime
        0.3,    // priorityClientRate (30%)
        10      // clientPatienceTime
    );
}

void executeSimulation(const SimulationEntry& entry) {
    try {
        SimulationNew sim(entry);
        sim.simulate();
        std::cout << "\n" << sim.simulationResults() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error during simulation: " << e.what() << std::endl;
    }
}

void clearBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    std::cout << "=== BANK SIMULATION - C++ PROJECT ===" << std::endl;
    std::cout << "Developed according to Java reference specifications" << std::endl;
    std::cout << "Features:" << std::endl;
    std::cout << "- Management of normal and VIP clients" << std::endl;
    std::cout << "- Three operation types (consultation, transfer, withdraw)" << std::endl;
    std::cout << "- Single queue with VIP priority" << std::endl;
    std::cout << "- Consultation clients leave after patience limit" << std::endl;
    std::cout << "- Detailed statistics" << std::endl;
    
    int choice;
    do {
        displayMenu();
        std::cin >> choice;
        
        if (std::cin.fail()) {
            clearBuffer();
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }
        
        switch (choice) {
            case 1: {
                std::cout << "\n--- SIMULATION WITH DEFAULT PARAMETERS ---" << std::endl;
                auto params = getDefaultParameters();
                executeSimulation(params);
                break;
            }
            case 2: {
                auto params = getCustomParameters();
                executeSimulation(params);
                break;
            }
            case 3: {
                std::cout << "\n--- DEMONSTRATION SIMULATION ---" << std::endl;
                auto params = getDemoParameters();
                executeSimulation(params);
                break;
            }
            case 0:
                std::cout << "Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
        
        if (choice != 0) {
            std::cout << "\nPress Enter to continue...";
            clearBuffer();
            std::cin.get();
        }
        
    } while (choice != 0);
    
    return 0;
}
