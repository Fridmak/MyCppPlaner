#include "../../include/managers/settings_manager.h"

const QString SettingsManager::SETTINGS_ORG = "Settings";
const QString SettingsManager::SETTINGS_APP = "MyCppPlanner";
const QString SettingsManager::KEY_LANGUAGE = "language";
const QString SettingsManager::KEY_THEME = "theme";
const QString SettingsManager::KEY_NOTIFICATIONS_ENABLED = "notifications_enabled";
const QString SettingsManager::KEY_NOTIFICATION_LEAD_TIME = "notification_lead_time";

SettingsManager& SettingsManager::instance() {
    static SettingsManager instance;
    return instance;
}

void SettingsManager::onLanguageChanged(std::function<void(const QString&)> callback) {
    languageChangeCallbacks.push_back(callback);
}

void SettingsManager::onThemeChanged(std::function<void(const QString&)> callback) {
    themeChangeCallbacks.push_back(callback);
}

SettingsManager::SettingsManager() {
	QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    if (!settings.contains(KEY_LANGUAGE) || !settings.contains(KEY_THEME)) {
        loadDefaults();
    }
}

QString SettingsManager::getLanguage() const {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    return settings.value(KEY_LANGUAGE).toString();
}

void SettingsManager::setLanguage(const QString& lang) {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.setValue(KEY_LANGUAGE, lang);
    for (const auto& callback : languageChangeCallbacks) {
        callback(lang);
    }
}

QString SettingsManager::getTheme() const {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    return settings.value(KEY_THEME).toString();
}

void SettingsManager::setTheme(const QString& theme) {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.setValue(KEY_THEME, theme);
    for (const auto& callback : themeChangeCallbacks) {
        callback(theme);
    }
}

bool SettingsManager::areNotificationsEnabled() const {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    return settings.value(KEY_NOTIFICATIONS_ENABLED, true).toBool();
}

void SettingsManager::setNotificationsEnabled(bool enabled) {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.setValue(KEY_NOTIFICATIONS_ENABLED, enabled);
}

int SettingsManager::getNotificationLeadTime() const {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    return settings.value(KEY_NOTIFICATION_LEAD_TIME).toInt();
}

void SettingsManager::setNotificationLeadTime(int minutes) {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.setValue(KEY_NOTIFICATION_LEAD_TIME, minutes);
}

void SettingsManager::deleteAllData() {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.clear();
    loadDefaults();
}

void SettingsManager::loadDefaults() {
    setLanguage("en");
    setTheme("dark");
    setNotificationsEnabled(true);
    setNotificationLeadTime(15);
}

void SettingsManager::resetToDefaults() {
    deleteAllData();
    loadDefaults();
}