QT       += qml quick testlib

QT       -= gui

TARGET = tst_$$TARGET
CONFIG   += warn_on qmltestcase

TEMPLATE = app
IMPORTPATH += $$PWD/data
#IMPORTPATH += $$OUT_PWD/../src/imports
DEFINES += QUICK_TEST_SOURCE_DIR=\\\"$$PWD/data\\\"

warning($$DEFINES)
SOURCES += \
    tst_qtleveldb.cpp

OTHER_FILES += \
    data/tst_leveldb.qml \
    data/tst_settings.qml
