#-------------------------------------------------
#
# Project created by QtCreator 2018-09-25T15:10:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = picaster
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
win32: DEFINES += TAGLIB_STATIC

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mediafile.cpp \
    optionsdialog.cpp \
    qlevelslider.cpp \
    buttons/basebutton.cpp \
    buttons/trackbutton.cpp \
    jack/jackclient.cpp \
    jack/jackfadermodule.cpp \
    jack/jackmodule.cpp \
    jack/jackport.cpp \
    jack/jackbutton.cpp \
    micbutton.cpp \
    signalbus.cpp \
    buttons/streambutton.cpp \
    buttons/recordbutton.cpp

HEADERS += \
    mainwindow.h \
    mediafile.h \
    optionsdialog.h \
    qlevelslider.h \
    buttons/basebutton.h \
    buttons/trackbutton.h \
    jack/jackclient.h \
    jack/jackfadermodule.h \
    jack/jackmodule.h \
    jack/jackport.h \
    jack/jackbutton.h \
    micbutton.h \
    signalbus.h \
    buttons/streambutton.h \
    buttons/recordbutton.h

FORMS += \
    mainwindow.ui \
    optionsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += taglib alsa jack

win32: LIBS += -L$$PWD/../opt/lib/ -ltag

win32: INCLUDEPATH += $$PWD/../opt/include
win32: INCLUDEPATH += $$PWD/../opt/include/taglib
win32: DEPENDPATH += $$PWD/../opt/include
win32: DEPENDPATH += $$PWD/../opt/include/taglib

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../opt/lib/tag.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../opt/lib/libtag.a
