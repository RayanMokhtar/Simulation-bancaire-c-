#include <SFML/Graphics.hpp>
#include "SimulationData.hpp"
#include <cmath>
#include <string>

// Couleurs modernes
static const sf::Color COL_BG(250, 250, 252);
static const sf::Color COL_APPBAR(35, 39, 47);
static const sf::Color COL_PANEL(245, 247, 250);
static const sf::Color COL_BORDER(220, 225, 232);
static const sf::Color COL_TEXT(33, 37, 41);
static const sf::Color COL_TEXT_MUTED(108, 117, 125);
static const sf::Color COL_OK(46, 204, 113);
static const sf::Color COL_KO(231, 76, 60);
static const sf::Color COL_CONSULT(52, 152, 219);   // consultation
static const sf::Color COL_VIREMENT(155, 89, 182);  // virement
static const sf::Color COL_RETRAIT(26, 188, 156);   // retrait
static const sf::Color COL_VIP(241, 196, 15);       // VIP

// Essaie de charger une police système Windows connue (Segoe UI ou Arial)
static bool loadUiFont(sf::Font &font) {
    const char* candidates[] = {
        "C:\\Windows\\Fonts\\segoeui.ttf",
        "C:\\Windows\\Fonts\\arial.ttf",
        "C:\\Windows\\Fonts\\tahoma.ttf"
    };
    for (auto path : candidates) {
        if (font.loadFromFile(path)) return true;
    }
    return false; // On dessinera quand même sans texte si aucune police n'est trouvée
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

// Dessine une "carte" caissier avec statut et informations détaillées
static void drawCaissierCard(sf::RenderWindow& window, const sf::Font* font, const sf::Vector2f& pos, const Caissier& caissier, int tempsActuel) {
    const sf::Vector2f size(180.f, 110.f);
    sf::RectangleShape card(size);
    card.setPosition(pos);
    card.setFillColor(sf::Color::White);
    card.setOutlineColor(COL_BORDER);
    card.setOutlineThickness(1.f);
    window.draw(card);

    // Pastille de statut
    sf::CircleShape dot(8.f);
    dot.setFillColor(caissier.occupe ? COL_KO : COL_OK);
    dot.setPosition(pos.x + 12.f, pos.y + 12.f);
    window.draw(dot);

    // Titre + statut
    auto title = makeText("Caissier " + std::to_string(caissier.id), 16, {pos.x + 34.f, pos.y + 6.f}, font, COL_TEXT);
    window.draw(title);
    auto status = makeText(caissier.occupe ? "Occupé" : "Libre", 14, {pos.x + 12.f, pos.y + 36.f}, font, caissier.occupe ? COL_KO : COL_OK);
    window.draw(status);
    
    // Si occupé, afficher info client
    if (caissier.occupe && caissier.clientActuel) {
        std::string clientInfo = caissier.clientActuel->isVIP ? "VIP" : "Client";
        clientInfo += " #" + std::to_string(caissier.clientActuel->id);
        window.draw(makeText(clientInfo, 12, {pos.x + 12.f, pos.y + 56.f}, font, COL_TEXT_MUTED));
        
        std::string demande = caissier.clientActuel->demande;
        window.draw(makeText(demande, 11, {pos.x + 12.f, pos.y + 72.f}, font, COL_TEXT_MUTED));
        
        int tempsRestant = std::max(0, caissier.tempsFinService - tempsActuel);
        window.draw(makeText("Reste: " + std::to_string(tempsRestant) + "u", 11, {pos.x + 12.f, pos.y + 88.f}, font, COL_TEXT_MUTED));
    }
}

// Dessine une étoile (pour VIP)
static sf::ConvexShape makeStar(float cx, float cy, float rOuter, float rInner, int points = 5) {
    sf::ConvexShape star(points * 2);
    for (int i = 0; i < points * 2; ++i) {
        float angle = static_cast<float>(i) * 3.14159265f / points; // pi/points
        float r = (i % 2 == 0) ? rOuter : rInner;
        float x = cx + r * std::cos(angle - 3.14159265f/2.f);
        float y = cy + r * std::sin(angle - 3.14159265f/2.f);
        star.setPoint(i, {x, y});
    }
    return star;
}

// Dessine une icône client avec ID et temps d'attente
static void drawClientIcon(sf::RenderWindow& window, const sf::Font* font, const sf::Vector2f& pos, const ClientData& client, int tempsActuel) {
    sf::Color base = COL_CONSULT;
    if (client.demande == "virement") base = COL_VIREMENT;
    else if (client.demande == "retrait") base = COL_RETRAIT;
    else base = COL_CONSULT;

    sf::CircleShape bubble(20.f);
    bubble.setFillColor(base);
    bubble.setPosition(pos);
    bubble.setOutlineThickness(2.f);
    bubble.setOutlineColor(sf::Color(255, 255, 255, 200));
    window.draw(bubble);

    if (client.isVIP) {
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
    std::string label = "C" + std::to_string(client.id);
    if (client.isVIP) label += " VIP";
    auto labelText = makeText(label, 11, {pos.x - 6.f, pos.y - 16.f}, font, COL_TEXT);
    window.draw(labelText);
    
    // Temps d'attente sous le client
    int tempsAttente = tempsActuel - client.tempsArrivee;
    std::string waitStr = std::to_string(tempsAttente) + "u";
    auto waitText = makeText(waitStr, 10, {pos.x + 10.f, pos.y + 45.f}, font, COL_TEXT_MUTED);
    window.draw(waitText);
}

// Dessine la légende à droite
static void drawLegend(sf::RenderWindow& window, const sf::Font* font, const sf::Vector2f& topLeft, float width) {
    // Panneau
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
    window.draw(makeText("Caissier occupé", 14, {x + 28.f, y - 2.f}, font, COL_TEXT));
    y += 36.f;

    window.draw(makeText("Types de demandes", 16, {x, y}, font, COL_TEXT_MUTED));
    y += 26.f;
    // Consultation
    sf::CircleShape c(9.f); c.setPosition(x, y); c.setFillColor(COL_CONSULT); window.draw(c);
    window.draw(makeText("Consultation", 14, {x + 28.f, y - 4.f}, font, COL_TEXT));
    y += 26.f;
    // Virement
    c.setPosition(x, y); c.setFillColor(COL_VIREMENT); window.draw(c);
    window.draw(makeText("Virement", 14, {x + 28.f, y - 4.f}, font, COL_TEXT));
    y += 26.f;
    // Retrait
    c.setPosition(x, y); c.setFillColor(COL_RETRAIT); window.draw(c);
    window.draw(makeText("Retrait", 14, {x + 28.f, y - 4.f}, font, COL_TEXT));
    y += 32.f;

    window.draw(makeText("Client VIP", 16, {x, y}, font, COL_TEXT_MUTED));
    y += 28.f;
    // Icône VIP (cercle + étoile)
    sf::CircleShape vip(10.f); vip.setPosition(x, y); vip.setFillColor(COL_CONSULT); window.draw(vip);
    sf::CircleShape ring(12.f); ring.setPosition(x - 2.f, y - 2.f); ring.setFillColor(sf::Color::Transparent); ring.setOutlineThickness(2.f); ring.setOutlineColor(COL_VIP); window.draw(ring);
    auto star = makeStar(x + 10.f, y - 4.f, 5.5f, 2.5f); star.setFillColor(COL_VIP); window.draw(star);
    window.draw(makeText("VIP prioritaire", 14, {x + 28.f, y - 4.f}, font, COL_TEXT));
}

int main() {
    // Fenêtre plus large pour l'IHM
    sf::RenderWindow window(sf::VideoMode(1200, 720), "Simulation Bank");
    window.setFramerateLimit(60);

    sf::Font uiFont; bool haveFont = loadUiFont(uiFont);
    const sf::Font* fontPtr = haveFont ? &uiFont : nullptr;
    
    // Obtenir la référence à la simulation
    SimulationBank& sim = getSimulation();
    
    // Variables de contrôle
    bool paused = false;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    paused = !paused; // Pause/Play avec Espace
                }
                if (event.key.code == sf::Keyboard::R) {
                    sim.resetSimulation(); // Reset avec R
                }
            }
        }

        // Avancer la simulation toutes les 0.5 secondes si pas en pause
        if (!paused && clock.getElapsedTime().asMilliseconds() > 500 && !sim.simulationTerminee()) {
            sim.executerPasSimulation();
            clock.restart();
        }

        // Récupération des données de simulation
        const auto& caissiers = sim.getCaissiers();
        const auto& file = sim.getFileAttente();
        int tempsActuel = sim.getTempsActuel();

        // Fond
        window.clear(COL_BG);

        // App bar (titre + infos simulation)
        sf::RectangleShape appbar({1200.f, 56.f});
        appbar.setPosition(0.f, 0.f);
        appbar.setFillColor(COL_APPBAR);
        window.draw(appbar);
        
        std::string title = "Simulation Bank — Temps: " + std::to_string(tempsActuel) + 
                           " | Servis: " + std::to_string(sim.getClientsServis()) + 
                           " | Partis: " + std::to_string(sim.getClientsPartis());
        window.draw(makeText(title, 18, {20.f, 18.f}, fontPtr, sf::Color(240, 240, 245)));
        
        std::string controls = paused ? "[PAUSE] Espace=Play R=Reset" : "[PLAY] Espace=Pause R=Reset";
        window.draw(makeText(controls, 14, {800.f, 20.f}, fontPtr, sf::Color(200, 200, 205)));

        // Zone caissiers
        window.draw(makeText("Caissiers", 18, {20.f, 72.f}, fontPtr, COL_TEXT));

        for (size_t i = 0; i < caissiers.size(); i++) {
            sf::Vector2f pos(20.f + static_cast<float>(i) * 200.f, 100.f);
            drawCaissierCard(window, fontPtr, pos, caissiers[i], tempsActuel);
        }

        // Zone file d'attente
        sf::RectangleShape queuePanel({860.f, 380.f});
        queuePanel.setPosition(20.f, 230.f);
        queuePanel.setFillColor(sf::Color::White);
        queuePanel.setOutlineColor(COL_BORDER);
        queuePanel.setOutlineThickness(1.f);
        window.draw(queuePanel);

        std::string queueTitle = "File d'attente (" + std::to_string(file.size()) + " clients)";
        window.draw(makeText(queueTitle, 16, {36.f, 240.f}, fontPtr, COL_TEXT_MUTED));

        // Dessiner les clients dans la file (plusieurs rangées si nécessaire)
        float startX = 40.f, startY = 280.f;
        float x = startX, y = startY;
        int perRow = 12;
        int idx = 0;
        if (file.empty()) {
            window.draw(makeText("(aucun client pour le moment)", 14, {36.f, 310.f}, fontPtr, COL_TEXT_MUTED));
        } else {
            for (const auto& client : file) {
                drawClientIcon(window, fontPtr, sf::Vector2f(x, y), client, tempsActuel);
                x += 70.f;
                idx++;
                if (idx % perRow == 0) { x = startX; y += 80.f; }
            }
        }

    // Légende à droite
    drawLegend(window, fontPtr, {900.f, 100.f}, 280.f);

        // Message de fin si simulation terminée
        if (sim.simulationTerminee()) {
            sf::RectangleShape endPanel({400.f, 100.f});
            endPanel.setPosition(400.f, 300.f);
            endPanel.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(endPanel);
            
            window.draw(makeText("SIMULATION TERMINÉE", 20, {450.f, 320.f}, fontPtr, sf::Color::White));
            window.draw(makeText("Appuyez sur R pour recommencer", 14, {430.f, 350.f}, fontPtr, sf::Color(200, 200, 200)));
        }

        window.display();
    }

    return 0;
}
