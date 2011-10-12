#-------------------------------------------------
#
# Project created by QtCreator 2011-10-02T20:03:48
#
#-------------------------------------------------

INCLUDEPATH += "/usr/include/qwt6/"
LIBS += "/usr/lib/libqwt6.so"

CONFIG += qwt
QT       += core gui

TARGET = sv1
TEMPLATE = app


SOURCES += main.cpp\
        mw.cpp \
    montecarlo.cpp \
    task4.cpp

HEADERS  += mw.h \
    montecarlo.h \
    task4.h

FORMS    += mw.ui \
    montecarlo.ui \
    task4.ui










