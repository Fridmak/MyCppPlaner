#pragma once

#include <QString>
#include <QStack>
#include <memory>
#include <variant>
#include "../windows/base_window.h"

using WindowParams = std::variant<int, QString, bool>;
using WindowParamsMap = std::unordered_map<std::string, WindowParams>;

class WindowManager {
public:
    static WindowManager& instance();

    void showWindow(WindowType type, const WindowParamsMap& params = {});
    
    void closeCurrentWindow();
    
    void closeAllAndShow(WindowType type, const WindowParamsMap& params = {});

    void hideCurrentWindow();
    
    BaseWindow* getCurrentWindow() const;
    
    WindowType getLastWindow() const;

    QStack<WindowType> navigationHistory;

private:
    static const QString SETTINGS_ORG;
    static const QString SETTINGS_APP;
    static const QString KEY_LAST_WINDOW;

    WindowManager() = default;
    ~WindowManager();

    void saveLastWindow(WindowType type);
    
    std::unique_ptr<BaseWindow> createWindow(WindowType type, const WindowParamsMap& params);
    
    QStack<std::unique_ptr<BaseWindow>> windows;
        
    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;
};