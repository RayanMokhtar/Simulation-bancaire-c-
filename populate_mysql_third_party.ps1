Param(
    [string]$ServerRoot = "C:\Program Files\MySQL\MySQL Server 8.0",
    [string]$MariaDBRoot = ""
)

$ErrorActionPreference = "Stop"

function Ensure-Dir($p) { if (-not (Test-Path $p)) { New-Item -ItemType Directory -Path $p | Out-Null } }

$dstInc = 'third_party/mysql/include'
$dstLib = 'third_party/mysql/lib'
$dstBin = 'third_party/mysql/bin'
Ensure-Dir $dstInc; Ensure-Dir $dstLib; Ensure-Dir $dstBin

function Copy-From-MySQLServer([string]$root) {
    $srcInc = Join-Path $root 'include'
    $srcLib = Join-Path $root 'lib'
    if (-not (Test-Path $srcInc) -or -not (Test-Path $srcLib)) { return $false }
    Write-Host "Copie des headers (MySQL Server) depuis $srcInc" -ForegroundColor Cyan
    Copy-Item -Recurse -Force (Join-Path $srcInc '*') $dstInc
    $dll = Join-Path $srcLib 'libmysql.dll'
    if (Test-Path $dll) {
        Write-Host "Copie libmysql.dll" -ForegroundColor Cyan
        Copy-Item -Force $dll $dstBin
        Copy-Item -Force $dll $dstLib
    } else {
        Write-Host "libmysql.dll introuvable dans $srcLib" -ForegroundColor Yellow
    }
    return $true
}

function Copy-From-MariaDB([string]$root) {
    $srcInc = Join-Path $root 'include'
    $srcBin = Join-Path $root 'bin'
    $srcLib = Join-Path $root 'lib'
    $srcLibMaria = Join-Path $root 'lib\mariadb'
    if (-not (Test-Path $srcInc)) { return $false }
    Write-Host "Copie des headers (MariaDB C) depuis $srcInc" -ForegroundColor Cyan
    Copy-Item -Recurse -Force (Join-Path $srcInc '*') $dstInc
    # DLL
    $dllMaria = Join-Path $srcBin 'libmariadb.dll'
    if (Test-Path $dllMaria) { Copy-Item -Force $dllMaria $dstBin; Copy-Item -Force $dllMaria $dstLib }
    # Import lib
    $imp1 = Join-Path $srcLib 'libmariadb.dll.a'
    $imp2 = Join-Path $srcLibMaria 'libmariadb.dll.a'
    if (Test-Path $imp1) { Copy-Item -Force $imp1 $dstLib }
    elseif (Test-Path $imp2) { Copy-Item -Force $imp2 $dstLib }
    else { Write-Host "libmariadb.dll.a introuvable (optionnel)" -ForegroundColor Yellow }
    return $true
}

$ok = $false
if ($ServerRoot -and (Test-Path $ServerRoot)) {
    $ok = Copy-From-MySQLServer $ServerRoot
}
if (-not $ok -and $MariaDBRoot -and (Test-Path $MariaDBRoot)) {
    $ok = Copy-From-MariaDB $MariaDBRoot
}

if (-not $ok) {
    Write-Host "Aucune source compatible trouvée." -ForegroundColor Yellow
    Write-Host "- Pour MySQL C API: installez 'MySQL Server 8.0' et passez -ServerRoot" -ForegroundColor Yellow
    Write-Host "- OU installez 'MariaDB Connector/C' et passez -MariaDBRoot" -ForegroundColor Yellow
    Write-Host "Note: 'MySQL Connector C++' (mysqlcppconn*.dll) ne fournit pas <mysql.h> ni libmysql.dll nécessaires." -ForegroundColor Yellow
    exit 1
}

Write-Host "Terminé. Build possible avec build_local_mysql.ps1" -ForegroundColor Green