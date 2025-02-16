#include "../include/windows/register_window.h"
#include <QVBoxLayout>
#include <QCalendarWidget>

RegisterWindow::RegisterWindow(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
}

RegisterWindow::~RegisterWindow() {
}

void RegisterWindow::setupUI() {
    

    QWidget* centralWidget = new QWidget(this);

    resize(400, 600);
    setWindowTitle("My Planer Registration");
}