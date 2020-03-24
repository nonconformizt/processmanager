#ifndef MANAGER_H
#define MANAGER_H
#include <QObject>
#include <QMessageBox>
#include <QTableWidget>
#include <QDateTime>
#include <QTimer>
#include <QDebug>


class Manager : public QObject
{

Q_OBJECT

class Process
{
public:
    ushort pid, pri, dur;
    bool isNew;
    int wastedTime, totalTime;
    Process(ushort pid, ushort pri, ushort dur)
    {
        this->pid = pid;
        this->pri = pri;
        this->dur = dur;
        isNew = true;
        wastedTime = totalTime = 0;
    }
};

private:
    static const int quant = 500;
    int algVariant = 1; // 1 is SRR, 2 is SJF
    int maxPriority = 10;
    int maxDuration = 20;
    int procNumber = 100;
    QTableWidget * table;

    ////////////////
    /// Runtime variables
    ////////////////
    QList<Process *> processes;
    Process * activeProc;
    bool isRunning;
    int tickCounter;
    int procCounter; // proccesses to be generated
    ////////////////

    ////////////////
    /// Statistics variables
    ////////////////
    double penaltyTotal;
    double reactivityTotal;
    double timeTotal;
    double wastedTotal;
    ////////////////

    void reset();
    void start();
    void finish();
    void activateProc(Process * proc);
    void deactivateProc(Process * proc);
    void deleteProc(Process * proc);
    void updateProc(Process * proc);
    void generateProc();
    Process * findGreatestPriority();
    Process * findShortestJob();


public:
    Manager(QTableWidget * table)
    {
        this->table = table;
        table->setHorizontalHeaderLabels({"PID", "Priority", "Duration"});
        qsrand(QDateTime::currentMSecsSinceEpoch() / 1000);
        reset();
    }

    void toggle(); // start or stop

private slots:
    void algorithmChanged(int alg) { this->algVariant = alg + 1; }
    void maxPriorityChanged(int pri) { this->maxPriority = pri; }
    void maxDurationChanged(int dur) { this->maxDuration = dur; }
    void procNumberChanged(int num) { this->procNumber = num; }

    void tick();

};




#endif // MANAGER_H
