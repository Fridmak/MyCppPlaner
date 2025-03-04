#pragma once

#include <QString>
#include <QDateTime>
#include <QList>
#include <QSettings>

struct Notification {
    QString id;
    QString taskId;
    QDateTime time;
    bool isRead;
    QString message;
};

class NotificationManager {
public:
    static NotificationManager& instance();

    void addNotification(const QString& taskId, const QDateTime& time, const QString& message);
    void markAsRead(const QString& notificationId);
    void deleteNotification(const QString& notificationId);
    
    QList<Notification> getPendingNotifications() const;
    QList<Notification> getNotificationsForTask(const QString& taskId) const;
    bool numberOfUnreadNotifications() const;
    
    void clearOldNotifications(const QDateTime& before);
    void clearAllNotifications();

private:
    static const QString SETTINGS_ORG;
    static const QString SETTINGS_APP;
    static const QString KEY_NOTIFICATIONS;
    static const QString KEY_LAST_CHECK;

    NotificationManager() = default;
    ~NotificationManager() = default;

    void deleteAllData();
    QString generateNotificationId() const;
    void saveNotifications() const;
    void loadNotifications();

    QList<Notification> notifications;

    NotificationManager(const NotificationManager&) = delete;
    NotificationManager& operator=(const NotificationManager&) = delete;
};