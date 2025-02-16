#include "../include/windows/main_window.h"
#include "../include/windows/register_window.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    RegisterWindow window;
    window.show();
    
    return app.exec();
}
