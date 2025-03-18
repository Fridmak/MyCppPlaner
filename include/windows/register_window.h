#pragma once

#include "../../include/windows/base_window.h"
#include <unordered_map>
#include <variant>
#include <QLineEdit>
#include <QPushButton>

class RegisterWindow : public BaseWindow {
    Q_OBJECT

public:
    explicit RegisterWindow(WindowParams params, QWidget* parent = nullptr);
    void initialize(const WindowParams& params) override;

protected:
    void setupUI() override;

private:
    QLineEdit* usernameInput;
    QLineEdit* passwordInput;
    QPushButton* registerButton;

    void handleRegisterButtonClicked();
    void signUpButtonClicked();
};