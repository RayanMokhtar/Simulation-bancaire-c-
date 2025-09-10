#include <QApplication>
#include "include/SimulationGUI.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    SimulationGUI window;
    window.show();
    
    return app.exec();
}
