#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <memory>
#include <cmath>
#include <SFML/Graphics.hpp>
// Ajouts pour gestion de chemins de polices multiplateforme
#include <vector>
#include <string>

#include "ihm/ihm.h"
#include "client/Client.h"
#include "client/VIPClient.h"
#include "simulation/SimulationUtility.h"
#include "client/Consultation.h"
#include "client/Transfer.h"
#include "client/Withdraw.h"

// Couleurs modernes (du code de base)
static const sf::Color COL_BG(250, 250, 252);
static const sf::Color COL_APPBAR(35, 39, 47);
static const sf::Color COL_PANEL(245, 247, 250);
static const sf::Color COL_BORDER(220, 225, 232);
static const sf::Color COL_TEXT(33, 37, 41);
static const sf::Color COL_TEXT_MUTED(108, 117, 125);
static const sf::Color COL_OK(46, 204, 113);
static const sf::Color COL_KO(231, 76, 60);
static const sf::Color COL_CONSULT(52, 152, 219);   // consultation
static const sf::Color COL_VIREMENT(155, 89, 182);  // virement (maintenant pour Transfer)
static const sf::Color COL_RETRAIT(26, 188, 156);   // retrait (maintenant pour Withdraw)
static const sf::Color COL_VIP(241, 196, 15);       // VIP

// Chargement robuste d'une police pour l'IHM (Windows, Linux, puis locaux)
static bool loadUiFont(sf::Font &font) {
#ifdef _WIN32
    // Chemins typiques des polices sous Windows (on récupère %WINDIR%)
    const char* envWindir = std::getenv("WINDIR");
    std::string win = envWindir ? envWindir : "C:\\Windows";
    const std::vector<std::string> windowsCandidates = {
        win + "\\Fonts\\segoeui.ttf",   // Segoe UI (par défaut Windows)
        win + "\\Fonts\\arial.ttf",     // Arial
        win + "\\Fonts\\tahoma.ttf",    // Tahoma
        win + "\\Fonts\\verdana.ttf",   // Verdana
        win + "\\Fonts\\calibri.ttf",   // Calibri
        win + "\\Fonts\\consola.ttf"    // Consolas
    };
    for (const auto& path : windowsCandidates) {
        if (font.loadFromFile(path)) {
            std::cout << "Police chargée: " << path << std::endl;
            return true;
        }
    }
#endif

    // Chemins typiques Linux
    const char* linuxCandidates[] = {
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",  // Liberation Sans
        "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf",                    // Ubuntu
        "/usr/share/fonts/truetype/freefont/FreeSans.ttf",
        "/usr/share/fonts/truetype/ttf-dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",                  // DejaVu Sans
    };
    for (auto path : linuxCandidates) {
        if (font.loadFromFile(path)) {
            std::cout << "Police chargée: " << path << std::endl;
            return true;
        }
    }

    // Fallbacks éventuels dans le projet (si vous ajoutez une police locale)
    const char* localCandidates[] = {
        "assets/fonts/DejaVuSans.ttf",
        "assets/DejaVuSans.ttf",
        "resources/fonts/DejaVuSans.ttf",
        "fonts/DejaVuSans.ttf"
    };
    for (auto path : localCandidates) {
        if (font.loadFromFile(path)) {
            std::cout << "Police chargée (locale): " << path << std::endl;
            return true;
        }
    }

    std::cout << "Aucune police trouvée. Le texte ne s'affichera pas." << std::endl;
    return false;  // Aucun chemin trouvé
}

// Crée un texte prêt à dessiner
static sf::Text makeText(const std::string& str, unsigned int size, const sf::Vector2f& pos, const sf::Font* font, sf::Color color = COL_TEXT) {
    sf::Text t;
    if (font) t.setFont(*font);
    t.setString(str);
    t.setCharacterSize(size);
    t.setFillColor(color);
    t.setPosition(pos);
    return t;
}

// Dessine une "carte" caissier avec statut et informations détaillées (accès direct)
static void drawCaissierCard(sf::RenderWindow& window, const sf::Font* font, const sf::Vector2f& pos, const std::unique_ptr<Cashier>& cashier, int id, int tempsActuel) {
    const sf::Vector2f size(180.f, 110.f);
    sf::RectangleShape card(size);
    card.setPosition(pos);
    card.setFillColor(sf::Color::White);
    card.setOutlineColor(COL_BORDER);
    card.setOutlineThickness(1.f);
    window.draw(card);

    // Pastille de statut
    sf::CircleShape dot(8.f);
    dot.setFillColor(cashier->isFree() ? COL_OK : COL_KO);
    dot.setPosition(pos.x + 12.f, pos.y + 12.f);
    window.draw(dot);

    // Titre + statut
    auto title = makeText("Caissier " + std::to_string(id), 16, {pos.x + 34.f, pos.y + 6.f}, font, COL_TEXT);
    window.draw(title);
    auto status = makeText(cashier->isFree() ? "Libre" : "busy", 14, {pos.x + 12.f, pos.y + 36.f}, font, cashier->isFree() ? COL_OK : COL_KO);
    window.draw(status);
    
    // Si occupé, afficher info client (direct comme Simulation)
    if (!cashier->isFree()) {
        auto client = cashier->getServingClient();
        std::string clientInfo = "Client #" + std::to_string(id * 10);  // Approximation ID
        window.draw(makeText(clientInfo, 12, {pos.x + 12.f, pos.y + 56.f}, font, COL_TEXT_MUTED));
        
        // Demande (type d'opération)
        std::string demande = client->getOperation().toString();
        window.draw(makeText("Demande: " + demande, 11, {pos.x + 12.f, pos.y + 72.f}, font, COL_TEXT_MUTED));
        
        // Temps restant (calcul direct comme dans Simulation)
        int totalServiceTime = client->getOperation().getServiceTime();
        int elapsed = tempsActuel - client->getServiceStartTime();
        int tempsRestant = std::max(0, totalServiceTime - elapsed);
        window.draw(makeText("Reste: " + std::to_string(tempsRestant) + "u", 11, {pos.x + 12.f, pos.y + 88.f}, font, COL_TEXT_MUTED));
    }
}

// Dessine une étoile (pour VIP)
static sf::ConvexShape makeStar(float cx, float cy, float rOuter, float rInner, int points = 5) {
    sf::ConvexShape star(points * 2);
    for (int i = 0; i < points * 2; ++i) {
        float angle = static_cast<float>(i) * 3.14159265f / points;
        float r = (i % 2 == 0) ? rOuter : rInner;
        float x = cx + r * std::cos(angle - 3.14159265f/2.f);
        float y = cy + r * std::sin(angle - 3.14159265f/2.f);
        star.setPoint(i, {x, y});
    }
    return star;
}

// Dessine une icône client avec ID et temps d'attente (accès direct)
static void drawClientIcon(sf::RenderWindow& window, const sf::Font* font, const sf::Vector2f& pos, const std::shared_ptr<AbstractClient>& client, int id, int tempsActuel) {
    sf::Color base = COL_CONSULT;
    std::string demande = client->getOperation().toString();
    // Conditions corrigées pour matcher les toString() réels
    if (demande == "Operation: Transfer") base = COL_VIREMENT;  // Violet pour Transfer
    else if (demande == "Operation: Withdraw") base = COL_RETRAIT;  // Vert pour Withdraw
    else base = COL_CONSULT;  // Bleu pour Consultation
    std::cout << "Demande: " << demande << " -> Couleur: " << base.r << "," << base.g << "," << base.b << std::endl;
    sf::CircleShape bubble(20.f);
    bubble.setFillColor(base);
    bubble.setPosition(pos);
    bubble.setOutlineThickness(2.f);
    bubble.setOutlineColor(sf::Color(255, 255, 255, 200));
    window.draw(bubble);

    if (client->isPriority()) {
        // Anneau VIP
        sf::CircleShape ring(22.f);
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineThickness(3.f);
        ring.setOutlineColor(COL_VIP);
        ring.setPosition(pos.x - 2.f, pos.y - 2.f);
        window.draw(ring);
        // Etoile VIP
        auto star = makeStar(pos.x + 20.f, pos.y + 8.f, 7.f, 3.f);
        star.setFillColor(COL_VIP);
        window.draw(star);
    }
    
    // Label client au-dessus (C{id} + VIP)
    std::string label = "C" + std::to_string(id);
    if (client->isPriority()) label += " VIP";
    auto labelText = makeText(label, 11, {pos.x - 6.f, pos.y - 16.f}, font, COL_TEXT);
    window.draw(labelText);
    
    // Temps d'attente sous le client
    int tempsAttente = tempsActuel - client->getArrivalTime();
    std::string waitStr = std::to_string(tempsAttente) + "u";
    auto waitText = makeText(waitStr, 10, {pos.x + 10.f, pos.y + 45.f}, font, COL_TEXT_MUTED);
    window.draw(waitText);
}

// Dessine la légende à droite
static void drawLegend(sf::RenderWindow& window, const sf::Font* font, const sf::Vector2f& topLeft, float width) {
    sf::RectangleShape panel({width, 540.f});
    panel.setPosition(topLeft);
    panel.setFillColor(COL_PANEL);
    panel.setOutlineColor(COL_BORDER);
    panel.setOutlineThickness(1.f);
    window.draw(panel);

    float x = topLeft.x + 20.f;
    float y = topLeft.y + 16.f;

    window.draw(makeText("Légende", 18, {x, y}, font, COL_TEXT));
    y += 30.f;

    // Caissier Libre / Occupé
    sf::RectangleShape sq({18.f, 18.f});
    sq.setPosition(x, y);
    sq.setFillColor(COL_OK); window.draw(sq);
    window.draw(makeText("Caissier libre", 14, {x + 28.f, y - 2.f}, font, COL_TEXT));
    y += 28.f;
    sq.setPosition(x, y);
    sq.setFillColor(COL_KO); window.draw(sq);
    window.draw(makeText("Caissier busy", 14, {x + 28.f, y - 2.f}, font, COL_TEXT));
    y += 36.f;

    window.draw(makeText("Types de demandes", 16, {x, y}, font, COL_TEXT_MUTED));
    y += 26.f;
    // Consultation
    sf::CircleShape c(9.f); c.setPosition(x, y); c.setFillColor(COL_CONSULT); window.draw(c);
    window.draw(makeText("Consultation", 14, {x + 28.f, y - 4.f}, font, COL_TEXT));
    y += 26.f;
    // Transfer (corrigé : pas Virement)
    c.setPosition(x, y); c.setFillColor(COL_VIREMENT); window.draw(c);
    window.draw(makeText("Transfer", 14, {x + 28.f, y - 4.f}, font, COL_TEXT));
    y += 26.f;
    // Withdraw (corrigé : pas Retrait)
    c.setPosition(x, y); c.setFillColor(COL_RETRAIT); window.draw(c);
    window.draw(makeText("Withdraw", 14, {x + 28.f, y - 4.f}, font, COL_TEXT));
    y += 32.f;

    window.draw(makeText("Client VIP", 16, {x, y}, font, COL_TEXT_MUTED));
    y += 28.f;
    // Icône VIP (cercle + étoile)
    sf::CircleShape vip(10.f); vip.setPosition(x, y); vip.setFillColor(COL_CONSULT); window.draw(vip);
    sf::CircleShape ring(12.f); ring.setPosition(x - 2.f, y - 2.f); ring.setFillColor(sf::Color::Transparent); ring.setOutlineThickness(2.f); ring.setOutlineColor(COL_VIP); window.draw(ring);
    auto star = makeStar(x + 10.f, y - 4.f, 5.5f, 2.5f); star.setFillColor(COL_VIP); window.draw(star);
    window.draw(makeText("VIP prioritaire", 14, {x + 28.f, y - 4.f}, font, COL_TEXT));
}

IHM::IHM(const SimulationEntry& entry) : params(entry), tempsActuel(0), prochainClientId(1), simulationTermineeFlag(false) {
    static bool seedInit = false;
    if (!seedInit) { 
        std::srand(static_cast<unsigned>(std::time(nullptr))); 
        seedInit = true; 
    }
    bank = std::make_unique<Bank>(params.getCashierCount());
    statisticManager = std::make_unique<StatisticManager>();
    // Pré-remplir la file
    for (int i = 0; i < 4; ++i) {
        genererNouveauClient();
    }
}

void IHM::genererNouveauClient() {
    bool isVIP = (std::rand() % 100) < (params.getPriorityClientRate() * 100);
    int typeRand = std::rand() % 3;
    int serviceTime = genererTempsService();
    
    std::unique_ptr<AbstractOperation> operation;
    if (typeRand == 0) {
        operation = std::make_unique<Consultation>(serviceTime);  // Consultation
    } else if (typeRand == 1) {
        operation = std::make_unique<Transfer>(serviceTime);     // Transfer (pas Virement)
    } else {
        operation = std::make_unique<Withdraw>(serviceTime);     // Withdraw (pas Retrait)
    }
    
    std::shared_ptr<AbstractClient> client = isVIP ?
        std::static_pointer_cast<AbstractClient>(std::make_shared<VIPClient>(tempsActuel, std::move(operation), params.getClientPatienceTime())) :
        std::static_pointer_cast<AbstractClient>(std::make_shared<Client>(tempsActuel, std::move(operation), params.getClientPatienceTime()));
    fileAttente.push_back(client);
}

void IHM::traiterArriveeClients() {
    if (tempsActuel % params.getClientArrivalInterval() == 0) {
        genererNouveauClient();
    }
}

void IHM::mettreAJourCaissiers() {
    const auto& cashiers = bank->getCashiers();
    for (size_t i = 0; i < cashiers.size(); ++i) {
        auto& cashier = cashiers[i];
        if (!cashier->isFree()) {
            statisticManager->cashierOccupationRecord();
        }
        cashier->work();
        if (cashier->serviceFinished()) {
            auto leavingClient = cashier->getServingClient();
            leavingClient->setDepartureTime(tempsActuel);
            statisticManager->registerServedClient(leavingClient);
            cashier->setServingClient(nullptr);
            if (!fileAttente.empty()) {
                auto nextClient = fileAttente.front();
                fileAttente.pop_front();
                nextClient->setServiceStartTime(tempsActuel);
                cashier->serve(nextClient);
            }
        }
    }
    // Règle d'impatience :
    // - Consultation (non urgente) : le client part s'il a attendu plus de N unités
    // - Transfer & Withdraw (urgentes) : ne partent pas par impatience
    const int patienceLimit = params.getClientPatienceTime();
    auto it = fileAttente.begin();
    while (it != fileAttente.end()) {
        const auto& client = *it;
        const auto& op = client->getOperation();
        const bool urgent = op.isUrgent();
        const int waited = tempsActuel - client->getArrivalTime();

        if (!urgent && waited > patienceLimit) {
            // Consultation trop longue -> le client quitte la file
            client->setDepartureTime(tempsActuel);
            statisticManager->registerNonServedClient(client);
            it = fileAttente.erase(it);
        } else {
            ++it;
        }
    }
}

void IHM::assignerClientLibre() {
    const auto& cashiers = bank->getCashiers();
    for (auto& cashier : cashiers) {
        if (cashier->isFree() && !fileAttente.empty()) {
            // Recherche du premier VIP dans la file (priorité, comme dans Simulation::updateBank)
            auto it = fileAttente.begin();
            for (; it != fileAttente.end(); ++it) {
                if ((*it)->isPriority()) {  // VIP trouvé
                    break;
                }
            }
            if (it == fileAttente.end()) {  // Aucun VIP, prendre le premier
                it = fileAttente.begin();
            }
            auto client = *it;
            fileAttente.erase(it);  // Retirer le client sélectionné
            client->setServiceStartTime(tempsActuel);
            cashier->serve(client);
        }
    }
}

int IHM::genererTempsService() {
    return params.getMinServiceTime() + 
           (std::rand() % (params.getMaxServiceTime() - params.getMinServiceTime() + 1));
}

bool IHM::newClientArrival(int clientArrivalInterval, int simIter) {
    return simIter % clientArrivalInterval == 0;
}

std::string IHM::getSimulationResults() const {
    std::string results = "########## Simulation results : #####################\n";
    results += "Simulation Duration : " + std::to_string(params.getSimulationDuration()) + "\n";
    results += "Served client count : " + std::to_string(statisticManager->servedClientCount()) + "\n";
    results += "Non-Served client count : " + std::to_string(statisticManager->nonServedClientCount()) + "\n";
    return results;
}

void IHM::afficher() {
    sf::RenderWindow window(sf::VideoMode(1200, 720), "Simulation Bank - IHM");
    window.setFramerateLimit(60);

    sf::Font uiFont;
    bool haveFont = loadUiFont(uiFont);
    const sf::Font* fontPtr = haveFont ? &uiFont : nullptr;
    
    bool paused = false;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) paused = !paused;
                if (event.key.code == sf::Keyboard::R) {
                    // Reset (recréer IHM)
                    tempsActuel = 0;
                    fileAttente.clear();
                    bank = std::make_unique<Bank>(params.getCashierCount());
                    statisticManager = std::make_unique<StatisticManager>();
                    for (int i = 0; i < 4; ++i) genererNouveauClient();
                }
            }
        }

        if (!paused && clock.getElapsedTime().asMilliseconds() > 500 && !simulationTermineeFlag) {
            traiterArriveeClients();
            mettreAJourCaissiers();
            assignerClientLibre();
            tempsActuel++;
            if (tempsActuel >= params.getSimulationDuration()) simulationTermineeFlag = true;
            clock.restart();
        }

        window.clear(COL_BG);

        // App bar
        sf::RectangleShape appbar({1200.f, 56.f});
        appbar.setPosition(0.f, 0.f);
        appbar.setFillColor(COL_APPBAR);
        window.draw(appbar);

        
        int totalClients = statisticManager->servedClientCount() + statisticManager->nonServedClientCount();
        double satisfactionRate = (totalClients > 0) ? (statisticManager->servedClientCount() * 100.0 / totalClients) : 0.0;
        std::string title = "Simulation Bank — Temps: " + std::to_string(tempsActuel) + 
                   " | Servis: " + std::to_string(statisticManager->servedClientCount()) + 
                   " | Partis: " + std::to_string(statisticManager->nonServedClientCount()) +
                   " | Satisfaction: " + std::to_string(satisfactionRate) + "%";
        window.draw(makeText(title, 18, {20.f, 18.f}, fontPtr, sf::Color(240, 240, 245)));
        
        std::string controls = paused ? "[PAUSE] Espace=Play R=Reset" : "[PLAY] Espace=Pause R=Reset";
        window.draw(makeText(controls, 14, {800.f, 20.f}, fontPtr, sf::Color(200, 200, 205)));

        // Zone caissiers (accès direct à bank->getCashiers())
        window.draw(makeText("Caissiers", 18, {20.f, 72.f}, fontPtr, COL_TEXT));
        const auto& cashiers = bank->getCashiers();
        for (size_t i = 0; i < cashiers.size(); ++i) {
            sf::Vector2f pos(20.f + static_cast<float>(i) * 200.f, 100.f);
            drawCaissierCard(window, fontPtr, pos, cashiers[i], static_cast<int>(i + 1), tempsActuel);
        }

        // Zone file (accès direct à fileAttente)
        sf::RectangleShape queuePanel({860.f, 380.f});
        queuePanel.setPosition(20.f, 230.f);
        queuePanel.setFillColor(sf::Color::White);
        queuePanel.setOutlineColor(COL_BORDER);
        queuePanel.setOutlineThickness(1.f);
        window.draw(queuePanel);

        std::string queueTitle = "File d'attente (" + std::to_string(fileAttente.size()) + " clients)";
        window.draw(makeText(queueTitle, 16, {36.f, 240.f}, fontPtr, COL_TEXT_MUTED));

        float startX = 40.f, startY = 280.f;
        float x = startX, y = startY;
        int perRow = 12;
        int idx = 0;
        if (fileAttente.empty()) {
            window.draw(makeText("(aucun client pour le moment)", 14, {36.f, 310.f}, fontPtr, COL_TEXT_MUTED));
        } else {
            int idCounter = 1;
            for (const auto& client : fileAttente) {
                drawClientIcon(window, fontPtr, sf::Vector2f(x, y), client, idCounter++, tempsActuel);
                x += 70.f;
                idx++;
                if (idx % perRow == 0) { x = startX; y += 80.f; }
            }
        }

        // Légende à droite
        drawLegend(window, fontPtr, {900.f, 100.f}, 280.f);

        if (simulationTermineeFlag) {
            sf::RectangleShape endPanel({400.f, 100.f});
            endPanel.setPosition(400.f, 300.f);
            endPanel.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(endPanel);
            
            window.draw(makeText("SIMULATION TERMINÉE", 20, {450.f, 320.f}, fontPtr, sf::Color::White));
            window.draw(makeText("Appuyez sur R pour recommencer", 14, {430.f, 350.f}, fontPtr, sf::Color(200, 200, 200)));
        }

        window.display();
    }
}





void drawPieSector(sf::RenderWindow& window, sf::Vector2f center, float radius, float startAngle, float endAngle, sf::Color color) {
    sf::ConvexShape sector;
    sector.setPointCount(32);  // Plus de points pour lisser
    sector.setFillColor(color);
    sector.setPosition(center);

    float angleStep = (endAngle - startAngle) / 30.f;
    sector.setPoint(0, sf::Vector2f(0.f, 0.f));  // Centre
    for (int i = 1; i <= 31; ++i) {
        float angle = startAngle + (i - 1) * angleStep;
        float rad = angle * 3.14159265f / 180.f;
        sector.setPoint(i, sf::Vector2f(radius * std::cos(rad), radius * std::sin(rad)));
    }
    window.draw(sector);
}

// Fonction pour dessiner une barre avec gradient
void drawGradientBar(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size, sf::Color topColor, sf::Color bottomColor) {
    sf::RectangleShape bar(size);
    bar.setPosition(position);
    // Gradient simple : couleur moyenne
    sf::Color midColor((topColor.r + bottomColor.r)/2, (topColor.g + bottomColor.g)/2, (topColor.b + bottomColor.b)/2);
    bar.setFillColor(midColor);
    window.draw(bar);

    // Bordure
    bar.setFillColor(sf::Color::Transparent);
    bar.setOutlineThickness(2.f);
    bar.setOutlineColor(sf::Color::Black);
    window.draw(bar);
}

void IHM::displayGraphs(int totalServed, int totalDeparted, const std::map<int, double>& occupancyData) {
    sf::RenderWindow window(sf::VideoMode(1000, 700), "Graphiques DB Améliorés");
    sf::Font font;
    bool haveFont = loadUiFont(font);
    const sf::Font* fontPtr = haveFont ? &font : nullptr;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear(sf::Color(240, 248, 255));  // Fond bleu clair

        // Titre
        if (fontPtr) {
            sf::Text title("Statistiques de Simulation", *fontPtr, 24);
            title.setPosition(350.f, 20.f);
            title.setFillColor(sf::Color::Black);
            window.draw(title);
        }

        // Camembert (à gauche)
        sf::Vector2f pieCenter(200.f, 200.f);
        float pieRadius = 120.f;
        if (totalServed + totalDeparted > 0) {
            double servedPercent = (double)totalServed / (totalServed + totalDeparted) * 100;
            double departedPercent = 100 - servedPercent;

            // Secteur servis (vert)
            drawPieSector(window, pieCenter, pieRadius, 0.f, servedPercent * 3.6f, sf::Color(34, 139, 34));  // Vert foncé

            // Secteur partis (rouge)
            drawPieSector(window, pieCenter, pieRadius, servedPercent * 3.6f, 360.f, sf::Color(220, 20, 60));  // Rouge

            // Légende camembert
            if (fontPtr) {
                sf::Text legend1("Servis: " + std::to_string(servedPercent).substr(0, 4) + "%", *fontPtr, 16);
                legend1.setPosition(50.f, 380.f);
                legend1.setFillColor(sf::Color(34, 139, 34));
                window.draw(legend1);

                sf::Text legend2("Partis: " + std::to_string(departedPercent).substr(0, 4) + "%", *fontPtr, 16);
                legend2.setPosition(50.f, 410.f);
                legend2.setFillColor(sf::Color(220, 20, 60));
                window.draw(legend2);
            }
        }

        // Barres (à droite) avec axes et grille
        float chartX = 500.f;
        float chartY = 100.f;
        float chartWidth = 400.f;
        float chartHeight = 400.f;

        // Axe Y (vertical)
        sf::RectangleShape yAxis(sf::Vector2f(2.f, chartHeight));
        yAxis.setPosition(chartX, chartY);
        yAxis.setFillColor(sf::Color::Black);
        window.draw(yAxis);

        // Axe X (horizontal)
        sf::RectangleShape xAxis(sf::Vector2f(chartWidth, 2.f));
        xAxis.setPosition(chartX, chartY + chartHeight);
        xAxis.setFillColor(sf::Color::Black);
        window.draw(xAxis);

        // Grille légère
        for (int i = 0; i <= 10; ++i) {
            float y = chartY + (chartHeight / 10) * i;
            sf::RectangleShape gridLine(sf::Vector2f(chartWidth, 1.f));
            gridLine.setPosition(chartX, y);
            gridLine.setFillColor(sf::Color(200, 200, 200, 100));
            window.draw(gridLine);
        }

        // Titre barres
        if (fontPtr) {
            sf::Text barTitle("Occupation par Caissier (%)", *fontPtr, 18);
            barTitle.setPosition(chartX + 50.f, chartY - 40.f);
            barTitle.setFillColor(sf::Color::Black);
            window.draw(barTitle);
        }

        // Barres
        int barCount = occupancyData.size();
        float barWidth = chartWidth / (barCount * 2.f);
        float spacing = barWidth;
        int index = 0;
        for (auto& data : occupancyData) {
            float barHeight = (data.second / 100.f) * chartHeight;  // Échelle 0-100%
            sf::Vector2f barPos(chartX + spacing + index * (barWidth + spacing), chartY + chartHeight - barHeight);
            sf::Vector2f barSize(barWidth, barHeight);

            // Gradient : bleu clair en haut, bleu foncé en bas
            drawGradientBar(window, barPos, barSize, sf::Color(135, 206, 250), sf::Color(25, 25, 112));

            // Label caissier
            if (fontPtr) {
                sf::Text label(std::to_string(data.first), *fontPtr, 12);
                label.setPosition(barPos.x + barWidth / 4.f, chartY + chartHeight + 10.f);
                label.setFillColor(sf::Color::Black);
                window.draw(label);

                // Valeur sur la barre
                sf::Text value(std::to_string(data.second).substr(0, 4) + "%", *fontPtr, 10);
                value.setPosition(barPos.x, barPos.y - 20.f);
                value.setFillColor(sf::Color::White);
                window.draw(value);
            }
            index++;
        }

        // Labels axes
        if (fontPtr) {
            sf::Text xLabel("Caissiers", *fontPtr, 14);
            xLabel.setPosition(chartX + chartWidth / 2.f - 30.f, chartY + chartHeight + 30.f);
            xLabel.setFillColor(sf::Color::Black);
            window.draw(xLabel);

            sf::Text yLabel("Occupation (%)", *fontPtr, 14);
            yLabel.setPosition(chartX - 100.f, chartY + chartHeight / 2.f - 50.f);
            yLabel.setRotation(-90.f);
            yLabel.setFillColor(sf::Color::Black);
            window.draw(yLabel);
        }

        window.display();
    }
}