#include "../../include/managers/notification_manager.h"
#include <QUuid>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

const QString NotificationManager::SETTINGS_ORG = "Notifications";
const QString NotificationManager::SETTINGS_APP = "MyCppPlanner";
const QString NotificationManager::KEY_NOTIFICATIONS = "notifications";
const QString NotificationManager::KEY_LAST_CHECK = "last_notification_check";

NotificationManager& NotificationManager::instance() {
    static NotificationManager instance;
    return instance;
}

void NotificationManager::addNotification(const QString& taskId, const QDateTime& time, const QString& message) {
    Notification notification;
    notification.id = generateNotificationId();
    notification.taskId = taskId;
    notification.time = time;
    notification.isRead = false;
    notification.message = message;

    notifications.append(notification);
    saveNotifications();
}

void NotificationManager::markAsRead(const QString& notificationId) {
    for (auto& notification : notifications) {
        if (notification.id == notificationId) {
            notification.isRead = true;
            break;
        }
    }
    saveNotifications();
}

void NotificationManager::deleteNotification(const QString& notificationId) {
    for (int i = 0; i < notifications.size(); ++i) {
        if (notifications[i].id == notificationId) {
            notifications.removeAt(i);
            break;
        }
    }
    saveNotifications();
}

QList<Notification> NotificationManager::getPendingNotifications() const {
    QList<Notification> pending;
    QDateTime now = QDateTime::currentDateTime();
    
    for (const auto& notification : notifications) {
        if (!notification.isRead && notification.time <= now) {
            pending.append(notification);
        }
    }
    return pending;
}

QList<Notification> NotificationManager::getNotificationsForTask(const QString& taskId) const {
    QList<Notification> taskNotifications;
    for (const auto& notification : notifications) {
        if (notification.taskId == taskId) {
            taskNotifications.append(notification);
        }
    }
    return taskNotifications;
}

bool NotificationManager::numberOfUnreadNotifications() const {
    int count = 0;
    for (const auto& notification : notifications) {
        if (!notification.isRead) {
            count++;
        }
    }
    return count > 0;
}

void NotificationManager::clearOldNotifications(const QDateTime& before) {
    notifications.erase(
        std::remove_if(notifications.begin(), notifications.end(),
            [&before](const Notification& n) { return n.time < before; }),
        notifications.end());
    saveNotifications();
}

void NotificationManager::clearAllNotifications() {
    notifications.clear();
    saveNotifications();
}

void NotificationManager::deleteAllData() {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.remove(KEY_NOTIFICATIONS);
    settings.remove(KEY_LAST_CHECK);
    notifications.clear();
}

QString NotificationManager::generateNotificationId() const {
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

void NotificationManager::saveNotifications() const {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    QJsonArray notificationsArray;

    for (const auto& notification : notifications) {
        QJsonObject notificationObj;
        notificationObj["id"] = notification.id;
        notificationObj["taskId"] = notification.taskId;
        notificationObj["time"] = notification.time.toString(Qt::ISODate);
        notificationObj["isRead"] = notification.isRead;
        notificationObj["message"] = notification.message;
        notificationsArray.append(notificationObj);
    }

    settings.setValue(KEY_NOTIFICATIONS, QJsonDocument(notificationsArray).toJson(QJsonDocument::Compact));
}

void NotificationManager::loadNotifications() {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    notifications.clear();

    QByteArray data = settings.value(KEY_NOTIFICATIONS).toByteArray();
    if (!data.isEmpty()) {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isArray()) {
            QJsonArray notificationsArray = doc.array();
            for (const auto& notificationValue : notificationsArray) {
                QJsonObject notificationObj = notificationValue.toObject();
                Notification notification;
                notification.id = notificationObj["id"].toString();
                notification.taskId = notificationObj["taskId"].toString();
                notification.time = QDateTime::fromString(notificationObj["time"].toString(), Qt::ISODate);
                notification.isRead = notificationObj["isRead"].toBool();
                notification.message = notificationObj["message"].toString();
                notifications.append(notification);
            }
        }
    }
}