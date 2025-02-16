#pragma once

#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QString>
#include <QByteArray>

class PasswordManager {
public:
	static PasswordManager& instance();

	void saveUserInfo(const QString& username, const QString& password, bool rememberMe);
	void deleteUserInfo(const QString& username, const QString& password);

	bool hasStoredInfo() const;

	QString getStoredUsername() const; //Don't forget to check via hasStoredInfo()!!!
	QString getStoredPassword() const;

	bool checkPassword(const QString& inputPassword, const QString& storedPassword);

private:
	//Encrypting settings
	static constexpr int SALT_LENGTH = 16;
	static const QString HASH_SEPARATOR;

	// Setting keys
	static const QString SETTINGS_ORG;
	static const QString SETTINGS_APP;
	static const QString KEY_USERNAME;
	static const QString KEY_PASSWORD;
	static const QString KEY_REMEMBER_ME;

	PasswordManager() = default;
	~PasswordManager() = default;

	void deleteAllData();
	QString generateSalt(int length);
	QString hashPassword(const QString& password, const QString& salt);
	QString encryptPassword(const QString& password);


	PasswordManager(const PasswordManager&) = delete;
	PasswordManager& operator=(const PasswordManager&) = delete; //Singleton pattern
};