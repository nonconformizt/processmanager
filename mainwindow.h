#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingsform.h"
#include "manager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SettingsForm * settingsForm;
    Manager * mgr;

    void setupSignalsSlots();
    void setupSettingsForm();


private slots:
    void onSettingsButtonClicked();
    void onPlayButtonClicked();



};
#endif // MAINWINDOW_H
