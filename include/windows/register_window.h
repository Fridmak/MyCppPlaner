#pragma once
#include "base_window.h"
#include <QLineEdit>

class RegisterWindow : public BaseWindow {
    Q_OBJECT

public:
    RegisterWindow(QWidget* parent = nullptr);
    void initialize() override;

private:
    void setupUI();
    void createConnections();

private slots:
    void onRegisterClicked();

private:
    QLineEdit* usernameInput;
    QLineEdit* passwordInput;
    QPushButton* registerButton;
};