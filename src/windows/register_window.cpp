#include "../../include/windows/register_window.h"
#include "../../include/managers/password_manager.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <iostream>

RegisterWindow::RegisterWindow(WindowParams params, QWidget* parent)
    : BaseWindow(params, parent), usernameInput(nullptr), passwordInput(nullptr), registerButton(nullptr) {
    setupUI();
}

void RegisterWindow::initialize(const WindowParams& params) {
    auto dbAnswer = getStoredInfoFromParamsByKey("title", params);

    if (std::holds_alternative<Error>(dbAnswer) || !std::holds_alternative<QString>(dbAnswer)) {
        Error error = std::get<Error>(dbAnswer);
        setWindowTitle("Register");
    }
    else {
        auto title = std::get<QString>(dbAnswer);
        setWindowTitle(title);
    }

    auto fullScreen = getStoredInfoFromParamsByKey("size", params);

    if (!std::holds_alternative<Error>(fullScreen) && std::holds_alternative<bool>(fullScreen))){
        showFullScreen();
    }
}

void RegisterWindow::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("Register", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Enter username");
    layout->addWidget(usernameInput);

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Enter password");
    passwordInput->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordInput);

    registerButton = new QPushButton("Register", this);
    layout->addWidget(registerButton);

    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::handleRegisterButtonClicked);

    setLayout(layout);
}

void RegisterWindow::handleRegisterButtonClicked() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Username and password cannot be empty.");
        return;
    }

    PasswordManager& passwordManager = PasswordManager::instance();

    if (passwordManager.hasStoredInfo(username)) {
        QMessageBox::warning(this, "Error", "User already registered");
        return;
    }

	passwordManager.saveUserInfo(username, password, false);

    showNextWindow();
}

void RegisterWindow::signUpButtonClicked() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Username and password cannot be empty.");
        return;
    }

    PasswordManager& passwordManager = PasswordManager::instance();
    bool correct = passwordManager.checkPassword(username, password);

    if (!correct) {
        QMessageBox::warning(this, "Error", "Username or password are incorrect");
        return;
    }

    showNextWindow();
}

void RegisterWindow::showNextWindow(WindowParams params) {

}