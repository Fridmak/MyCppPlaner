#include "../../include/managers/tasks_manager.h"
#include <QUuid>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <algorithm>
#include <iostream>

const QString TasksManager::SETTINGS_ORG = "Tasks";
const QString TasksManager::SETTINGS_APP = "MyCppPlanner";
const QString TasksManager::KEY_TASKS = "tasks";
const QString TasksManager::KEY_CATEGORIES = "categories";

TasksManager& TasksManager::instance() {
    static TasksManager instance;
    return instance;
}

QString TasksManager::addTask(const Task& task) {
    Task newTask = task;
    newTask.id = generateTaskId();
    tasks.append(newTask);
    saveTasks();
    return newTask.id;
}

bool TasksManager::updateTask(const QString& taskId, const Task& task) {
    for (int i = 0; i < tasks.size(); ++i) {
        if (tasks[i].id == taskId) {
            Task updatedTask = task;
            updatedTask.id = taskId;
            tasks[i] = updatedTask;
            saveTasks();
            return true;
        }
    }
    return false;
}

bool TasksManager::deleteTask(const QString& taskId) {
    for (int i = 0; i < tasks.size(); ++i) {
        if (tasks[i].id == taskId) {
            tasks.removeAt(i);
            saveTasks();
            return true;
        }
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
    QList<Task> filteredTasks;
    for (const auto& task : tasks) {
        if (task.status == status) {
            filteredTasks.append(task);
        }
    }
    return filteredTasks;
}

QList<Task> TasksManager::getTasksByDeadline(const QDate& date, std::function<bool(const QDate&, const QDate&)> cond) const {
    QList<Task> filteredTasks;
    for (const auto& task : tasks) {
        if (cond(task.deadline.date(), date)) {
            filteredTasks.append(task);
        }
    }
    return filteredTasks;
}

QList<Task> TasksManager::getTasksBeforeDeadline(const QDate& date) const {
    auto lessThanOrEqualTo = [](const QDate& a, const QDate& b) { return a <= b; };
    return getTasksByDeadline(date, lessThanOrEqualTo);
}

QList<Task> TasksManager::getTasksAfterDeadline(const QDate& date) const {
    auto biggerThanOrEqualTo = [](const QDate& a, const QDate& b) { return a >= b; };
    return getTasksByDeadline(date, biggerThanOrEqualTo);
}

QStringList TasksManager::getCategories() const {
    return categories;
}

void TasksManager::addCategory(const QString& category) {
    if (!categories.contains(category)) {
        categories.append(category);
        QSettings settings(SETTINGS_ORG, SETTINGS_APP);
        settings.setValue(KEY_CATEGORIES, categories);
    }
}

void TasksManager::removeCategory(const QString& category) {
    if (categories.removeOne(category)) {
        QSettings settings(SETTINGS_ORG, SETTINGS_APP);
        settings.setValue(KEY_CATEGORIES, categories);
    }
}

void TasksManager::deleteAllData() {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.remove(KEY_TASKS);
    settings.remove(KEY_CATEGORIES);
    tasks.clear();
    categories.clear();
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
}

void TasksManager::loadTasks() {
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    tasks.clear();

    QByteArray data = settings.value(KEY_TASKS).toByteArray();
    if (!data.isEmpty()) {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isArray()) {
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