@echo off
echo ===================================================
echo    SIMULATION BANCAIRE C++ - VERSION FINALE
echo         Conforme à l'architecture Java
echo ===================================================
echo.

REM Nettoyage initial
echo Nettoyage des anciens fichiers...
if exist simulation_java_like.exe del simulation_java_like.exe
if exist demo_auto.exe del demo_auto.exe

REM Compilation version interactive
echo.
echo === COMPILATION VERSION INTERACTIVE ===
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude ^
    main_new.cpp ^
    src/AbstractOperation.cpp ^
    src/Consultation.cpp ^
    src/Transfer.cpp ^
    src/Withdraw.cpp ^
    src/AbstractClient.cpp ^
    src/ClientNew.cpp ^
    src/VIPClient.cpp ^
    src/Caissier.cpp ^
    src/FileAttente.cpp ^
    src/Bank.cpp ^
    src/StatisticManager.cpp ^
    src/SimulationUtility.cpp ^
    src/SimulationNew.cpp ^
    -o simulation_java_like.exe

if %ERRORLEVEL% NEQ 0 (
    echo ✗ ERREUR: Echec de la compilation interactive
    pause
    exit /b 1
)
echo ✓ Version interactive compilée avec succès !

REM Compilation version démo automatique
echo.
echo === COMPILATION VERSION DÉMO AUTOMATIQUE ===
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude ^
    demo_auto.cpp ^
    src/AbstractOperation.cpp ^
    src/Consultation.cpp ^
    src/Transfer.cpp ^
    src/Withdraw.cpp ^
    src/AbstractClient.cpp ^
    src/ClientNew.cpp ^
    src/VIPClient.cpp ^
    src/Caissier.cpp ^
    src/FileAttente.cpp ^
    src/Bank.cpp ^
    src/StatisticManager.cpp ^
    src/SimulationUtility.cpp ^
    src/SimulationNew.cpp ^
    -o demo_auto.exe

if %ERRORLEVEL% NEQ 0 (
    echo ✗ ERREUR: Echec de la compilation démo
    pause
    exit /b 1
)
echo ✓ Version démo compilée avec succès !

echo.
echo ===================================================
echo    ✓ COMPILATION RÉUSSIE - TOUTES LES VERSIONS
echo ===================================================
echo.

:menu
echo === MENU PRINCIPAL ===
echo.
echo 1. Exécuter la démonstration automatique
echo 2. Exécuter la version interactive
echo 3. Informations sur le projet
echo 4. Nettoyer les fichiers compilés
echo 0. Quitter
echo.
set /p choix="Votre choix: "

if "%choix%"=="1" goto demo
if "%choix%"=="2" goto interactive
if "%choix%"=="3" goto info
if "%choix%"=="4" goto clean
if "%choix%"=="0" goto end
echo Choix invalide, veuillez réessayer.
goto menu

:demo
echo.
echo ===================================================
echo        DÉMONSTRATION AUTOMATIQUE
echo ===================================================
demo_auto.exe
echo.
echo Appuyez sur une touche pour revenir au menu...
pause >nul
goto menu

:interactive
echo.
echo ===================================================
echo         VERSION INTERACTIVE
echo ===================================================
echo Utilisez les options du menu pour configurer votre simulation
echo.
simulation_java_like.exe
echo.
echo Appuyez sur une touche pour revenir au menu...
pause >nul
goto menu

:info
echo.
echo ===================================================
echo         INFORMATIONS SUR LE PROJET
echo ===================================================
echo.
echo ✓ Architecture conforme à la version Java de référence
echo ✓ Hiérarchie AbstractClient → ClientNew/VIPClient  
echo ✓ Hiérarchie AbstractOperation → Consultation/Transfer/Withdraw
echo ✓ File d'attente unique avec priorité VIP
echo ✓ Gestion de patience pour les consultations uniquement
echo ✓ Statistiques détaillées (temps d'attente, occupation, satisfaction)
echo ✓ Simulation temporelle identique à Java
echo.
echo Fonctionnalités implémentées:
echo - Arrivées uniformes des clients
echo - Service aléatoire entre min/max
echo - Trois types d'opérations avec urgence
echo - Clients VIP prioritaires dans la file
echo - Clients consultation partent après limite patience
echo - Calculs statistiques avancés
echo.
echo Extension: Interface graphique disponible avec Qt6
echo.
pause
goto menu

:clean
echo.
echo Nettoyage des fichiers compilés...
if exist simulation_java_like.exe del simulation_java_like.exe
if exist demo_auto.exe del demo_auto.exe
if exist test_*.exe del test_*.exe
if exist *.o del *.o
echo Nettoyage terminé.
echo.
goto menu

:end
echo.
echo ===================================================
echo    Merci d'avoir utilisé la simulation bancaire !
echo        Version C++ conforme architecture Java
echo ===================================================
pause
