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
    qInfo() << "Algorithm: " << algVariant;

    reset();
    isRunning = true;
    procCounter = procNumber;
    generateProc();
    activeProc = processes[0];
    activateProc(processes[0]);
    tick();
}


void Manager::tick()
{

    if (!isRunning)
        return;

    tickCounter++;

    if (procCounter > 0 && (tickCounter % (maxDuration / 5) == 0)) // weird random
        generateProc();

    if (procCounter == 0 && processes.size() == 0) {
        finish();
        return;
    }

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

        if (processes.size() != 0)
            proc = findGreatestPriority();
        break;

    case 2: // SJF

        if (processes.size() == 0) break;

        if (activeProc->dur <= 0) {
            deleteProc(activeProc);
            if (processes.size() != 0)
                proc = findShortestJob();
            else break;
        } else
            proc = activeProc;


        break;

    }

    if (processes.size() != 0 && proc != nullptr) {
        activateProc(proc);
        updateProc(proc);
        proc->dur--;
    }


    // collect statistics
    for (Process * p : processes)
    {
        p->totalTime++;
        if (p != activeProc)
            p->wastedTime++;
    }


    if (procCounter == 0 && processes.size() == 0)
        finish();
    else
        QTimer::singleShot(quant, this, SLOT(tick()));
}


void Manager::reset()
{
    isRunning = false;
    tickCounter = penaltyTotal = reactivityTotal = timeTotal = wastedTotal = 0;

    while(processes.size() != 0)
        delete(processes.takeAt(0));
    while(table->rowCount() != 0)
        table->removeRow(0);
}


void Manager::finish()
{
    QMessageBox msgBox;
    msgBox.setText(
                "Потерянное время: " +
                QString::number(wastedTotal / procNumber) + "\n" +
                "Время пребывания процесса в системе: " +
                QString::number(timeTotal / procNumber) + "\n" +
                "Отношение реактивности: " +
                QString::number(reactivityTotal / procNumber) + "\n" +
                "Штрафное отношение: " +
                QString::number(penaltyTotal / procNumber) + "\n"
            );
    msgBox.exec();
}


void Manager::activateProc(Process *proc)
{
    if (proc == nullptr) return;

    if (proc != activeProc)
        deactivateProc(activeProc);
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
    if (!proc) return;

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
    // count stats
    reactivityTotal += ((double) proc->wastedTime / proc->totalTime);
    if (proc->wastedTime != 0)
        penaltyTotal += ((double) proc->totalTime / proc->wastedTime);
    timeTotal += proc->totalTime;
    wastedTotal += proc->wastedTime;

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
    if (proc == nullptr) return;

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
    int minDur = maxDuration + 1;
    Process * tmp = processes[0];

    for (auto curr : processes)
    {
        if (curr->dur <= minDur)
        {
            tmp = curr;
            minDur = curr->dur;
        }
    }

    qInfo() << "Shortest job: " << tmp->dur;

    return tmp;
}



