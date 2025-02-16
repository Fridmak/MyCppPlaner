#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class RegisterWindow : public QWidget
{
    Q_OBJECT

public:
    RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();

    void showNextWindow(WindowType type);
    void goBack();

private:
    QLineEdit* usernameLineEdit;
    QLineEdit* passwordLineEdit;
    QPushButton* registerButton;
    QLabel* statusLabel;

    void setupUI();
};