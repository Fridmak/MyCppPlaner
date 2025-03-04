#include "../../include/managers/window_manager.h"
#include "../../include/windows/main_window.h"
#include "../../include/windows/settings_window.h"
#include "../../include/windows/register_window.h"
#include <QSettings>

const QString WindowManager::SETTINGS_ORG = "MyPlanner";
const QString WindowManager::SETTINGS_APP = "PlannerApp";
const QString WindowManager::KEY_LAST_WINDOW = "LastWindow";

WindowManager& WindowManager::instance() {
    static WindowManager instance;
    return instance;
}

WindowManager::~WindowManager() {
    while (!windows.isEmpty()) {
        windows.pop();
    }
}

void WindowManager::showWindow(WindowType type, const WindowParamsMap& params) {
    auto newWindow = createWindow(type, params);
    if (!newWindow) return;

    newWindow->show();
    windows.push(std::move(newWindow));
    navigationHistory.push(type);
    saveLastWindow(type);
}

void WindowManager::hideCurrentWindow() {
    if (!windows.isEmpty()) {
        windows.top()->hide();
    }
}

void WindowManager::closeCurrentWindow() {
    if (windows.isEmpty()) return;

    windows.pop();
    navigationHistory.pop();

    if (!windows.isEmpty()) {
        windows.top()->show();
        saveLastWindow(windows.top()->type);
    }
}

void WindowManager::closeAllAndShow(WindowType type, const WindowParamsMap& params) {
    while (!windows.isEmpty()) {
        windows.pop();
    }
    navigationHistory.clear();

    showWindow(type, params);
}

BaseWindow* WindowManager::getCurrentWindow() const {
    return windows.isEmpty() ? nullptr : windows.top().get();
}

WindowType WindowManager::getLastWindow() const {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    return static_cast<WindowType>(settings.value(KEY_LAST_WINDOW, static_cast<int>(WindowType::Login)).toInt());
}

void WindowManager::saveLastWindow(WindowType type) {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.setValue(KEY_LAST_WINDOW, static_cast<int>(type));
}

std::unique_ptr<BaseWindow> WindowManager::createWindow(WindowType type, const WindowParamsMap& params) {
    std::unique_ptr<BaseWindow> window;
    
    switch (type) {
        case WindowType::Login:
            window = std::make_unique<RegisterWindow>();
            break;
        // ToDO: Add more
        default:
            return nullptr;
    }

    if (window) {
        window->initialize();
    }

    return window;
}