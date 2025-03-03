#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QString>
#include <map>

struct task {
    QString content;
    int id;
    bool done;
};

class TaskManager : public QObject
{
    Q_OBJECT
public:
    explicit TaskManager(QObject *parent = nullptr);
    ~TaskManager();

    std::map<int, task*>* getTasks();

public slots:
    int addTask(QString content);
    void setTaskDone(int taskId, bool done);

signals:
    void taskAdded(task* t);
    void taskEdited(task* t);

private:
    int lastId;
    const char* fileName;
    std::map<int, task*> tasks;

    void readTasksFromFile();
    void updateFile();
};

#endif // TASKMANAGER_H
