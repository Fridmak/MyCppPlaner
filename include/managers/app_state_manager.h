#pragma once

#include <QString>
#include <QList>

//Contains info about errors and app state

class StateManager {
public:
    static StateManager& instance();

    enum class AppState {
        Unauthorized,
        Authorized,
        Loading,
        Error,
        Banned
    };

    void setAppState(AppState state);
    AppState getCurrentState() const;

    void setError(const QString& error);
    QString getLastError() const;
    void clearErrors();

    bool isAuthorized() const;

    void clearState();

private:
    static const QString SETTINGS_ORG;
    static const QString SETTINGS_APP;
    static const QString KEY_APP_STATE;
	static const int MAX_ERRORS = 10;

    StateManager() = default;
    ~StateManager() = default;

    void saveState();
    void loadState();


    AppState currentState;
    QList<QString> lastError;

    StateManager(const StateManager&) = delete;
    StateManager& operator=(const StateManager&) = delete;
};