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

QString SettingsManager::getLanguage() const {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    return settings.value(KEY_LANGUAGE, "en").toString();
}

void SettingsManager::setLanguage(const QString& lang = "en") {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.setValue(KEY_LANGUAGE, lang);
}

QString SettingsManager::getTheme() const {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    return settings.value(KEY_THEME, "dark").toString(); // Dark theme start
}

void SettingsManager::setTheme(const QString& theme = "dark") {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.setValue(KEY_THEME, theme);
}

bool SettingsManager::areNotificationsEnabled() const {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    return settings.value(KEY_NOTIFICATIONS_ENABLED, true).toBool();
}

void SettingsManager::setNotificationsEnabled(bool enabled = true) {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.setValue(KEY_NOTIFICATIONS_ENABLED, enabled);
}

int SettingsManager::getNotificationLeadTime() const {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    return settings.value(KEY_NOTIFICATION_LEAD_TIME, 15).toInt();
}

void SettingsManager::setNotificationLeadTime(int minutes = 15) {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.setValue(KEY_NOTIFICATION_LEAD_TIME, minutes);
}

void SettingsManager::deleteAllData() {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.clear();
    loadDefaults();
}

void SettingsManager::loadDefaults() {
    setLanguage();
    setTheme();
    setNotificationsEnabled();
    setNotificationLeadTime();
}