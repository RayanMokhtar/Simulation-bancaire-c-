#include "include/WindowsGUI.h"
#include <windows.h>
#include <iostream>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance; (void)lpCmdLine; (void)nCmdShow;
    std::wcout << L"=== SIMULATION BANCAIRE - INTERFACE GRAPHIQUE WINDOWS ===" << std::endl;
    WindowsGUI gui;
    if (!gui.Initialize(hInstance)) {
        MessageBoxW(nullptr, L"Impossible d'initialiser l'interface graphique", L"Erreur", MB_OK | MB_ICONERROR);
        return 1;
    }
    gui.Run();
    return 0;
}
