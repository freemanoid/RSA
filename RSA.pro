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
    bigprime.cpp \
    key.cpp \
    crypt.cpp

HEADERS  += mainwindow.h \
    bigprime.h \
    key.h \
    crypt.h

FORMS    += mainwindow.ui

unix|win32: LIBS += -lgmpxx
