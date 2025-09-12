#include <iostream>
#include <windows.h>
#include "WindowsGUI.h"

#ifdef WITH_PERSISTENCE
#include "persistence/DatabaseConnection.h"
#include "persistence/PersistenceManager.h"
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    try {
        std::cout << "=== DEBUT DEBUG ===" << std::endl;
        
        // Test 1: Allocation console pour debug
        AllocConsole();
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
        
        std::cout << "Console allouee avec succes" << std::endl;
        
#ifdef WITH_PERSISTENCE
        std::cout << "Compilation avec support MySQL detecte" << std::endl;
        
        // Test 2: Test de connexion MySQL
        try {
            std::cout << "Tentative de connexion a la base de donnees..." << std::endl;
            DatabaseConnection& db = DatabaseConnection::getInstance();
            std::cout << "Instance DatabaseConnection obtenue" << std::endl;
            
            db.connect();
            std::cout << "Connexion MySQL reussie !" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "ERREUR MySQL: " << e.what() << std::endl;
            std::cout << "Continuons sans MySQL..." << std::endl;
        }
#else
        std::cout << "Compilation sans support MySQL" << std::endl;
#endif
        
        // Test 3: Initialisation GUI
        std::cout << "Creation de l'interface GUI..." << std::endl;
        WindowsGUI gui;
        std::cout << "Interface GUI creee" << std::endl;
        
        std::cout << "Initialisation de l'interface..." << std::endl;
        if (!gui.Initialize(hInstance)) {
            std::cout << "ERREUR: Echec de l'initialisation GUI" << std::endl;
            system("pause");
            return -1;
        }
        std::cout << "Interface GUI initialisee avec succes" << std::endl;
        
        std::cout << "Lancement de la boucle principale..." << std::endl;
        gui.Run();
        std::cout << "Application terminee normalement" << std::endl;
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cout << "EXCEPTION GENERALE: " << e.what() << std::endl;
        system("pause");
        return -1;
    } catch (...) {
        std::cout << "EXCEPTION INCONNUE" << std::endl;
        system("pause");
        return -1;
    }
}
