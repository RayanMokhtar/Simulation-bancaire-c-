Param(
    [switch]$Gui = $true
)

$ErrorActionPreference = "Stop"

$inc = @(
    'include',
    'third_party/mysql/include'
)

$src = @(
    'main_gui_windows.cpp',
    'src/WindowsGUI.cpp',
    'src/bank/*.cpp',
    'src/client/*.cpp',
    'src/simulation/*.cpp',
    'src/persistence/*.cpp'
)

$libs = @(
    'third_party/mysql/lib'
)

$cmd = @('g++','-std=c++17','-DWITH_PERSISTENCE')
$inc | ForEach-Object { $cmd += ("-I`"$_`"") }
$cmd += $src
$libs | ForEach-Object { $cmd += ("-L`"$_`"") }
# Choix de la lib MySQL selon ce qui est disponible
$libDir = 'third_party/mysql/lib'
$dllDir = 'third_party/mysql/bin'
$libMysqlA = Join-Path $libDir 'libmysql.dll.a'
$libMariaA = Join-Path $libDir 'libmariadb.dll.a'
$dllMysql  = Join-Path $dllDir 'libmysql.dll'
$dllMaria  = Join-Path $dllDir 'libmariadb.dll'

if (Test-Path $libMysqlA) {
    $cmd += '-lmysql'
} elseif (Test-Path $libMariaA) {
    $cmd += '-lmariadb'
} elseif (Test-Path $dllMysql) {
    # lier directement la DLL comme dernier recours
    $cmd += $dllMysql
} elseif (Test-Path $dllMaria) {
    $cmd += $dllMaria
} else {
    Write-Host 'WARNING: Aucune import lib MySQL trouvée. Placez libmysql.dll.a (ou libmariadb.dll.a) dans third_party/mysql/lib' -ForegroundColor Yellow
}

$cmd += @('-lws2_32','-lsecur32','-lcrypt32','-lbcrypt','-luser32','-lgdi32','-lkernel32','-lcomctl32','-lole32','-loleaut32','-luuid')
if ($Gui) { $cmd += @('-mwindows','-municode') }
$cmd += @('-o','BankSimulation_MySQL.exe')

# Tenter de libérer l'EXE/DLL s'ils sont en cours d'utilisation
try { & taskkill /IM BankSimulation_MySQL.exe /F | Out-Null } catch { }
Start-Sleep -Milliseconds 200

Write-Host "Building with:" -ForegroundColor Cyan
Write-Host ($cmd -join ' ') -ForegroundColor DarkGray
if ($cmd.Count -gt 1) {
    & $cmd[0] @($cmd[1..($cmd.Count-1)])
} else {
    & $cmd
}

try {
    if (Test-Path $dllMysql) { Copy-Item -Force $dllMysql '.\libmysql.dll' }
    elseif (Test-Path $dllMaria) { Copy-Item -Force $dllMaria '.\libmariadb.dll' }
} catch {
    Write-Host "WARN: Echec de copie de la DLL (peut être utilisée). Fermez l'application puis re-lancez la copie." -ForegroundColor Yellow
}

Write-Host "Done. Run .\BankSimulation_MySQL.exe" -ForegroundColor Green