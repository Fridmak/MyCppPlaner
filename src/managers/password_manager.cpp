#include "../include/managers/password_manager.h"
#include <QCryptographicHash>
#include <QSettings>
#include <QString>
#include <QByteArray>
#include <QRandomGenerator>

//Setting const values

const QString PasswordManager::HASH_SEPARATOR = ":";

const QString PasswordManager::SETTINGS_ORG = "LoginData";

const QString PasswordManager::SETTINGS_APP = "MyCppPlanner";

const QString PasswordManager::KEY_USERNAME = "username";

const QString PasswordManager::KEY_PASSWORD = "password";

const QString PasswordManager::KEY_REMEMBER_ME = "rememberMe";

// Done

// Private help methods

QString PasswordManager::generateSalt(int length) {
    QByteArray salt(length, 0);
    QRandomGenerator* generator = QRandomGenerator::global();
    for (int i = 0; i < length; ++i) {
        salt[i] = static_cast<char>(generator->generate() & 0xFF);
    }

    return QString(salt.toHex());
}

QString PasswordManager::hashPassword(const QString& password, const QString& salt) {
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8() + salt.toUtf8(), QCryptographicHash::Sha256);
    return QString(hashedPassword.toHex());
}

QString PasswordManager::encryptPassword(const QString& password) {
    QString salt = generateSalt(SALT_LENGTH);
    QString hashedPassword = hashPassword(password, salt);
    return salt + HASH_SEPARATOR + hashedPassword;
} 

// End private help methods here <0_->

PasswordManager& PasswordManager::instance() {
    static PasswordManager instance;
    return instance;
}

void PasswordManager::deleteAllData() {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.clear();
}

void PasswordManager::saveUserInfo(const QString& username, const QString& password, bool rememberMe) {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    if (rememberMe) {
        settings.setValue(KEY_USERNAME, username);

        settings.setValue(KEY_PASSWORD, encryptPassword(password));
        settings.setValue(KEY_REMEMBER_ME, true);
    }
    else {
        deleteUserInfo(username, password);
    }
}

bool PasswordManager::checkPassword(const QString& inputPassword, const QString& storedPassword) {
    QStringList parts = storedPassword.split(":");
    if (parts.size() != 2) {
        return false;
    }

    QString salt = parts[0];
    QString storedHashedPassword = parts[1];

    QString hashedInputPassword = hashPassword(inputPassword, salt);
    return hashedInputPassword == storedHashedPassword;
}

void PasswordManager::deleteUserInfo(const QString& username, const QString& password) {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);

    settings.remove(KEY_USERNAME);
    settings.remove(KEY_PASSWORD);
    settings.remove(KEY_REMEMBER_ME);
}

bool PasswordManager::hasStoredInfo() const {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    return settings.value(KEY_REMEMBER_ME, false).toBool();
}

QString PasswordManager::getStoredUsername() const {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    return settings.value(KEY_USERNAME).toString();
}

QString PasswordManager::getStoredPassword() const {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    return settings.value(KEY_PASSWORD).toString();
}