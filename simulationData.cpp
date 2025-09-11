#include "SimulationData.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

// Instance globale de simulation
static SimulationBank simulation;

SimulationBank::SimulationBank(const ParametresSimulation& p) : params(p), tempsActuel(0), prochainClientId(1) {
    initialiser();
}

void SimulationBank::initialiser() {
    static bool seedInit = false;
    if (!seedInit) { 
        std::srand(static_cast<unsigned>(std::time(nullptr))); 
        seedInit = true; 
    }
    
    // Initialiser les caissiers
    caissiers.clear();
    for (int i = 0; i < params.nbCaissiers; i++) {
        caissiers.emplace_back(i + 1);
    }
    
    // Vider la file
    fileAttente.clear();
    tempsActuel = 0;
    prochainClientId = 1;
    
    // Reset statistiques
    clientsServis = clientsPartis = clientsVIPServis = 0;

    // Pré-remplir la file pour visualiser immédiatement
    for (int i = 0; i < params.clientsInitial; ++i) {
        genererNouveauClient();
    }
}

void SimulationBank::executerPasSimulation() {
    if (simulationTerminee()) return;
    
    // 1. Traiter arrivées de nouveaux clients
    traiterArriveeClients();
    
    // 2. Mettre à jour l'état des caissiers
    mettreAJourCaissiers();
    
    // 3. Assigner clients en attente aux caissiers libres
    assignerClientLibre();
    
    // 4. Retirer clients consultation impatients
    retirerClientsImpatients();
    
    // 5. Avancer le temps
    tempsActuel++;
}

bool SimulationBank::simulationTerminee() const {
    return tempsActuel >= params.dureeSimulation;
}

void SimulationBank::resetSimulation() {
    initialiser();
}

void SimulationBank::traiterArriveeClients() {
    // Nouveau client arrive tous les intervalleArrivee unités de temps
    if (tempsActuel % params.intervalleArrivee == 0) {
        genererNouveauClient();
    }
}

void SimulationBank::genererNouveauClient() {
    // Déterminer si VIP
    bool isVIP = (std::rand() % 100) < params.tauxVIP;
    
    // Déterminer type de demande (1/3 chance pour chaque)
    int typeRand = std::rand() % 3;
    std::string demande;
    if (typeRand == 0) demande = "consultation";
    else if (typeRand == 1) demande = "virement";  
    else demande = "retrait";
    
    ClientData nouveau(prochainClientId++, isVIP, demande, tempsActuel);
    if (demande == "consultation") {
        nouveau.tempsAttenteMax = params.tempsAttenteMaxConsult;
    }
    
    fileAttente.push_back(nouveau);
}

void SimulationBank::mettreAJourCaissiers() {
    for (auto& caissier : caissiers) {
        if (caissier.occupe && tempsActuel >= caissier.tempsFinService) {
            // Service terminé
            if (caissier.clientActuel) {
                clientsServis++;
                if (caissier.clientActuel->isVIP) clientsVIPServis++;
                delete caissier.clientActuel;
                caissier.clientActuel = nullptr;
            }
            caissier.occupe = false;
        }
    }
}

void SimulationBank::assignerClientLibre() {
    for (auto& caissier : caissiers) {
        if (!caissier.occupe && !fileAttente.empty()) {
            ClientData* client = nullptr;
            
            // Chercher d'abord un client VIP prioritaire
            ClientData* vip = trouverProchainVIP();
            if (vip) {
                client = vip;
                // Retirer le VIP de la file
                fileAttente.erase(std::find_if(fileAttente.begin(), fileAttente.end(), 
                    [vip](const ClientData& c) { return c.id == vip->id; }));
            } else {
                // Prendre le premier client normal
                client = new ClientData(fileAttente.front());
                fileAttente.pop_front();
            }
            
            // Assigner le client au caissier
            caissier.occupe = true;
            caissier.clientActuel = client;
            int tempsService = genererTempsService();
            caissier.tempsFinService = tempsActuel + tempsService;
        }
    }
}

void SimulationBank::retirerClientsImpatients() {
    auto it = fileAttente.begin();
    while (it != fileAttente.end()) {
        if (it->demande == "consultation" && 
            (tempsActuel - it->tempsArrivee) > it->tempsAttenteMax) {
            clientsPartis++;
            it = fileAttente.erase(it);
        } else {
            ++it;
        }
    }
}

int SimulationBank::genererTempsService() {
    return params.tempsServiceMin + 
           (std::rand() % (params.tempsServiceMax - params.tempsServiceMin + 1));
}

ClientData* SimulationBank::trouverProchainVIP() {
    for (auto& client : fileAttente) {
        if (client.isVIP) {
            return new ClientData(client);
        }
    }
    return nullptr;
}

// Fonctions de compatibilité pour l'interface existante
std::vector<ClientData> getFileAttente() {
    const auto& file = simulation.getFileAttente();
    return std::vector<ClientData>(file.begin(), file.end());
}

int getNbCaissiersLibres() {
    const auto& caissiers = simulation.getCaissiers();
    int libres = 0;
    for (const auto& c : caissiers) {
        if (!c.occupe) libres++;
    }
    return libres;
}

SimulationBank& getSimulation() {
    return simulation;
}
