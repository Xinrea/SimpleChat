#-------------------------------------------------
#
# Project created by QtCreator 2017-10-10T20:04:15
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleChat
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mfindpwd.cpp \
    stdafx.cpp \
    mlogin.cpp \
    mregister.cpp \
    mytcpsocket.cpp \
    myudpsocket.cpp \
    messagebox.cpp \
    registerwindow.cpp \
    findpwdwindow.cpp \
    mainprocwindow.cpp \
    mrequest.cpp \
    chatwindow.cpp \
    mfile.cpp

HEADERS += \
        mainwindow.h \
    mfindpwd.h \
    stdafx.h \
    mlogin.h \
    mregister.h \
    msgstruct.h \
    mytcpsocket.h \
    myudpsocket.h \
    messagebox.h \
    registerwindow.h \
    findpwdwindow.h \
    mainprocwindow.h \
    mrequest.h \
    chatwindow.h \
    mfile.h

LIBS += -lWs2_32

FORMS += \
        mainwindow.ui \
    messagebox.ui \
    registerwindow.ui \
    findpwdwindow.ui \
    mainprocwindow.ui \
    chatwindow.ui

DISTFILES += \
    config.ini \
    listView.qss

RESOURCES += \
    resource.qrc
