#include "../include/mainwindow.h"
#include <QVBoxLayout>
#include <QCalendarWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    QCalendarWidget* calendar = new QCalendarWidget(this);
    layout->addWidget(calendar);

    resize(800, 600);
    setWindowTitle("My Calendar Application");
}