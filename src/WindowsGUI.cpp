#include "WindowsGUI.h"
#include <sstream>
#include <iomanip>
#include <cctype>
#include <string>
#include <cwchar>
#include <cstdlib>

#ifdef WITH_PERSISTENCE
#include "persistence/DatabaseConnection.h"
#include "persistence/SimulationRepository.h"
#include "persistence/DatabaseConfig.h"
#endif

#define ID_START 1001
#define ID_PAUSE 1002
#define ID_RESET 1003
#define ID_TIMER 1004

#define ID_DURATION 2001
#define ID_CASHIERS 2002
#define ID_INTERVAL 2003
#define ID_MINSERVICE 2004
#define ID_MAXSERVICE 2005
#define ID_VIPRATE 2006
#define ID_PATIENCE 2007

// --- Prototypes utilitaires wide ---
static std::wstring FormatFinalResultsFR_Direct(SimulationNew* sim);

WindowsGUI::WindowsGUI() : hwnd(nullptr), simulation(nullptr), currentEntry(nullptr), 
                           isRunning(false), currentTime  (0), maxTime(100)
#ifdef WITH_PERSISTENCE
                           , persistenceEnabled(false), persistenceConfigId(-1), hasSavedSimulation(false)
#endif
{
}

WindowsGUI::~WindowsGUI() {
}

bool WindowsGUI::Initialize(HINSTANCE hInstance) {
    const wchar_t* className = L"BankSimulationWindow";
    // Enregistrer la classe de fenetre (version wide)
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    if (!RegisterClassW(&wc)) {
        return false;
    }
    // Creer la fenetre principale
    hwnd = CreateWindowExW(
        0,
        className,
        L"Simulation Bancaire - Interface Graphique",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInstance, this
    );
    if (!hwnd) {
        return false;
    }
    // Parametres
    CreateWindowW(L"STATIC", L"Duree:", WS_VISIBLE | WS_CHILD,
                 10, 10, 80, 20, hwnd, nullptr, hInstance, nullptr);
    hDuration = CreateWindowW(L"EDIT", L"100", WS_VISIBLE | WS_CHILD | WS_BORDER,
                             100, 10, 60, 20, hwnd, (HMENU)ID_DURATION, hInstance, nullptr);

    CreateWindowW(L"STATIC", L"Caissiers:", WS_VISIBLE | WS_CHILD,
                 10, 40, 80, 20, hwnd, nullptr, hInstance, nullptr);
    hCashiers = CreateWindowW(L"EDIT", L"3", WS_VISIBLE | WS_CHILD | WS_BORDER,
                             100, 40, 60, 20, hwnd, (HMENU)ID_CASHIERS, hInstance, nullptr);

    CreateWindowW(L"STATIC", L"Intervalle:", WS_VISIBLE | WS_CHILD,
                 10, 70, 80, 20, hwnd, nullptr, hInstance, nullptr);
    hInterval = CreateWindowW(L"EDIT", L"5", WS_VISIBLE | WS_CHILD | WS_BORDER,
                             100, 70, 60, 20, hwnd, (HMENU)ID_INTERVAL, hInstance, nullptr);

    CreateWindowW(L"STATIC", L"Service Min:", WS_VISIBLE | WS_CHILD,
                 10, 100, 80, 20, hwnd, nullptr, hInstance, nullptr);
    hMinService = CreateWindowW(L"EDIT", L"3", WS_VISIBLE | WS_CHILD | WS_BORDER,
                               100, 100, 60, 20, hwnd, (HMENU)ID_MINSERVICE, hInstance, nullptr);

    CreateWindowW(L"STATIC", L"Service Max:", WS_VISIBLE | WS_CHILD,
                 10, 130, 80, 20, hwnd, nullptr, hInstance, nullptr);
    hMaxService = CreateWindowW(L"EDIT", L"8", WS_VISIBLE | WS_CHILD | WS_BORDER,
                               100, 130, 60, 20, hwnd, (HMENU)ID_MAXSERVICE, hInstance, nullptr);

    CreateWindowW(L"STATIC", L"Taux VIP:", WS_VISIBLE | WS_CHILD,
                 10, 160, 80, 20, hwnd, nullptr, hInstance, nullptr);
    hVipRate = CreateWindowW(L"EDIT", L"0.2", WS_VISIBLE | WS_CHILD | WS_BORDER,
                            100, 160, 60, 20, hwnd, (HMENU)ID_VIPRATE, hInstance, nullptr);

    CreateWindowW(L"STATIC", L"Patience:", WS_VISIBLE | WS_CHILD,
                 10, 190, 80, 20, hwnd, nullptr, hInstance, nullptr);
    hPatience = CreateWindowW(L"EDIT", L"15", WS_VISIBLE | WS_CHILD | WS_BORDER,
                             100, 190, 60, 20, hwnd, (HMENU)ID_PATIENCE, hInstance, nullptr);

    // Boutons
    hStart = CreateWindowW(L"BUTTON", L"Demarrer", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                          200, 10, 80, 30, hwnd, (HMENU)ID_START, hInstance, nullptr);
    hPause = CreateWindowW(L"BUTTON", L"Pause", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                          290, 10, 80, 30, hwnd, (HMENU)ID_PAUSE, hInstance, nullptr);
    hReset = CreateWindowW(L"BUTTON", L"Reset", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                          380, 10, 80, 30, hwnd, (HMENU)ID_RESET, hInstance, nullptr);

    // Status
    hStatus = CreateWindowW(L"STATIC", L"Pret a simuler", WS_VISIBLE | WS_CHILD | SS_LEFT,
                           200, 50, 300, 20, hwnd, nullptr, hInstance, nullptr);

    // Resultats
    CreateWindowW(L"STATIC", L"Resultats:", WS_VISIBLE | WS_CHILD,
                 200, 80, 80, 20, hwnd, nullptr, hInstance, nullptr);
    hResults = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_READONLY,
                            200, 100, 550, 400, hwnd, nullptr, hInstance, nullptr);

    EnableWindow(hPause, FALSE);
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

#ifdef WITH_PERSISTENCE
    // Initialiser la persistance à partir du fichier config
    persistenceManager = std::make_unique<PersistenceManager>();
    if (persistenceManager->initializeFromConfig("config/database.conf", "database")) {
        persistenceEnabled = true;
    } else {
        persistenceEnabled = false;
        // Optionnel : afficher dans le statut qu'on est en mode sans persistance
        SetWindowTextW(hStatus, L"Persistance desactivee (config/DB) – simulation locale uniquement");
    }
#endif
    return true;
}

void WindowsGUI::Run() {
    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

void WindowsGUI::StartSimulation() {
    if (isRunning) return;
    wchar_t buffer[100];
    GetWindowTextW(hDuration, buffer, 100); int duration = _wtoi(buffer);
    GetWindowTextW(hCashiers, buffer, 100); int cashiers = _wtoi(buffer);
    GetWindowTextW(hInterval, buffer, 100); int interval = _wtoi(buffer);
    GetWindowTextW(hMinService, buffer, 100); int minService = _wtoi(buffer);
    GetWindowTextW(hMaxService, buffer, 100); int maxService = _wtoi(buffer);
    GetWindowTextW(hVipRate, buffer, 100); double vipRate = wcstod(buffer, nullptr);
    GetWindowTextW(hPatience, buffer, 100); int patience = _wtoi(buffer);
    currentEntry = std::make_unique<SimulationEntry>(duration, cashiers, interval, minService, maxService, vipRate, patience);
    simulation = std::make_unique<SimulationNew>(*currentEntry);
#ifdef WITH_PERSISTENCE
    hasSavedSimulation = false;
    persistenceConfigId = -1;
    if (persistenceEnabled && persistenceManager && persistenceManager->isInitialized()) {
        // Sauvegarder la configuration (nom basique avec timestamp)
        SYSTEMTIME st; GetLocalTime(&st);
        wchar_t nameW[128];
        swprintf(nameW, 128, L"GUI_%04d%02d%02d_%02d%02d%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
        int wideLen = lstrlenW(nameW);
        std::string nameUTF8;
        if (wideLen > 0) {
            int needed = WideCharToMultiByte(CP_UTF8, 0, nameW, -1, nullptr, 0, nullptr, nullptr);
            if (needed > 0) {
                nameUTF8.resize(needed - 1);
                WideCharToMultiByte(CP_UTF8, 0, nameW, -1, &nameUTF8[0], needed, nullptr, nullptr);
            }
        } else {
            nameUTF8 = "GUI_Run";
        }

        // Enregistrer la configuration et activer la persistance sur la simulation
        persistenceConfigId = persistenceManager->saveSimulationConfiguration(nameUTF8, *currentEntry);
        if (persistenceConfigId > 0) {
            if (auto repo = persistenceManager->getRepository()) {
                simulation->enablePersistence(repo);
            }
        }
    }
#endif
    currentTime = 0; maxTime = duration; isRunning = true;
    EnableWindow(hStart, FALSE); EnableWindow(hPause, TRUE); EnableWindow(hReset, TRUE);
    SetWindowTextW(hStatus, L"Simulation en cours...");
    SetWindowTextW(hResults, L"Simulation demarree...\r\n");
    SetTimer(hwnd, ID_TIMER, 500, TimerProc);
}

void WindowsGUI::PauseSimulation() {
    if (!isRunning) return;
    isRunning = false; KillTimer(hwnd, ID_TIMER);
    EnableWindow(hStart, TRUE); EnableWindow(hPause, FALSE);
    SetWindowTextW(hStatus, L"Simulation en pause");
}

void WindowsGUI::ResetSimulation() {
    KillTimer(hwnd, ID_TIMER); isRunning = false; currentTime = 0;
    simulation.reset(); currentEntry.reset();
    EnableWindow(hStart, TRUE); EnableWindow(hPause, FALSE);
    SetWindowTextW(hStatus, L"Pret a simuler");
    SetWindowTextW(hResults, L"");
}

void WindowsGUI::RunStep() {
    if (!isRunning || !simulation) return;
    if (simulation->isFinished()) {
        isRunning = false; KillTimer(hwnd, ID_TIMER);
        EnableWindow(hStart, TRUE); EnableWindow(hPause, FALSE);
        SetWindowTextW(hStatus, L"Simulation terminee");
        std::wstring fr = FormatFinalResultsFR_Direct(simulation.get());
        SetWindowTextW(hResults, fr.c_str());

#ifdef WITH_PERSISTENCE
        // Sauvegarder les resultats si la persistance est activee et pas encore faite
        if (!hasSavedSimulation && persistenceEnabled && persistenceManager && persistenceManager->isInitialized() && persistenceConfigId > 0) {
            if (auto repo = persistenceManager->getRepository()) {
                int simId = repo->saveSimulation(persistenceConfigId, simulation.get());
                hasSavedSimulation = (simId > 0);
            }
        }
#endif
        return;
    }
    simulation->step();
    currentTime = simulation->getCurrentTime();
    maxTime = currentEntry->getSimulationDuration();
    if (simulation->isFinished()) {
        std::wstring fr = FormatFinalResultsFR_Direct(simulation.get());
        SetWindowTextW(hResults, fr.c_str());
        SetWindowTextW(hStatus, L"Simulation terminee");
        isRunning = false; KillTimer(hwnd, ID_TIMER);
        EnableWindow(hStart, TRUE); EnableWindow(hPause, FALSE);

#ifdef WITH_PERSISTENCE
        if (!hasSavedSimulation && persistenceEnabled && persistenceManager && persistenceManager->isInitialized() && persistenceConfigId > 0) {
            if (auto repo = persistenceManager->getRepository()) {
                int simId = repo->saveSimulation(persistenceConfigId, simulation.get());
                hasSavedSimulation = (simId > 0);
            }
        }
#endif
    } else {
        UpdateDisplay();
    }
}

void WindowsGUI::UpdateDisplay() {
    std::wstringstream status; status << L"Temps : " << currentTime << L"/" << currentEntry->getSimulationDuration();
    SetWindowTextW(hStatus, status.str().c_str());
    double progression = (double)currentTime / std::max(1, currentEntry->getSimulationDuration()) * 100.0;
    std::wstringstream results;
    results << L"Temps actuel : " << currentTime << L"\r\n";
    results << L"Duree cible : " << currentEntry->getSimulationDuration() << L"\r\n";
    results << std::fixed << std::setprecision(1);
    results << L"Progression : " << progression << L" %\r\n";
    results << L"Clients servis (actuel) : " << simulation->getServedClientCount() << L"\r\n";
    results << L"Clients non servis (actuel) : " << simulation->getNonServedClientCount() << L"\r\n";
    results << L"Attente moyenne (actuel) : " << simulation->getAverageWaitingTime() << L"\r\n";
    results << L"Service moyen (actuel) : " << simulation->getAverageServiceTime() << L"\r\n";
    results << L"Occupation caissiers (actuel) : " << simulation->getCashierOccupationRate() << L" %\r\n";
    SetWindowTextW(hResults, results.str().c_str());
}

static std::wstring FormatFinalResultsFR_Direct(SimulationNew* sim) {
    std::wstringstream oss;
    oss << L"######## Resultats de la simulation ########\r\n";
    if (!sim) { oss << L"Aucune simulation.\r\n"; return oss.str(); }
    oss << L"Duree totale de simulation : " << sim->getConfiguredSimulationDuration() << L"\r\n";
    oss << L"Clients servis : " << sim->getServedClientCount() << L"\r\n";
    oss << L"Clients non servis : " << sim->getNonServedClientCount() << L"\r\n";
    oss << L"Temps d'attente moyen : " << sim->getAverageWaitingTime() << L"\r\n";
    oss << L"Temps de service moyen : " << sim->getAverageServiceTime() << L"\r\n";
    oss << L"Taux d'occupation des caissiers : " << sim->getCashierOccupationRate() << L" %\r\n";
    oss << L"Taux de satisfaction clients : " << sim->getClientSatisfactionRate() << L" %\r\n";
    return oss.str();
}

LRESULT CALLBACK WindowsGUI::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    WindowsGUI* gui = nullptr;
    if (uMsg == WM_NCCREATE) {
        CREATESTRUCTW* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
        gui = reinterpret_cast<WindowsGUI*>(cs->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(gui));
    } else {
        gui = reinterpret_cast<WindowsGUI*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    }
    if (gui) {
        switch (uMsg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
            case ID_START: gui->StartSimulation(); break;
            case ID_PAUSE: gui->PauseSimulation(); break;
            case ID_RESET: gui->ResetSimulation(); break;
            }
            break;
        case WM_DESTROY: PostQuitMessage(0); break;
        }
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

VOID CALLBACK WindowsGUI::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    (void)uMsg; (void)dwTime;
    WindowsGUI* gui = reinterpret_cast<WindowsGUI*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (gui && idEvent == ID_TIMER) gui->RunStep();
}
