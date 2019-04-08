#-------------------------------------------------
#
# Project created by QtCreator 2019-04-02T15:08:56
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = scriptbuilder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    actiontreemodel.cpp \
    scripttreemodel.cpp \
    parameterslistmodel.cpp \
    pugixml.cpp

HEADERS  += mainwindow.h \
    actiontreemodel.h \
    scripttreemodel.h \
    parameterslistmodel.h \
    pugiconfig.hpp \
    pugixml.hpp

LIBS += -lstdc++fs

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
