#include "../../include/windows/register_window.h"
#include <iostream>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

RegisterWindow::RegisterWindow(QWidget* parent)
    : BaseWindow(WindowType::Login, parent),
    usernameInput(new QLineEdit(this)),
    passwordInput(new QLineEdit(this)),
    registerButton(new QPushButton("Register", this)) {
    initialize();}

void RegisterWindow::initialize() {
    setupUI();
    createConnections();
}

void RegisterWindow::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Username:", this));
    layout->addWidget(usernameInput);

    layout->addWidget(new QLabel("Password:", this));
    passwordInput->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordInput);

    layout->addWidget(registerButton);

    setLayout(layout);
}

void RegisterWindow::createConnections() {
    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::onRegisterClicked);
}

void RegisterWindow::onRegisterClicked() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Username and password cannot be empty!");
    }
    else {
        QMessageBox::information(this, "Success", "Registration successful!");
        std::cout << "Success";
    }
}