#pragma once

#include <QString>
#include <QTime>
#include <QSettings>

class SettingsManager {
public:
    static SettingsManager& instance();

    QString getLanguage() const;
    void setLanguage(const QString& lang);
    QString getTheme() const;
    void setTheme(const QString& theme);
    void resetToDefaults();
    
    bool areNotificationsEnabled() const;
    void setNotificationsEnabled(bool enabled);
    int getNotificationLeadTime() const;
    void setNotificationLeadTime(int minutes);

    /**
     * @brief Registers a callback to be called when the language changes.
    * @param callback Function to call when the language changes.
    */
    void onLanguageChanged(std::function<void(const QString&)> callback);

    /**
     * @brief Registers a callback to be called when the theme changes.
     * @param callback Function to call when the theme changes.
     */
    void onThemeChanged(std::function<void(const QString&)> callback);

private:
    static const QString SETTINGS_ORG;
    static const QString SETTINGS_APP;
    static const QString KEY_LANGUAGE;
    static const QString KEY_THEME;
    static const QString KEY_NOTIFICATIONS_ENABLED;
    static const QString KEY_NOTIFICATION_LEAD_TIME;

    std::vector<std::function<void(const QString&)>> languageChangeCallbacks;
    std::vector<std::function<void(const QString&)>> themeChangeCallbacks;

    SettingsManager() = default;
    ~SettingsManager() = default;

    void deleteAllData();
    void loadDefaults();

    SettingsManager(const SettingsManager&) = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;
};