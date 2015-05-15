TARGET = tst_cpp

QT += core concurrent leveldb testlib
CONFIG += testcase c++11

INCLUDEPATH += ../../../src/leveldb
VPATH += ../../../src/leveldb

SOURCES += \
    tst_cpp.cpp
