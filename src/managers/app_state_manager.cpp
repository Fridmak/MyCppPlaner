#include "../../include/managers/app_state_manager.h"
#include <QSettings>

const QString StateManager::SETTINGS_ORG = "AppState";
const QString StateManager::SETTINGS_APP = "MyCppPlanner";
const QString StateManager::KEY_APP_STATE = "app_state";

StateManager& StateManager::instance() {
    static StateManager instance;
    return instance;
}

StateManager::StateManager() : currentState(AppState::Unauthorized) {
    loadState();
    saveState();
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
    if (lastError.size() > MAX_ERRORS) {
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

void StateManager::saveState() {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.setValue(KEY_APP_STATE, static_cast<int>(currentState));
}

void StateManager::loadState() {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    currentState = static_cast<AppState>(settings.value(KEY_APP_STATE, 
        static_cast<int>(AppState::Unauthorized)).toInt());
}

void StateManager::clearState() {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.remove(KEY_APP_STATE);
    currentState = AppState::Unauthorized;
    lastError.clear();
}