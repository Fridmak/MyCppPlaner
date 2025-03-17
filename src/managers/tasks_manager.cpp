#include "../../include/managers/tasks_manager.h"
#include <QUuid>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDate>
#include <algorithm>

const QString TasksManager::SETTINGS_ORG = "Tasks";
const QString TasksManager::SETTINGS_APP = "MyCppPlanner";
const QString TasksManager::KEY_TASKS = "tasks";
const QString TasksManager::KEY_CATEGORIES = "categories";

TasksManager& TasksManager::instance() {
    static TasksManager instance;
    return instance;
}

QString TasksManager::addTask(const Task& task) {
    QString taskId = generateTaskId();
    Task newTask = task;
    newTask.id = taskId;
    tasks.append(newTask);
    saveTasks();
    return taskId;
}

bool TasksManager::updateTask(const QString& taskId, const Task& task) {
    for (auto& t : tasks) {
        if (t.id == taskId) {
            t = task;
            t.id = taskId; // Ensure ID remains unchanged
            saveTasks();
            return true;
        }
    }
    return false;
}

bool TasksManager::deleteTask(const QString& taskId) {
    auto it = std::remove_if(tasks.begin(), tasks.end(), [&taskId](const Task& t) { return t.id == taskId; });
    if (it != tasks.end()) {
        tasks.erase(it, tasks.end());
        saveTasks();
        return true;
    }
    return false;
}

Task TasksManager::getTask(const QString& taskId) const {
    for (const auto& task : tasks) {
        if (task.id == taskId) {
            return task;
        }
    }
    return Task();
}

QList<Task> TasksManager::getAllTasks() const {
    return tasks;
}

QList<Task> TasksManager::getTasksByStatus(Task::Status status) const {
    QList<Task> result;
    for (const auto& task : tasks) {
        if (task.status == status) {
            result.append(task);
        }
    }
    return result;
}

QList<Task> TasksManager::getTasksBeforeDeadline(const QDate& date) const {
    return getTasksByDeadline(date, [](const QDate& deadline, const QDate& compareDate) {
        return deadline < compareDate;
        });
}

QList<Task> TasksManager::getTasksAfterDeadline(const QDate& date) const {
    return getTasksByDeadline(date, [](const QDate& deadline, const QDate& compareDate) {
        return deadline > compareDate;
        });
}

QList<Task> TasksManager::getTasksDueToday() const {
    return getTasksByDeadline(QDate::currentDate(), [](const QDate& deadline, const QDate& today) {
        return deadline == today;
        });
}

QList<Task> TasksManager::getTasksDueTomorrow() const {
    QDate tomorrow = QDate::currentDate().addDays(1);
    return getTasksByDeadline(tomorrow, [](const QDate& deadline, const QDate& compareDate) {
        return deadline == compareDate;
        });
}

QList<Task> TasksManager::getTasksDueThisWeek() const {
    QDate startOfWeek = QDate::currentDate().addDays(-QDate::currentDate().dayOfWeek() + 1);
    QDate endOfWeek = startOfWeek.addDays(6);
    QList<Task> result;
    for (const auto& task : tasks) {
        if (task.deadline.date() >= startOfWeek && task.deadline.date() <= endOfWeek) {
            result.append(task);
        }
    }
    return result;
}

QList<Task> TasksManager::getTasksByCategory(const QString& category) const {
    QList<Task> result;
    for (const auto& task : tasks) {
        if (task.category == category) {
            result.append(task);
        }
    }
    return result;
}

QList<Task> TasksManager::getTasksByPriority(Task::Priority priority) const {
    QList<Task> result;
    for (const auto& task : tasks) {
        if (task.priority == priority) {
            result.append(task);
        }
    }
    return result;
}

QStringList TasksManager::getCategories() const {
    return categories;
}

void TasksManager::addCategory(const QString& category) {
    if (!categories.contains(category)) {
        categories.append(category);
        saveTasks();
    }
}

void TasksManager::removeCategory(const QString& category) {
    categories.removeAll(category);
    for (auto& task : tasks) {
        if (task.category == category) {
            task.category.clear();
        }
    }
    saveTasks();
}

void TasksManager::setTaskNotificationsEnabled(const QString& taskId, bool enable) {
    for (auto& task : tasks) {
        if (task.id == taskId) {
            task.hasNotification = enable;
            saveTasks();
            break;
        }
    }
}

void TasksManager::resetToDefaults() {
    deleteAllData();
}

QList<Task> TasksManager::getTasksByDeadline(const QDate& date, std::function<bool(const QDate&, const QDate&)> cond) const {
    QList<Task> result;
    for (const auto& task : tasks) {
        if (cond(task.deadline.date(), date)) {
            result.append(task);
        }
    }
    return result;
}

void TasksManager::deleteAllData() {
    tasks.clear();
    categories.clear();
    saveTasks();
}

QString TasksManager::generateTaskId() const {
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

void TasksManager::saveTasks() const {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);

    QJsonArray tasksArray;
    for (const auto& task : tasks) {
        QJsonObject taskObj;
        taskObj["id"] = task.id;
        taskObj["title"] = task.title;
        taskObj["description"] = task.description;
        taskObj["startTime"] = task.startTime.toString(Qt::ISODate);
        taskObj["deadline"] = task.deadline.toString(Qt::ISODate);
        taskObj["priority"] = static_cast<int>(task.priority);
        taskObj["status"] = static_cast<int>(task.status);
        taskObj["category"] = task.category;
        taskObj["hasNotification"] = task.hasNotification;
        tasksArray.append(taskObj);
    }
    settings.setValue(KEY_TASKS, QJsonDocument(tasksArray).toJson(QJsonDocument::Compact));

    settings.setValue(KEY_CATEGORIES, categories);
}

void TasksManager::loadTasks() {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);

    tasks.clear();
    QByteArray tasksData = settings.value(KEY_TASKS).toByteArray();
    if (!tasksData.isEmpty()) {
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(tasksData, &parseError);
        if (parseError.error == QJsonParseError::NoError && doc.isArray()) {
            QJsonArray tasksArray = doc.array();
            for (const auto& taskValue : tasksArray) {
                QJsonObject taskObj = taskValue.toObject();
                Task task;
                task.id = taskObj["id"].toString();
                task.title = taskObj["title"].toString();
                task.description = taskObj["description"].toString();
                task.startTime = QDateTime::fromString(taskObj["startTime"].toString(), Qt::ISODate);
                task.deadline = QDateTime::fromString(taskObj["deadline"].toString(), Qt::ISODate);
                task.priority = static_cast<Task::Priority>(taskObj["priority"].toInt());
                task.status = static_cast<Task::Status>(taskObj["status"].toInt());
                task.category = taskObj["category"].toString();
                task.hasNotification = taskObj["hasNotification"].toBool();
                tasks.append(task);
            }
        }
    }

    categories = settings.value(KEY_CATEGORIES).toStringList();
}