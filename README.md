# Simulation-bancaire-c-

Commande compilation : g++ -std=c++17 -I include src/*.cpp -o simulation_gui -lsfml-graphics -lsfml-window -lsfml-system




nouvelle compilation depuis split par dossiers  : g++ -std=c++14 -I include $(find . -name "*.cpp") -o simulation_gui -lsfml-graphics -lsfml-window -lsfml-system

à exécuter directement depuis appli/ 