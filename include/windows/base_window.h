#pragma once

#include <QWidget>
#include <string>

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
    explicit BaseWindow(WindowType type, QWidget* parent = nullptr)
        : QWidget(parent), type(type) {
    }

    virtual ~BaseWindow() = default;

    WindowType type;
    virtual void initialize(const std::unordered_map<std::string, std::variant<int, QString, bool>>& params) = 0;

protected:
    virtual void setupUI() = 0;
    virtual void cleanup() {}
    virtual void showNextWindow(WindowType type) { Q_UNUSED(type); }
    virtual void goBack() {}
    virtual void goHome() {}
};