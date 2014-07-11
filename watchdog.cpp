/*
 * KWin-Comp-Forcer: Disable KWin compositing based on your secondary NVidia card.
 * Copyright (C) 2014 Alfred Neumayer
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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
