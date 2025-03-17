#pragma once

#include <QString>
#include <QStack>
#include <memory>
#include <unordered_map>
#include <variant>
#include <string>
#include "../windows/base_window.h"

using WindowParams = std::variant<int, QString, bool>;
using WindowParamsMap = std::unordered_map<std::string, WindowParams>;

/**
 * @brief Show|Hide|Navigate.
 */
class WindowManager {
public:
    /*
     * @brief Singleton.
     * @return &WindowManager.
     */
    static WindowManager& instance();

    /**
     * @brief Shows window with params...
     * @param type of window (written in base_window).
	 * @param params Params for window.
     */
    void showNewWindow(WindowType type, const WindowParamsMap& params = {});

    /**
     * @brief Closes current window.
     */
    void closeCurrentWindow();

    /**
     * @brief Closes all windows and shows widow of type and params...
     * @param type - of window.
     * @param params - for window.
     */
    void closeAllAndShow(WindowType type, const WindowParamsMap& params = {});

    /**
     * @brief Hides current window not closing it.
     */
    void hideCurrentWindow();
    /*
	* @brief Shows window was hidden...
    */
	void showWindow();

    /**
     * @brief Return pointer to current window.
     * @return pointer | nullptr.
     */
    BaseWindow* getCurrentWindow() const;

    /**
     * @brief Returns type of last window.
     * @return type of last opened window.
     */
    WindowType getLastWindow() const;

private:
    static const QString SETTINGS_ORG;
    static const QString SETTINGS_APP;
    static const QString KEY_LAST_WINDOW;

    WindowManager() = default;
    ~WindowManager();


    void saveLastWindow(WindowType type);

    std::unique_ptr<BaseWindow> createWindow(WindowType type, const WindowParamsMap& params);

    QStack<std::unique_ptr<BaseWindow>> windows;
    QStack<WindowType> navigationHistory;

    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;
};