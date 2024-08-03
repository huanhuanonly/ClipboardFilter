QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    maindialog.cpp

HEADERS += \
    StringReplaceListView.h \
    Titlebar.h \
    hotkey.h \
    maindialog.h

FORMS += \
    maindialog.ui

TRANSLATIONS += \
    ClipboardFilter_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = "ClipboardFilter"
VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "YangHuanhuan"
QMAKE_TARGET_PRODUCT = "ClipboardFilter"
QMAKE_TARGET_DESCRIPTION = "ClipboardFilter"
QMAKE_TARGET_COPYRIGHT = "Copyright (c) 2023-2023 YangHuanhuan 3347484963@qq.com.  All rights reserved."
RC_LANG = 0x0004

RESOURCES += \
    res.qrc
