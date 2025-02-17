#pragma once

#include <QString>
#include <QDateTime>
#include <QList>
#include <QSettings>

class Task {
public:
    enum class Priority { Low, Medium, High, Incredible };
    enum class Status { ToDo, InProgress, Done };

    QString id;
    QString title;
    QString description;
    QDateTime startTime;
    QDateTime deadline;
    Priority priority;
    Status status;
    QString category;

    bool hasNotification;
};

class TasksManager {
public:
    static TasksManager& instance();

    QString addTask(const Task& task);
    bool updateTask(const QString& taskId, const Task& task);
    bool deleteTask(const QString& taskId);
    Task getTask(const QString& taskId) const;
    
    QList<Task> getAllTasks() const;
    QList<Task> getTasksByStatus(Task::Status status) const;
    QList<Task> getTasksBeforeDeadline(const QDate& date) const;
    QList<Task> getTasksAfterDeadline(const QDate& date) const;
    
    QStringList getCategories() const;
    void addCategory(const QString& category);
    void removeCategory(const QString& category);

private:
    static const QString SETTINGS_ORG;
    static const QString SETTINGS_APP;
    static const QString KEY_TASKS;
    static const QString KEY_CATEGORIES;

    TasksManager() = default;
    ~TasksManager() = default;
    
    QList<Task> getTasksByDeadline(const QDate& date, std::function<bool(const QDate&, const QDate&)> cond) const;

    void deleteAllData();
    QString generateTaskId() const;
    void saveTasks() const;
    void loadTasks();

    QList<Task> tasks;
    QStringList categories;

    TasksManager(const TasksManager&) = delete;
    TasksManager& operator=(const TasksManager&) = delete;
};