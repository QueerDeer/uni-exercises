#-------------------------------------------------
#
# Project created by QtCreator 2017-06-08T01:04:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GAUSS_MAPREDUCE
TEMPLATE = app

TRANSLATIONS += $${TARGET}_ru.ts

SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

DISTFILES += \
    GAUSS_MAPREDUCE_ru.ts
