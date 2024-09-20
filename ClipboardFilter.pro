QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/Logger.cpp \
    src/TextReplacer.cpp \
    src/VariableParser.cpp \
    src/main.cpp \
    src/maindialog.cpp

INCLUDEPATH += include

HEADERS += \
    include/Logger.h \
    include/StatusBarLabel.h \
    include/TextEditWithVariables.h \
    include/TextReplaceRuleListView.h \
    include/TextReplacer.h \
    include/Titlebar.h \
    include/Trie.h \
    include/VariableParser.h \
    include/VariablesShader.h \
    include/maindialog.h

FORMS += \
    data/Ui/maindialog.ui

TRANSLATIONS += \
    data/Ts/ClipboardFilter_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = "ClipboardFilter"

VERSION = 1.1.0
DEFINES += __VERSION__=1.1.0

QMAKE_TARGET_COMPANY = "YangHuanhuan"
QMAKE_TARGET_PRODUCT = "ClipboardFilter"
QMAKE_TARGET_DESCRIPTION = "ClipboardFilter"
QMAKE_TARGET_COPYRIGHT = "Copyright (c) 2023-2024 YangHuanhuan 3347484963@qq.com.  All rights reserved."
RC_LANG = 0x0004

RESOURCES += \
    data/res.qrc

RC_ICONS += "data/Icon/MainIcon.ico"
