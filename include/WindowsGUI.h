#ifndef WINDOWS_GUI_H
#define WINDOWS_GUI_H

#include <windows.h>
#include <string>
#include <memory>
#include "../include/SimulationNew.h"
#include "../include/SimulationEntry.h"

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
