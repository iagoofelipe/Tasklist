#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Tasklist");
    tasksLayout = new QVBoxLayout(ui->widTasks);
    tasksFinishedLayout = new QVBoxLayout(ui->widTasksFinished);

    // connecting events
    connect(&manager, &TaskManager::taskAdded, this, &MainWindow::on_manager_taskAdded);
    connect(&manager, &TaskManager::taskEdited, this, &MainWindow::on_manager_taskEdited);

    // setting contents
    for(const auto& [id, t] : *manager.getTasks())
        on_manager_taskAdded(t);
}

MainWindow::~MainWindow()
{
    delete tasksLayout;
    delete tasksFinishedLayout;
    delete ui;
}


void MainWindow::on_btnAdd_clicked()
{
    // get the content and save
    QString content = ui->lineEdit->text();
    ui->lineEdit->clear();
    manager.addTask(content);
}

void MainWindow::on_manager_taskAdded(task* t) {
    QCheckBox *check = new QCheckBox(t->content, ui->widTasks);
    check->setProperty("id", t->id);

    if(t->done) {
        auto font = check->font();
        tasksFinishedLayout->addWidget(check);
        check->setChecked(true);
        font.setStrikeOut(true);
        check->setFont(font);

    } else {
        tasksLayout->addWidget(check);
    }

    QObject::connect(check, &QCheckBox::clicked, this, &MainWindow::on_checkBox_clicked);
    tasks[t->id] = std::pair<task*, QCheckBox*>{t, check};
}

void MainWindow::on_manager_taskEdited(task* t) {
    auto wid = tasks[t->id].second;

    if(t->done) {
        tasksLayout->removeWidget(wid);
        tasksFinishedLayout->addWidget(wid);

    } else {
        tasksFinishedLayout->removeWidget(wid);
        tasksLayout->addWidget(wid);
    }

    auto font = wid->font();
    font.setStrikeOut(t->done);
    wid->setFont(font);
}

void MainWindow::on_checkBox_clicked() {
    QObject* check = sender();
    int id = check->property("id").toInt();

    manager.setTaskDone(id, tasks[id].second->isChecked());
}

void MainWindow::on_btnShowPending_clicked() {
    ui->scrollAreaTasks->setHidden(ui->scrollAreaTasks->isVisible());
}


void MainWindow::on_btnShowDone_clicked() {
    ui->scrollAreaTasksFinished->setHidden(ui->scrollAreaTasksFinished->isVisible());
}

