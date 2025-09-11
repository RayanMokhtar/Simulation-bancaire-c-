#ifndef SIMULATION_DATA_HPP
#define SIMULATION_DATA_HPP

#include <string>
#include <vector>
#include <queue>

// Structure représentant un client
struct ClientData {
    int id;                    // Identifiant unique
    bool isVIP;               // Client VIP ou non
    std::string demande;      // "consultation", "virement", "retrait"
    int tempsArrivee;        // Moment d'arrivée dans la simulation
    int tempsAttenteMax;     // Temps max d'attente (pour consultation)
    
    ClientData(int clientId = 0, bool vip = false, const std::string& type = "consultation", int arrivee = 0)
        : id(clientId), isVIP(vip), demande(type), tempsArrivee(arrivee), tempsAttenteMax(0) {
        // Clients consultation : maximum N unités d'attente
        if (demande == "consultation") tempsAttenteMax = 10; // N=10 par défaut
    }
};

// Structure représentant un caissier
struct Caissier {
    int id;                   // Identifiant
    bool occupe;             // Libre ou occupé
    ClientData* clientActuel; // Client en cours de traitement
    int tempsFinService;     // Moment où le service se termine
    
    Caissier(int caissId) : id(caissId), occupe(false), clientActuel(nullptr), tempsFinService(0) {}
};

// Paramètres de simulation
struct ParametresSimulation {
    int nbCaissiers = 3;             // Nombre de caissiers
    int intervalleArrivee = 8;       // Arrivée client toutes les N unités (plus lent pour observer)
    int tempsServiceMin = 12;        // Temps service minimum (plus long pour voir la file)
    int tempsServiceMax = 20;        // Temps service maximum  
    int tauxVIP = 30;                // Pourcentage de clients VIP (un peu plus pour visualiser la priorité)
    int dureeSimulation = 300;       // Durée totale simulation
    int tempsAttenteMaxConsult = 10; // Temps max attente consultation
    int clientsInitial = 4;          // Nombre de clients présents dès le début
};

// Classe principale de simulation
class SimulationBank {
private:
    std::vector<Caissier> caissiers;
    std::deque<ClientData> fileAttente;
    int tempsActuel;
    int prochainClientId;
    ParametresSimulation params;
    
    // Statistiques
    int clientsServis = 0;
    int clientsPartis = 0;
    int clientsVIPServis = 0;
    
public:
    SimulationBank(const ParametresSimulation& p = ParametresSimulation());
    
    // Méthodes principales
    void initialiser();
    void executerPasSimulation();
    bool simulationTerminee() const;
    void resetSimulation();
    void setParametres(const ParametresSimulation& p) { params = p; initialiser(); }
    
    // Getters pour l'affichage
    const std::vector<Caissier>& getCaissiers() const { return caissiers; }
    const std::deque<ClientData>& getFileAttente() const { return fileAttente; }
    int getTempsActuel() const { return tempsActuel; }
    int getClientsServis() const { return clientsServis; }
    int getClientsPartis() const { return clientsPartis; }
    int getClientsVIPServis() const { return clientsVIPServis; }
    const ParametresSimulation& getParametres() const { return params; }
    
private:
    // Méthodes internes
    void genererNouveauClient();
    void traiterArriveeClients();
    void mettreAJourCaissiers();
    void assignerClientLibre();
    void retirerClientsImpatients();
    int genererTempsService();
    ClientData* trouverProchainVIP();
};

// Fonctions d'accès pour l'interface (compatibilité)
std::vector<ClientData> getFileAttente();
int getNbCaissiersLibres();
SimulationBank& getSimulation(); // Accès à l'instance globale

#endif
