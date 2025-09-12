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

### Dépendances MySQL locales (sans installation système)
Vous pouvez compiler/faire tourner sans installer le Connector globalement. Placez simplement les fichiers nécessaires dans:

- `third_party/mysql/include/` → doit contenir `mysql.h` (et headers associés)
- `third_party/mysql/lib/` → doit contenir l'import lib MinGW: `libmysql.dll.a` (ou `libmariadb.dll.a`)
- `third_party/mysql/bin/` → doit contenir la DLL runtime: `libmysql.dll` (ou `libmariadb.dll`)

Un aide-mémoire est disponible dans `third_party/mysql/README.txt`.

### Version sans persistance MySQL

# Compilation manuelle
g++ -std=c++17 -O2 -municode -DUNICODE -D_UNICODE -Wall -Wextra -pedantic main_gui_windows.cpp src/bank/Bank.cpp src/bank/Caissier.cpp src/bank/FileAttente.cpp src/simulation/SimulationNew.cpp src/simulation/SimulationUtility.cpp src/simulation/StatisticManager.cpp src/simulation/AbstractClient.cpp src/simulation/AbstractOperation.cpp src/client/ClientNew.cpp src/client/VIPClient.cpp src/client/Consultation.cpp src/client/Transfer.cpp src/client/Withdraw.cpp src/WindowsGUI.cpp -Iinclude -lgdi32 -luser32 -o simulation_gui_windows.exe


.\simulation_gui_windows.exe


### Version avec persistance MySQL
# Build relatif (MinGW, Windows, GUI + MySQL) — aucun install MySQL requis
g++ -std=c++17 -DWITH_PERSISTENCE `
  -I"include" -I"third_party/mysql/include" `
  main_gui_windows.cpp src/WindowsGUI.cpp src/bank/*.cpp src/client/*.cpp src/simulation/*.cpp src/persistence/*.cpp `
  -L"third_party/mysql/lib" -lmysql `
  -lws2_32 -lsecur32 -lcrypt32 -lbcrypt -luser32 -lgdi32 -lkernel32 -lcomctl32 -lole32 -loleaut32 -luuid `
  -mwindows -municode -o BankSimulation_MySQL.exe

Copy-Item -Force "third_party/mysql/bin/libmysql.dll" ".\libmysql.dll"


.\BankSimulation_MySQL.exe  


Notes:
- Le nom de la lib `-lmysql` suppose un fichier `libmysql.dll.a` dans `third_party/mysql/lib`. Si vous utilisez MariaDB, remplacez par `-lmariadb` et fournissez `libmariadb.dll.a`+`libmariadb.dll`.
- L’EXE requiert la DLL au runtime (copie locale ou PATH).

.\BankSimulation_MySQL.exe  