#pragma once

#include <QMainWindow>
#include "base_window.h"

class MainWindow : BaseWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void showNextWindow(WindowType type);
    void goBack();

private:
    void setupUI();
};
