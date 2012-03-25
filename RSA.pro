#-------------------------------------------------
#
# Project created by QtCreator 2012-03-01T12:14:03
#
#-------------------------------------------------

QT       += core gui

TARGET = RSA
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    bigprime.cpp

HEADERS  += mainwindow.h \
    bigprime.h

FORMS    += mainwindow.ui

unix|win32: LIBS += -lgmpxx
