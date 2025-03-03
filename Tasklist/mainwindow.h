#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QCheckBox>
#include <map>
#include <utility>
#include "taskmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAdd_clicked();
    void on_btnShowPending_clicked();
    void on_btnShowDone_clicked();

private:
    Ui::MainWindow *ui;
    QVBoxLayout *tasksLayout;
    QVBoxLayout *tasksFinishedLayout;
    TaskManager manager;
    std::map<int, std::pair<task*, QCheckBox*>> tasks;

    void on_manager_taskAdded(task* t);
    void on_manager_taskEdited(task* t);
    void on_checkBox_clicked();

};
#endif // MAINWINDOW_H
