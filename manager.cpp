#include "manager.h"


void Manager::toggle()
{
    if (isRunning)
        reset();
    else
        start();
}


void Manager::start()
{
    reset();
    isRunning = true;
    procCounter = procNumber;
    generateProc();
    activateProc(processes[0]);
    tick();
}


void Manager::tick()
{
    tickCounter++;

    if (tickCounter % (maxDuration / 3) == 0) // weird random
        generateProc();

    // maintain processes

    Process * proc;

    switch (algVariant)
    {
    case 1: // SRR

        if (processes.size() == 0) break;

        // recount priorities
        for (auto proc : processes)
        {
            if (proc->isNew)
                proc->pri += 2;
            else
                proc->pri += 1;
            updateProc(proc);
        }

        if (activeProc->dur <= 0)
            deleteProc(activeProc);

        proc = findGreatestPriority();
        break;

    case 2: // SJF

        if (processes.size() == 0) break;

        if (activeProc->dur <= 0) {
            deleteProc(activeProc);
            proc = findShortestJob();
        } else
            proc = activeProc;


        break;

    }

    activateProc(proc);
    updateProc(proc);
    proc->dur--;


    if (procCounter > 0)
        QTimer::singleShot(quant, this, SLOT(tick()));
}


void Manager::reset()
{
    isRunning = false;
    tickCounter = 0;
    while(processes.size() != 0)
        delete(processes.takeAt(0));
    while(table->rowCount() != 0)
        table->removeRow(0);

}

void Manager::activateProc(Process *proc)
{
    if (proc != activeProc) deactivateProc(activeProc);
    activeProc = proc;
    activeProc->isNew = false;

    // find proc in table by PID
    QTableWidgetItem * item;
    bool found = false;
    int rowIdx = 0;
    for (; rowIdx < table->rowCount(); rowIdx++) {
        item = table->item(rowIdx, 0);
        if (item->text() == QString::number(proc->pid))
            { found = true; break; }
    }

    if (found)
    {
        table->item(rowIdx, 0)->setBackgroundColor(Qt::green);
        table->item(rowIdx, 1)->setBackgroundColor(Qt::green);
        table->item(rowIdx, 2)->setBackgroundColor(Qt::green);
    }
}


void Manager::deactivateProc(Process *proc)
{
    activeProc = nullptr;

    // find proc in table by PID
    QTableWidgetItem * item;
    bool found = false;
    int rowIdx = 0;
    for (; rowIdx < table->rowCount(); rowIdx++) {
        item = table->item(rowIdx, 0);
        if (item->text() == QString::number(proc->pid))
            { found = true; break; }
    }

    if (found)
    {
        table->item(rowIdx, 0)->setBackgroundColor(Qt::transparent);
        table->item(rowIdx, 1)->setBackgroundColor(Qt::transparent);
        table->item(rowIdx, 2)->setBackgroundColor(Qt::transparent);
    }
}


void Manager::deleteProc(Process *proc)
{
    // find proc in table by PID
    QTableWidgetItem * item;
    bool found = false;
    int rowIdx = 0;
    for (; rowIdx < table->rowCount(); rowIdx++) {
        item = table->item(rowIdx, 0);
        if (item->text() == QString::number(proc->pid))
            { found = true; break; }
    }

    if (found)
        table->removeRow(rowIdx);

    processes.removeOne(proc);
    delete proc;
}


void Manager::updateProc(Process *proc)
{
    // find proc in table by PID
    QTableWidgetItem * item;
    bool found = false;
    int rowIdx = 0;
    for (; rowIdx < table->rowCount(); rowIdx++) {
        item = table->item(rowIdx, 0);
        if (item->text() == QString::number(proc->pid))
            { found = true; break; }
    }

    if (found)
    {
        table->item(rowIdx, 0)->setText(QString::number(proc->pid));
        table->item(rowIdx, 1)->setText(QString::number(proc->pri));
        table->item(rowIdx, 2)->setText(QString::number(proc->dur));
    }
}


void Manager::generateProc()
{
    int pri;
    if (algVariant == 1)
        pri = 0;
    else
        pri = qrand() % maxPriority;

    Process * proc = new Process(procNumber - procCounter, pri, qrand() % maxDuration + 1);
    processes.push_front(proc);

    table->insertRow(0);
    table->setItem(0, 0, new QTableWidgetItem(QString::number(proc->pid)));
    table->setItem(0, 1, new QTableWidgetItem(QString::number(proc->pri)));
    table->setItem(0, 2, new QTableWidgetItem(QString::number(proc->dur)));

    procCounter--;

    qInfo() << "Process generated PID=" << proc->pid;
}


Manager::Process * Manager::findGreatestPriority()
{
    int maxpri = -1;
    Process * tmp = processes[0];

    for (auto curr : processes)
    {
        if (curr->pri >= maxpri)
        {
            tmp = curr;
            maxpri = curr->pri;
        }
    }

    return tmp;
}


Manager::Process * Manager::findShortestJob()
{
    int maxpri = -1;
    Process * tmp = processes[0];

    for (auto curr : processes)
    {
        if (curr->pri >= maxpri)
        {
            tmp = curr;
            maxpri = curr->pri;
        }
    }

    return tmp;
}



