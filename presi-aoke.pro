#-------------------------------------------------
#
# Project created by QtCreator 2015-11-28T19:26:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = presi-aoke
TEMPLATE = app


SOURCES += src/main.cpp\
    src/PresentationWidget.cpp \
    src/SettingsDialog.cpp \
    src/Settings.cpp \
    src/SlideLoader.cpp

HEADERS  += \
    src/PresentationWidget.h \
    src/SettingsDialog.h \
    src/Settings.h \
    src/SlideLoader.h

FORMS    += \
    src/SettingsDialog.ui

DISTFILES += \
    deploy_linux.sh \
    deploy_osx.sh \
    deploy_win32.bat \
    presi-aoke.nsi \
    .travis.yml

unix {
    target.path = /usr/local/bin
    INSTALLS += target
}

RESOURCES += \
    ui.qrc

