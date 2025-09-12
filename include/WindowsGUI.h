#ifndef WINDOWS_GUI_H
#define WINDOWS_GUI_H

#include <windows.h>
#include <string>
#include <memory>
#include "simulation/SimulationNew.h"
#include "simulation/SimulationEntry.h"

#ifdef WITH_PERSISTENCE
#include "persistence/PersistenceManager.h"
#endif

class WindowsGUI {
private:
    HWND hwnd;
    HWND hStart, hPause, hReset, hStatus;
    HWND hDuration, hCashiers, hInterval, hMinService, hMaxService, hVipRate, hPatience;
    HWND hResults;
    
    std::unique_ptr<SimulationNew> simulation;
    std::unique_ptr<SimulationEntry> currentEntry;
    
    bool isRunning;
    int currentTime;
    int maxTime;

#ifdef WITH_PERSISTENCE
    std::unique_ptr<PersistenceManager> persistenceManager;
    bool persistenceEnabled;
    int persistenceConfigId; // ID de configuration sauvegardée
    bool hasSavedSimulation; // éviter double sauvegarde
    HWND hSaveConfig, hLoadConfig, hShowHistory;
#endif

public:
    WindowsGUI();
    ~WindowsGUI();
    
    bool Initialize(HINSTANCE hInstance);
    void Run();
    void UpdateDisplay();
    void StartSimulation();
    void PauseSimulation();
    void ResetSimulation();
    void RunStep();
    
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
};

#endif
