#pragma once

#include <QString>
#include <QStack>
#include "../windows/base_window.h"

class WindowManager {
public:
    static WindowManager& instance();

    void showWindow(WindowType type);
    void showTaskWindow(const QString& taskId);
    void closeCurrentWindow();
    void showNotification(const QString& message);

    void goBack();
    void goToMain();

    QStack<WindowType> navigationHistory;

private:
    static const QString SETTINGS_ORG;
    static const QString SETTINGS_APP;
    static const QString KEY_LAST_WINDOW;

    WindowManager() = default;
    ~WindowManager() = default;

    void saveLastWindow(WindowType type);
    WindowType getLastWindow() const;
    void loadWindow(WindowType window);

    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;
};