#include <iostream>
#include <windows.h>

// Version très basique pour tester
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Allocation console pour debug
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
    
    std::cout << "=== TEST BASIC ===" << std::endl;
    std::cout << "Application demarree" << std::endl;
    
#ifdef WITH_PERSISTENCE
    std::cout << "WITH_PERSISTENCE est defini" << std::endl;
#else
    std::cout << "WITH_PERSISTENCE n'est PAS defini" << std::endl;
#endif
    
    std::cout << "Appuyez sur une touche pour continuer..." << std::endl;
    system("pause");
    
    return 0;
}
