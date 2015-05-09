QT = core quick leveldb-private

INCLUDEPATH += ../../leveldb
VPATH += ../../leveldb

SOURCES += \
    plugin.cpp

load(qml_plugin)

