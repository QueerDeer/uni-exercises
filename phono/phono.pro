#-------------------------------------------------
#
# Project created by QtCreator 2016-12-05T03:00:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = phono
TEMPLATE = app

TRANSLATIONS += QtLanguage_ru.ts

SOURCES += main.cpp\
        mainwindow.cpp \
    item.cpp \
    artist.cpp \
    album.cpp \
    song.cpp \
    musicmodel.cpp \
    ratingdelegate.cpp \
    albumdialog.cpp \
    artistdialog.cpp \
    ratingeditor.cpp

HEADERS  += mainwindow.h \
    item.h \
    artist.h \
    album.h \
    song.h \
    musicmodel.h \
    ratingdelegate.h \
    albumdialog.h \
    artistdialog.h \
    ratingeditor.h

FORMS    += mainwindow.ui \
    albumdialog.ui \
    artistdialog.ui

DISTFILES += \
    QtLanguage_ru.ts
