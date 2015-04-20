QT       += qml quick testlib

QT       -= gui

TARGET = tst_qtleveldb
CONFIG   += warn_on qmltestcase

TEMPLATE = app
IMPORTPATH += $$OUT_PWD/../src/imports
SOURCES += \
    tst_qtleveldb.cpp
OTHER_FILES += \
    tst_main.qml

DISTFILES += \
    tst_leveldb.qml
