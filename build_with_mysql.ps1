# Script PowerShell pour compiler avec support MySQL
# Assurez-vous d'avoir installé MySQL Connector/C++ avant d'exécuter ce script

param(
    [string]$Target = "demo_persistence",
    [switch]$Run,
    [string]$MySQLPath = "C:\Program Files\MySQL\MySQL Server 8.0"
)

$ErrorActionPreference = 'Stop'

# Vérifier si MySQL Connector/C++ est disponible
$mysqlInclude = "$MySQLPath\include"
$mysqlLib = "$MySQLPath\lib"

if (!(Test-Path $mysqlInclude)) {
    Write-Host "ATTENTION: Répertoire MySQL include introuvable: $mysqlInclude" -ForegroundColor Yellow
    Write-Host "Veuillez installer MySQL Connector/C++ ou ajuster le chemin avec -MySQLPath" -ForegroundColor Yellow
    Write-Host "Téléchargement: https://dev.mysql.com/downloads/connector/cpp/" -ForegroundColor Yellow
    
    # Tentative avec des chemins alternatifs
    $altPaths = @(
        "C:\mysql\include",
        "C:\Program Files\MySQL\Connector C++ 8.0\include",
        "C:\Program Files (x86)\MySQL\MySQL Connector C++ 8.0\include"
    )
    
    foreach ($altPath in $altPaths) {
        if (Test-Path $altPath) {
            $mysqlInclude = $altPath
            $mysqlLib = $altPath -replace "include", "lib"
            Write-Host "Trouvé: $mysqlInclude" -ForegroundColor Green
            break
        }
    }
}

# Sources communes
$commonSrc = @(
    'src/bank/Bank.cpp','src/bank/Caissier.cpp','src/bank/FileAttente.cpp',
    'src/simulation/SimulationNew.cpp','src/simulation/SimulationUtility.cpp','src/simulation/StatisticManager.cpp',
    'src/client/AbstractClient.cpp','src/client/AbstractOperation.cpp','src/client/ClientNew.cpp',
    'src/client/VIPClient.cpp','src/client/Consultation.cpp','src/client/Transfer.cpp','src/client/Withdraw.cpp'
)

# Sources spécifiques à la persistance
$persistenceSrc = @(
    'src/persistence/DatabaseConnection.cpp',
    'src/persistence/SimulationRepository.cpp',
    'src/persistence/PersistenceManager.cpp'
)

# Arguments de compilation de base
$baseArgs = @('-std=c++17','-O2','-Wall','-Wextra','-pedantic')

# Configuration selon la cible
switch ($Target) {
    "demo_persistence" {
        $sources = @('demo_persistence.cpp') + $commonSrc + $persistenceSrc
        $includes = @('-Iinclude', "-I$mysqlInclude")
        $libs = @("-L$mysqlLib", '-lmysqlclient')
        $output = 'demo_persistence.exe'
        Write-Host "Compilation de la démonstration avec persistance MySQL..." -ForegroundColor Cyan
    }
    "gui_with_persistence" {
        $sources = @('main_gui_windows.cpp', 'src/WindowsGUI.cpp') + $commonSrc + $persistenceSrc
        $includes = @('-Iinclude', "-I$mysqlInclude")
        $libs = @("-L$mysqlLib", '-lmysqlclient', '-lgdi32', '-luser32')
        $defines = @('-municode', '-DUNICODE', '-D_UNICODE', '-DWITH_PERSISTENCE')
        $output = 'simulation_gui_persistence.exe'
        Write-Host "Compilation de l'interface graphique avec persistance..." -ForegroundColor Cyan
    }
    "gui_only" {
        $sources = @('main_gui_windows.cpp', 'src/WindowsGUI.cpp') + $commonSrc
        $includes = @('-Iinclude')
        $libs = @('-lgdi32', '-luser32')
        $defines = @('-municode', '-DUNICODE', '-D_UNICODE')
        $output = 'simulation_gui_windows.exe'
        Write-Host "Compilation de l'interface graphique sans persistance..." -ForegroundColor Cyan
    }
    default {
        Write-Host "Cible inconnue: $Target" -ForegroundColor Red
        Write-Host "Cibles disponibles: demo_persistence, gui_with_persistence, gui_only" -ForegroundColor Yellow
        exit 1
    }
}

# Construire la commande de compilation
$compileArgs = $baseArgs + $defines + $sources + $includes + $libs + @('-o', $output)

Write-Host "Commande: g++ $($compileArgs -join ' ')" -ForegroundColor Gray

try {
    & g++ @compileArgs
    Write-Host "Compilation réussie: $output" -ForegroundColor Green
    
    if ($Run) {
        Write-Host "Lancement de l'application..." -ForegroundColor Yellow
        & ".\$output"
    }
} catch {
    Write-Host "Erreur de compilation:" -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Red
    
    if ($Target -eq "demo_persistence" -or $Target -eq "gui_with_persistence") {
        Write-Host "`nPour résoudre les erreurs MySQL:" -ForegroundColor Yellow
        Write-Host "1. Installez MySQL Server et MySQL Connector/C++" -ForegroundColor White
        Write-Host "2. Vérifiez les chemins d'include et lib" -ForegroundColor White
        Write-Host "3. Utilisez -MySQLPath pour spécifier un chemin personnalisé" -ForegroundColor White
        Write-Host "4. Compilez sans persistance avec -Target gui_only" -ForegroundColor White
    }
    
    exit 1
}
