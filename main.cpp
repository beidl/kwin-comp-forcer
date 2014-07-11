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

#include <QCoreApplication>
#include <iostream>
#include <QThread>
#include "watchdog.h"

using namespace std;

void printHelp(char *name) {
    cout << "usage: " << name << " [options]" << endl << endl;
    cout << "options:" << endl;
    cout << "\t-h:\t\t\tThis help" << endl;
    cout << "\t-r:\t\t\tReset KWin compositing on start" << endl;
    cout << "\t-t TIME:\t\tSet polling interval to TIME in seconds (default: 5)" << endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    bool resume = false; //force resume on start
    int sleepTime = 5;

    if(argc > 4 || (argc == 2 && strcmp(argv[1], "-h") == 0)) {
        printHelp(argv[0]);
        exit(0);
    }

    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], "-r") == 0) {
            resume = true;
        }

        if(strcmp(argv[i], "-t") == 0) {
            if (i+1 < argc) {
                try {
                    sleepTime = stoi(argv[i+1]);
                    if(sleepTime < 1) {
                        sleepTime = 5;
                        cout << "sleep time below 1, defaulting to 5" << endl;
                    }
                } catch (invalid_argument) {
                    sleepTime = 5;
                    cout << "invalid sleep time, defaulting to 5" << endl;
                }
            } else {
                sleepTime = 5;
                cout << "missing time, defaulting to 5" << endl;
            }
        }
    }

    WatchDog *wd = new WatchDog(0, resume, sleepTime);
    QObject::connect(wd, SIGNAL(finished()), &a, SLOT(quit()));
    wd->start(QThread::LowestPriority);

    return a.exec();
}
