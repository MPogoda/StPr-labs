#-------------------------------------------------
#
# Project created by QtCreator 2011-10-02T20:03:48
#
#-------------------------------------------------

CONFIG += qwt
QT       += core gui

TARGET = sv1
TEMPLATE = app


SOURCES += main.cpp\
        mw.cpp \
    montecarlo.cpp

HEADERS  += mw.h \
    montecarlo.h

FORMS    += mw.ui \
    montecarlo.ui

INCLUDEPATH += "/usr/include/qwt6"
DEPENDPATH  += "/usr/include/qwt6"
LIBS        += -lqwt

