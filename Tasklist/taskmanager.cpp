#include "taskmanager.h"
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

TaskManager::TaskManager(QObject *parent)
    : QObject{parent}
    , lastId(-1)
    , fileName("tasks.csv")
{
    readTasksFromFile();
}

TaskManager::~TaskManager() {
    for(const auto& [id, t] : tasks) {
        delete t;
    }
}

int TaskManager::addTask(QString content) {
    std::ofstream file(fileName, std::ios::app);
    std::string c = (content.contains(',')? ('"' + content.toStdString() + '"') : content.toStdString());

    // saving data
    file << ++lastId << "," << c << ",0\n";
    task* t = new task{ content, lastId, false };
    tasks[lastId] = t;

    file.close();
    emit taskAdded(t);
    return lastId;
}

void TaskManager::setTaskDone(int taskId, bool done) {
    if(!tasks.count(taskId))
        return;

    task* t = tasks[taskId];
    t->done = done;
    updateFile();
    emit taskEdited(t);
}

std::map<int, task*>* TaskManager::getTasks() { return &tasks; }

void TaskManager::readTasksFromFile() {
    // read all tasks in the file
    std::string line;
    std::vector<std::string> tokens;
    task* ptask = nullptr;
    std::ifstream file(fileName);

    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token, buffer;

        while (std::getline(ss, token, ',')) {
            if(token.front() == '\"') {
                token.erase(0, 1);
                buffer = token;
                continue;
            }

            if(buffer.empty()) {
                tokens.push_back(token);
                continue;
            }

            buffer += "," + token;

            if (token.back() != '\"')
                continue;

            buffer.pop_back();
            tokens.push_back(buffer);
            buffer.clear();
        }
    }

    // generate structs from tokens
    int count = 0;

    for(const auto& v : tokens) {
        switch(count++) {
        case 0:
            ptask = new task;
            ptask->id = std::stoi(v);
            break;

        case 1:
            ptask->content = v.c_str();
            break;

        case 2:
            ptask->done = v == "1";
            tasks[ptask->id] = ptask;
            count = 0;
            break;
        }
    }

    file.close();
    if(ptask)
        lastId = ptask->id;
}

void TaskManager::updateFile() {
    std::ofstream f(fileName, std::ios::trunc);
    std::string c;

    for(const auto& p : tasks) {
        task* ptask = p.second;
        c = (ptask->content.contains(',')? ('"' + ptask->content.toStdString() + '"') : ptask->content.toStdString());
        f << ptask->id << "," << c << "," << (ptask->done? "1" : "0") << std::endl;
    }

    f.close();
}
