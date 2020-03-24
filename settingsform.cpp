#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);
    setupSignalsSlots();
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::setupSignalsSlots()
{
    connect(ui->typeOfAlgorithm, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index){ emit algorithmChanged(index); });
    connect(ui->maxPriority, QOverload<int>::of(&QSpinBox::valueChanged),
        [=](int index){ emit maxPriorityChanged(index); });
    connect(ui->maxDuration, QOverload<int>::of(&QSpinBox::valueChanged),
        [=](int index){ emit maxDurationChanged(index); });
    connect(ui->numberOfProcesses, QOverload<int>::of(&QSpinBox::valueChanged),
        [=](int index){ emit procNumberChanged(index); });
}
