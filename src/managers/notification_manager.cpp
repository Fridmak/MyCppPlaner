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
 
NotificationManager::NotificationManager() {
	loadNotifications();
}

void NotificationManager::addNotification(const QString& taskId, const QDateTime& time, const QString& message) {
    QMutexLocker locker(&mutex);
    QString newId = generateNotificationId();

    while (std::any_of(notifications.begin(), notifications.end(),
        [newId](const Notification& n) { return n.id == newId; })) {
        newId = generateNotificationId();
    }
    Notification notification;
	notification.id = newId;
    notification.taskId = taskId;
    notification.time = time;
    notification.isRead = false;
    notification.message = message;

    notifications.append(notification);
    saveNotifications(true);
}

void NotificationManager::markAsRead(const QString& notificationId) {
    QMutexLocker locker(&mutex);
    for (auto& notification : notifications) {
        if (notification.id == notificationId) {
            notification.isRead = true;
            break;
        }
    }
    needsSave = true;
}

void NotificationManager::deleteNotification(const QString& notificationId) {  
   QMutexLocker locker(&mutex);
   for (int i = 0; i < notifications.size(); ++i) {  
       if (notifications[i].id == notificationId) {  
           notifications.removeAt(i);  
           break;  
       }  
   }  
   needsSave = true;
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

int NotificationManager::getNumberOfUnreadNotifications() const {
    return std::count_if(notifications.begin(), notifications.end(),
        [](const Notification& n) { return !n.isRead; });
}


void NotificationManager::clearOldNotifications(const QDateTime& before) {
    QMutexLocker locker(&mutex);
    for (auto it = notifications.begin(); it != notifications.end(); ) {
        if (it->time < before) {
            it = notifications.erase(it);
        }
        else {
            ++it;
        }
    }
    needsSave = true;
}

void NotificationManager::clearAllNotifications() {
    QMutexLocker locker(&mutex);
    notifications.clear();
    needsSave = true;
}

void NotificationManager::deleteAllData() {
    QMutexLocker locker(&mutex);
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.remove(KEY_NOTIFICATIONS);
    settings.remove(KEY_LAST_CHECK);
    notifications.clear();
}

QString NotificationManager::generateNotificationId() const {
    QMutexLocker locker(&mutex);
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

void NotificationManager::saveNotifications(bool forceSave = false) {
    QMutexLocker locker(&mutex);
    if (!forceSave && !needsSave) {
        return;
    }

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
    QMutexLocker locker(&mutex);
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    notifications.clear();

    QByteArray data = settings.value(KEY_NOTIFICATIONS).toByteArray();
    if (!data.isEmpty()) {
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "Failed to parse notifications JSON:" << parseError.errorString();
            return;
        }

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