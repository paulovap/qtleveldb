CXX_MODULE = leveldb
TARGET = declarative_leveldb
TARGETPATH = QtLevelDB
IMPORT_VERSION = 1.0

QT = core quick leveldb leveldb-private

SOURCES +=  plugin.cpp

load(qml_plugin)

OTHER_FILES += qmldir

