#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mgr = new Manager(ui->table);

    setupSettingsForm();
    setupSignalsSlots();
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::onSettingsButtonClicked()
{
    settingsForm->show();
}


void MainWindow::setupSignalsSlots()
{
    connect(ui->settingsButton, SIGNAL(clicked()), this, SLOT(onSettingsButtonClicked()));
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(onPlayButtonClicked()));

    connect(settingsForm, SIGNAL(algorithmChanged(int)), mgr, SLOT(algorithmChanged(int)));
    connect(settingsForm, SIGNAL(maxPriorityChanged(int)), mgr, SLOT(maxPriorityChanged(int)));
    connect(settingsForm, SIGNAL(maxDurationChanged(int)), mgr, SLOT(maxDurationChanged(int)));
    connect(settingsForm, SIGNAL(procNumberChanged(int)), mgr, SLOT(procNumberChanged(int)));

}


void MainWindow::setupSettingsForm()
{
    settingsForm = new SettingsForm();
    settingsForm->setWindowFlags(Qt::Dialog);
}


void MainWindow::onPlayButtonClicked()
{
    if (ui->playButton->text() == "Start")
        ui->playButton->setText("Reset");
    else
        ui->playButton->setText("Start");

    mgr->toggle();
}




