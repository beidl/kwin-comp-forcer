#-------------------------------------------------
#
# Project created by QtCreator 2014-07-11T03:58:48
#
#-------------------------------------------------

QT       += core
QT       += dbus
QT       -= gui

TARGET = kwin-comp-forcer
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -std=c++11 -O2

TEMPLATE = app


SOURCES += main.cpp \
    watchdog.cpp

HEADERS += \
    watchdog.h
