#-------------------------------------------------
#
# Project created by QtCreator 2018-11-24T17:49:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GasTraining
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++1z
#QMAKE_CXXFLAGS += -std=c++17
#QMAKE_CXXFLAGS += -std=gnu++1z

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    Data/model.cpp \
    View/mainview.cpp \
    Services/picketgenerator.cpp \
    View/picketsview.cpp \
    View/picketview.cpp

HEADERS += \
        mainwindow.h \
    Data/model.h \
    View/mainview.h \
    Services/picketgenerator.h \
    View/picketsview.h \
    View/picketview.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
