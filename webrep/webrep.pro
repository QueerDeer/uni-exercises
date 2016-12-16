#-------------------------------------------------
#
# Project created by QtCreator 2016-12-14T23:56:21
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = webrep
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    diagrammwidget.cpp \
    pluginfactory.cpp

HEADERS  += mainwindow.h \
    diagrammwidget.h \
    pluginfactory.h

FORMS    += mainwindow.ui

RESOURCES += \
    html.qrc
