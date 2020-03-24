#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>

namespace Ui {
class SettingsForm;
}

class SettingsForm : public QWidget
{
Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = nullptr);
    ~SettingsForm();

private:
    Ui::SettingsForm *ui;

    void setupSignalsSlots();

signals:
    void algorithmChanged(int val);
    void maxPriorityChanged(int val);
    void maxDurationChanged(int val);
    void procNumberChanged(int val);



};

#endif // SETTINGSFORM_H
