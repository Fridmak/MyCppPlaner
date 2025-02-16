#pragma once

#include <QWidget>

enum class WindowType {
    Login,
    Main,
    Settings,
    TaskDetails,
    Calendar
};

class BaseWindow : public QWidget {
    Q_OBJECT

public:
    virtual void initialize() = 0;
    virtual void cleanup() = 0;

    BaseWindow(QWidget* parent = nullptr) : QWidget(parent) {}
    virtual ~BaseWindow() = default;

protected:
    void showNextWindow(WindowType type);
    void goBack();
private:
    void setupUI();
};