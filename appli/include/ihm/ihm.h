#ifndef IHM_H
#define IHM_H

#include <string>
#include <vector>
#include <deque>
#include <memory>


#include "simulation/SimulationEntry.h"
#include "client/AbstractClient.h"
#include "bank/Cashier.h"
#include "bank/Bank.h"
#include "simulation/StatisticManager.h"
#include "bank/Queue.h"

// Structures adaptées pour IHM (sans modifier les classes)
struct ClientDataIHM {
    int id;  // Compteur interne (pas celui de AbstractClient)
    bool isVIP;
    std::string demande;
    int tempsArrivee;
    int tempsAttenteMax;
    
    ClientDataIHM(int clientId, bool vip, const std::string& type, int arrivee, int attenteMax)
        : id(clientId), isVIP(vip), demande(type), tempsArrivee(arrivee), tempsAttenteMax(attenteMax) {}
};

struct CaissierIHM {
    int id;  // Index basé sur la listes
    bool occupe;
    int tempsRestant;  // Estimation (pas accès direct)
    
    CaissierIHM(int caissId, bool occ, int rest) : id(caissId), occupe(occ), tempsRestant(rest) {}
};

// Classe IHM principale
class IHM {
private:
    SimulationEntry params;
    std::unique_ptr<Bank> bank;
    std::unique_ptr<StatisticManager> statisticManager;
    std::deque<std::shared_ptr<AbstractClient>> fileAttente;  // Copie interne
    int tempsActuel;
    int prochainClientId;
    bool simulationTermineeFlag;

    // Méthodes internes (dupliquées de Simulation pour éviter modifications)
    void genererNouveauClient();
    void traiterArriveeClients();
    void mettreAJourCaissiers();
    void assignerClientLibre();
    void retirerClientsImpatients();
    int genererTempsService();
    bool newClientArrival(int clientArrivalInterval, int simIter);

public:
    IHM(const SimulationEntry& entry);
    void afficher();  // Lance la GUI
    std::vector<ClientDataIHM> getFileAttenteIHM() const;
    std::vector<CaissierIHM> getCaissiersIHM() const;
    int getTempsActuel() const { return tempsActuel; }
    std::string getSimulationResults() const; 
    void displayGraphs(int totalServed, int totalDeparted, const std::map<int, double>& occupancyData);  // Nouvelle déclaration
};

#endif // IHM_Hs