# Simulation Bancaire C++ avec Persistance MySQL

Système de simulation bancaire développé en C++ avec architecture en packages et support de persistance MySQL.

## Architecture

### Packages
- **bank/**: Gestion des banques, caissiers et files d'attente
- **client/**: Hiérarchie des clients et opérations
- **simulation/**: Moteur de simulation et statistiques
- **persistence/**: Couche de persistance MySQL

### Structure des fichiers
```
demo_cpp/
├── include/
│   ├── bank/           # Headers banque
│   ├── client/         # Headers clients
│   ├── simulation/     # Headers simulation
│   └── persistence/    # Headers persistance
├── src/
│   ├── bank/           # Implémentations banque
│   ├── client/         # Implémentations clients
│   ├── simulation/     # Implémentations simulation
│   └── persistence/    # Implémentations persistance
├── scripts/
│   └── create_database.sql  # Script création BDD
└── *.cpp               # Fichiers principaux
```

## Installation

### Prérequis
1. **Compilateur C++17** (MinGW/GCC)
2. **MySQL Server** (8.0 recommandé)
3. **MySQL Connector/C++** pour la persistance

### Installation MySQL Connector/C++
# Télécharger depuis: https://dev.mysql.com/downloads/connector/cpp/
# Installer dans C:\Program Files\MySQL\Connector C++ 8.0\'''

### Version sans persistance MySQL

# Compilation manuelle
g++ -std=c++17 -O2 -municode -DUNICODE -D_UNICODE -Wall -Wextra -pedantic main_gui_windows.cpp src/bank/Bank.cpp src/bank/Caissier.cpp src/bank/FileAttente.cpp src/simulation/SimulationNew.cpp src/simulation/SimulationUtility.cpp src/simulation/StatisticManager.cpp src/simulation/AbstractClient.cpp src/simulation/AbstractOperation.cpp src/client/ClientNew.cpp src/client/VIPClient.cpp src/client/Consultation.cpp src/client/Transfer.cpp src/client/Withdraw.cpp src/WindowsGUI.cpp -Iinclude -lgdi32 -luser32 -o simulation_gui_windows.exe


.\simulation_gui_windows.exe