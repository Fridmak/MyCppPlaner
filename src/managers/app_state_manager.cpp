#include "../../include/managers/app_state_manager.h"
#include <QSettings>

const QString StateManager::SETTINGS_ORG = "AppState";
const QString StateManager::SETTINGS_APP = "MyCppPlanner";
const QString StateManager::KEY_APP_STATE = "app_state";

StateManager& StateManager::instance() {
    static StateManager instance;
    return instance;
}

void StateManager::setAppState(AppState state) {
    currentState = state;
    saveState();
}

StateManager::AppState StateManager::getCurrentState() const {
    return currentState;
}

void StateManager::setError(const QString& error) {
    lastError.append(error);
    if (lastError.size() > 10) { // Only 10 errors saved
        lastError.removeFirst();
    }
    setAppState(AppState::Error);
}

QString StateManager::getLastError() const {
    return lastError.isEmpty() ? QString() : lastError.last();
}

void StateManager::clearErrors() {
    lastError.clear();
}

bool StateManager::isAuthorized() const {
    return currentState == AppState::Authorized;
}

void StateManager::setAuthorized(bool auth) {
    setAppState(auth ? AppState::Authorized : AppState::Unauthorized);
}

void StateManager::saveState() {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.setValue(KEY_APP_STATE, static_cast<int>(currentState));
}

void StateManager::loadState() {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    currentState = static_cast<AppState>(settings.value(KEY_APP_STATE, 
        static_cast<int>(AppState::Unauthorized)).toInt());
}