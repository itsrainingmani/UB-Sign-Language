#-------------------------------------------------
#
# Project created by QtCreator 2016-10-31T02:42:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui
TEMPLATE = app


SOURCES += main.cpp\
        leapgui.cpp

HEADERS  += leapgui.h

FORMS    += leapgui.ui

unix:!macx: LIBS += -L$$PWD/../lib/x64/ -lLeap

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include
