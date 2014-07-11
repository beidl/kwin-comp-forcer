#include "watchdog.h"
#include <iostream>

using namespace std;

WatchDog::WatchDog(QObject *parent, bool resume, int sleepTime) : QThread(parent)
{
    bbFile = new QFile(QString("/proc/acpi/bbswitch"));

    compSuspend = QDBusMessage::createMethodCall("org.kde.kwin",
                                                 "/Compositor",
                                                 "",
                                                 "suspend");

    compResume = QDBusMessage::createMethodCall("org.kde.kwin",
                                                 "/Compositor",
                                                 "",
                                                 "resume");

    connect(this, SIGNAL(started()), this, SLOT(runCheck()), Qt::DirectConnection);

    this->sleepTime = sleepTime;

    if(resume)
        QDBusConnection::sessionBus().call(compResume);
}

void WatchDog::runCheck() {
    bool run = true;
    while(run) {
        if(!bbFile->exists()) {
            cout << "bbswitch is not installed or running, exiting" << endl;
            run = false;
            exit();
        } else {
            QThread::sleep(sleepTime);

            bbFile->open(QFile::ReadOnly);
            QString stateStr = QString(bbFile->readAll());

            if(stateStr.contains(QString("ON")) && !compDisabled) {
                // disable Compositing
                QDBusConnection::sessionBus().call(compSuspend);
                compDisabled = true;
            } else if(stateStr.contains(QString("OFF")) && compDisabled) {
                // enable Compositing
                QDBusConnection::sessionBus().call(compResume);
                compDisabled = false;
            }
            bbFile->close();
        }
    }
}
