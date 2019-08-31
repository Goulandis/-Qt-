#-------------------------------------------------
#
# Project created by QtCreator 2019-07-05T20:10:03
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartHome
TEMPLATE = app


SOURCES += main.cpp\
        loginwindow.cpp \
    registerwindow.cpp \
    userinformation.cpp \
    tcpsocket.cpp \
    serveraddressdialog.cpp \
    smartprotocol.cpp \
    majorwindow.cpp \
    roomwindow.cpp \
    delay.cpp \
    forgetpwwindow.cpp \
    authcordwindow.cpp \
    widget.cpp \
    mediawindow.cpp

HEADERS  += loginwindow.h \
    registerwindow.h \
    userinformation.h \
    tcpsocket.h \
    serveraddressdialog.h \
    smartprotocol.h \
    majorwindow.h \
    roomwindow.h \
    delay.h \
    forgetpwwindow.h \
    authcordwindow.h \
    widget.h \
    mediawindow.h

FORMS    += loginwindow.ui \
    registerwindow.ui \
    serveraddressdialog.ui \
    majorwindow.ui \
    roomwindow.ui \
    forgetpwwindow.ui \
    authcordwindow.ui \
    widget.ui \
    mediawindow.ui

RESOURCES += \
    resources.qrc
