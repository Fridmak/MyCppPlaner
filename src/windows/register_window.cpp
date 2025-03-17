#include "../../include/windows/register_window.h"
#include "../../include/managers/password_manager.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <iostream>

RegisterWindow::RegisterWindow(WindowType type, QWidget* parent)
    : BaseWindow(type, parent), usernameInput(nullptr), passwordInput(nullptr), registerButton(nullptr) {
    setupUI();
}

void RegisterWindow::initialize(const std::unordered_map<std::string, std::variant<int, QString, bool>>& params) {
    auto titleIt = params.find("title");
    if (titleIt != params.end() && std::holds_alternative<QString>(titleIt->second)) {
        setWindowTitle(std::get<QString>(titleIt->second));
    }
    else {
        setWindowTitle("Register");
    }

    auto fullscreenIt = params.find("fullscreen");
    if (fullscreenIt != params.end() && std::holds_alternative<bool>(fullscreenIt->second)) {
        if (std::get<bool>(fullscreenIt->second)) {
            showFullScreen();
        }
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
	passwordManager.saveUserInfo(username, password, false);

    showNextWindow(WindowType::Main);
}