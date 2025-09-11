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
```powershell
# Télécharger depuis: https://dev.mysql.com/downloads/connector/cpp/
# Installer dans C:\Program Files\MySQL\Connector C++ 8.0\
```

### Configuration de la base de données
```sql
-- Créer la base de données
mysql -u root -p < scripts/create_database.sql
```

## Compilation

g++ -std=c++17 -O2 -municode -DUNICODE -D_UNICODE -Wall -Wextra -pedantic main_gui_windows.cpp src/bank/Bank.cpp src/bank/Caissier.cpp src/bank/FileAttente.cpp src/simulation/SimulationNew.cpp src/simulation/SimulationUtility.cpp src/simulation/StatisticManager.cpp src/simulation/AbstractClient.cpp src/simulation/AbstractOperation.cpp src/client/ClientNew.cpp src/client/VIPClient.cpp src/client/Consultation.cpp src/client/Transfer.cpp src/client/Withdraw.cpp src/WindowsGUI.cpp -Iinclude -lgdi32 -luser32 -o simulation_gui_windows.exe

### Version sans persistance (de base)
```powershell
# Simulation Bancaire - Projet C++

Un système de simulation bancaire complet avec interface graphique Windows et persistance MySQL.

## Structure du Projet

```
demo_cpp/
├── include/                    # Headers organisés en packages
│   ├── bank/                  # Package banque (Bank, Cashier, Queue)
│   ├── client/                # Package client (AbstractClient, Operations)
│   ├── simulation/            # Package simulation (SimulationNew, Statistics)
│   ├── persistence/           # Package persistance MySQL
│   └── WindowsGUI.h          # Interface graphique Windows
├── src/                       # Implémentations
│   ├── bank/
│   ├── client/
│   ├── simulation/
│   └── persistence/
├── config/                    # Configuration
│   └── database.conf         # Configuration MySQL
├── scripts/                   # Scripts SQL
│   └── create_database.sql   # Création de la base de données
└── docs/                     # Documentation
    └── DATABASE_SETUP.md     # Guide configuration MySQL
```

## Compilation et Exécution

### Version Standard (Interface Graphique)
```bash
# Compilation
g++ -std=c++17 -O2 -municode -DUNICODE -D_UNICODE -Wall -Wextra -pedantic main_gui_windows.cpp src/bank/*.cpp src/client/*.cpp src/simulation/*.cpp src/WindowsGUI.cpp -Iinclude -lgdi32 -luser32 -o simulation_gui_windows.exe

# Exécution
.\simulation_gui_windows.exe
```

### Version avec Persistance MySQL
```bash
# Compilation (nécessite MySQL Connector/C++)
.\build_mysql.ps1

# Ou manuellement :
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main_persistence.cpp src/bank/*.cpp src/client/*.cpp src/simulation/*.cpp src/persistence/*.cpp -Iinclude -IC:\mysql-connector-cpp\include -LC:\mysql-connector-cpp\lib64 -lmysqlclient -o simulation_persistence.exe

# Exécution
.\simulation_persistence.exe
```

### Scripts de Compilation Disponibles
- `build_gui.ps1` : Compilation version interface graphique
- `build_mysql.ps1` : Compilation version avec MySQL
- `build_mysql.ps1 -Run` : Compiler et exécuter
- `build_mysql.ps1 -Help` : Aide

## Configuration de la Base de Données

### 1. Éditer le Fichier de Configuration
Modifiez `config/database.conf` avec vos identifiants MySQL :
```ini
[database]
host=localhost
port=3306
username=votre_utilisateur
password=votre_mot_de_passe
database=simulation_bancaire
```

### 2. Créer la Base de Données
```bash
mysql -u root -p < scripts/create_database.sql
```

Voir `docs/DATABASE_SETUP.md` pour le guide complet.

## Fonctionnalités

### Interface Graphique Windows
- Paramétrage des simulations (durée, caissiers, intervalles, etc.)
- Exécution pas-à-pas ou complète
- Affichage en temps réel des statistiques
- Interface native Windows avec Win32 API

### Persistance MySQL
- Sauvegarde des configurations de simulation
- Historique complet des exécutions
- Détails des clients traités
- Statistiques et rapports
- Gestion des erreurs et transactions

### Architecture en Packages
- **bank/** : Logique bancaire (banque, caissiers, file d'attente)
- **client/** : Hiérarchie des clients et opérations
- **simulation/** : Moteur de simulation et statistiques
- **persistence/** : Accès aux données MySQL

## Exemples d'Utilisation

### Interface Graphique
```cpp
WindowsGUI gui;
gui.Initialize(hInstance);
gui.Run();
```

### Persistance
```cpp
PersistenceManager persistence;
persistence.initializeFromConfig();

SimulationEntry config(100, 3, 5, 2, 8, 0.2, 15);
int configId = persistence.saveSimulationConfiguration("Ma Config", config);
int simId = persistence.runAndSaveSimulation(configId);
```

## Statistiques Calculées
- Nombre de clients servis/non servis
- Temps d'attente moyen
- Temps de service moyen
- Taux d'occupation des caissiers
- Taux de satisfaction client
- Historique complet par période

## Dépendances
- **Standard** : MinGW g++, Win32 API
- **MySQL** : MySQL Connector/C++, MySQL Server

## Documentation
- `docs/DATABASE_SETUP.md` : Configuration MySQL complète
- Code documenté avec commentaires détaillés
- Exemples d'utilisation intégrés
```

### Version sans persistance MySQL

# Compilation manuelle
g++ -std=c++17 -O2 -municode -DUNICODE -D_UNICODE -Wall -Wextra -pedantic main_gui_windows.cpp src/bank/Bank.cpp src/bank/Caissier.cpp src/bank/FileAttente.cpp src/simulation/SimulationNew.cpp src/simulation/SimulationUtility.cpp src/simulation/StatisticManager.cpp src/simulation/AbstractClient.cpp src/simulation/AbstractOperation.cpp src/client/ClientNew.cpp src/client/VIPClient.cpp src/client/Consultation.cpp src/client/Transfer.cpp src/client/Withdraw.cpp src/WindowsGUI.cpp -Iinclude -lgdi32 -luser32 -o simulation_gui_windows.exe


.\simulation_gui_windows.exe