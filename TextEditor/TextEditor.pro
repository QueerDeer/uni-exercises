#-------------------------------------------------
#
# Project created by QtCreator 2016-10-12T01:10:03
#
#-------------------------------------------------

QT += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TextEditor
TEMPLATE = app


SOURCES += main.cpp\
        texteditor.cpp \
    gifanimationdemowidget.cpp

HEADERS  += texteditor.h \
    gifanimationdemowidget.h

FORMS    += texteditor.ui \
    gifanimationdemowidget.ui

RESOURCES += \
    icons.qrc \
    pusheen.qrc \
    creedence.qrc

RC_FILE = WindowsResource.rc
