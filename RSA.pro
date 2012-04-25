#-------------------------------------------------
#
# Project created by QtCreator 2012-03-01T12:14:03
#-------------------------------------------------

QT       += core gui

TARGET = RSA
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    key.cpp \
    mycombobox.cpp \
    bpthread.cpp \
    bpethread.cpp \
    bigprime.cpp

HEADERS  += mainwindow.h \
    key.h \
    mycombobox.h \
    bpthread.h \
    bpethread.h \
    bigprime.h

FORMS    += mainwindow.ui

#unix|win32: LIBS += -lgmpxx

unix:!macx:!symbian: LIBS += -L$$PWD/ -lgmpxx

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/libgmpxx.a

unix:!macx:!symbian: LIBS += -L$$PWD/ -lgmp

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/libgmp.a
