Param(
    [switch]$Run,
    [switch]$TestDB,
    [string]$MySQLDir = "C:\mysql-connector-cpp",
    [switch]$Help
)

if ($Help) {
    Write-Host "Usage: .\build_mysql.ps1 [-Run] [-TestDB] [-MySQLDir <path>] [-Help]" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Parameters:" -ForegroundColor Cyan
    Write-Host "  -Run        : Execute the compiled program after building"
    Write-Host "  -TestDB     : Compile and run database connection test"
    Write-Host "  -MySQLDir   : Path to MySQL Connector/C++ installation (default: C:\mysql-connector-cpp)"
    Write-Host "  -Help       : Show this help message"
    Write-Host ""
    Write-Host "Examples:" -ForegroundColor Green
    Write-Host "  .\build_mysql.ps1                    # Just compile"
    Write-Host "  .\build_mysql.ps1 -Run               # Compile and run"
    Write-Host "  .\build_mysql.ps1 -TestDB            # Test database connection"
    Write-Host "  .\build_mysql.ps1 -MySQLDir 'C:\mysql'  # Custom MySQL path"
    exit 0
}

$ErrorActionPreference = 'Stop'

# Vérifier si MySQL Connector existe
if (-not (Test-Path $MySQLDir)) {
    Write-Host "ERROR: MySQL Connector/C++ not found at: $MySQLDir" -ForegroundColor Red
    Write-Host "Please install MySQL Connector/C++ or specify correct path with -MySQLDir" -ForegroundColor Yellow
    Write-Host "Download from: https://dev.mysql.com/downloads/connector/cpp/" -ForegroundColor Cyan
    exit 1
}

Write-Host "Using MySQL Connector at: $MySQLDir" -ForegroundColor Green

# Choisir les fichiers source selon le mode
if ($TestDB) {
    $src = @(
        'test_database.cpp',
        'src/persistence/DatabaseConnection.cpp',
        'src/persistence/DatabaseConfig.cpp'
    )
    $outputFile = 'test_database.exe'
    Write-Host "Building database test..." -ForegroundColor Cyan
} else {
    $src = @(
        'main_mysql.cpp',
        'src/bank/Bank.cpp',
        'src/bank/Caissier.cpp',
        'src/bank/FileAttente.cpp',
        'src/simulation/SimulationNew.cpp',
        'src/simulation/SimulationUtility.cpp',
        'src/simulation/StatisticManager.cpp',
        'src/simulation/AbstractClient.cpp',
        'src/simulation/AbstractOperation.cpp',
        'src/client/ClientNew.cpp',
        'src/client/VIPClient.cpp',
        'src/client/Consultation.cpp',
        'src/client/Transfer.cpp',
        'src/client/Withdraw.cpp',
        'src/persistence/DatabaseConnection.cpp',
        'src/persistence/DatabaseConfig.cpp',
        'src/persistence/SimulationRepository.cpp',
        'src/persistence/PersistenceManager.cpp'
    )
    $outputFile = 'simulation_mysql.exe'
    Write-Host "Building simulation with MySQL persistence (always enabled)..." -ForegroundColor Cyan
}

# Arguments de compilation (MySQL persistence toujours activée)
$args = @(
    '-std=c++17',
    '-O2',
    '-Wall',
    '-Wextra',
    '-pedantic'
)

# Ajouter les fichiers source
$args += $src

# Ajouter les chemins d'include et de bibliothèques
$args += @(
    '-Iinclude',
    "-I$MySQLDir\include",
    "-L$MySQLDir\lib64",
    '-lmysqlcppconn8',
    '-lssl',
    '-lcrypto',
    '-o',
    $outputFile
)

Write-Host "Compiling with MySQL persistence support..." -ForegroundColor Cyan
Write-Host "Command: g++ $($args -join ' ')" -ForegroundColor DarkGray

try {
    & g++ @args
    Write-Host "✓ Build successful: $outputFile" -ForegroundColor Green
    
    # Vérifier si les DLL MySQL sont nécessaires
    $mysqlDll = Join-Path $MySQLDir "lib64\libmysql.dll"
    if (Test-Path $mysqlDll) {
        $localDll = ".\libmysql.dll"
        if (-not (Test-Path $localDll)) {
            Copy-Item $mysqlDll $localDll
            Write-Host "✓ Copied MySQL DLL to local directory" -ForegroundColor Green
        }
    }
    
    if ($Run -or $TestDB) {
        Write-Host ""
        if ($TestDB) {
            Write-Host "Running database connection test..." -ForegroundColor Yellow
            & .\test_database.exe
        } else {
            Write-Host "Launching persistence demo..." -ForegroundColor Yellow
            Write-Host "Make sure your database is configured in config/database.conf"
            Write-Host ""
            & .\simulation_persistence.exe
        }
    } else {
        Write-Host ""
        if ($TestDB) {
            Write-Host "To test database connection:" -ForegroundColor Yellow
            Write-Host "  .\test_database.exe" -ForegroundColor Cyan
        } else {
            Write-Host "To run the persistence demo:" -ForegroundColor Yellow
            Write-Host "  .\simulation_persistence.exe" -ForegroundColor Cyan
        }
        Write-Host ""
        Write-Host "Make sure to configure your database in config/database.conf" -ForegroundColor Yellow
    }
    
} catch {
    Write-Host "✗ Build failed!" -ForegroundColor Red
    Write-Host "Error: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host ""
    Write-Host "Common issues:" -ForegroundColor Yellow
    Write-Host "  - MySQL Connector/C++ not installed or wrong path"
    Write-Host "  - Missing g++ compiler"
    Write-Host "  - Incorrect MySQL library path"
    Write-Host ""
    Write-Host "Solutions:" -ForegroundColor Cyan
    Write-Host "  - Install MySQL Connector/C++ from: https://dev.mysql.com/downloads/connector/cpp/"
    Write-Host "  - Use -MySQLDir parameter to specify correct path"
    Write-Host "  - Check that g++ is in your PATH"
    exit 1
}
