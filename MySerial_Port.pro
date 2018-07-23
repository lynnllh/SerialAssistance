#-------------------------------------------------
#
# Project created by QtCreator 2016-11-18T08:14:33
#
#-------------------------------------------------

QT       += core gui
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MySerial_Port
TEMPLATE = app


SOURCES += main.cpp\
        mywidget.cpp \
    spcomm.cpp \
    disnumthread.cpp \
    qcustomplot.cpp

HEADERS  += mywidget.h \
    spcomm.h \
    disnumthread.h \
    qcustomplot.h

FORMS    += mywidget.ui

RC_FILE +=myserialport.rc

RESOURCES += \
    resource.qrc
