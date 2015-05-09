QT = core-private leveldb-private testlib qmltest

CONFIG += testcase strict_flags warn_on

INCLUDEPATH += ../../../src/leveldb
VPATH += ../../../src/leveldb

TARGET = tst_leveldb

SOURCES += \
    tst_leveldb.cpp

HEADERS += \
    tst_leveldb.h

OTHER_FILES += \
    data/*
