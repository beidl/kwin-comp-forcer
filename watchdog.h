#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <QFile>
#include <QDebug>
#include <QThread>
#include <QtDBus>

class WatchDog : public QThread
{
    Q_OBJECT
public:
    explicit WatchDog(QObject *parent = 0, bool resume = false, int sleepTime = 5);

private:
    int sleepTime = 5;
    QFile *bbFile;
    bool compDisabled = false;
    QDBusMessage compSuspend;
    QDBusMessage compResume;

signals:

public slots:
    void runCheck();

};

#endif // WATCHDOG_H
