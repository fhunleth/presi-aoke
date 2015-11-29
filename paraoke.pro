#-------------------------------------------------
#
# Project created by QtCreator 2015-11-28T19:26:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = paraoke
TEMPLATE = app


SOURCES += main.cpp\
    PresentationWidget.cpp \
    SettingsDialog.cpp \
    Settings.cpp \
    SlideLoader.cpp

HEADERS  += \
    PresentationWidget.h \
    SettingsDialog.h \
    Settings.h \
    SlideLoader.h

FORMS    += \
    SettingsDialog.ui

DISTFILES += \
    TODO.md
