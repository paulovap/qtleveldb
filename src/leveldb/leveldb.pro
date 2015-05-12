TARGET = QtLevelDB
QT = core-private
CONFIG += warn_on strict_flags

load(qt_module)

QMAKE_DOCS = $$PWD/doc/qtleveldb.qdocconf

include($$PWD/../3rdparty/leveldb.pri)

PUBLIC_HEADERS += \
    $$PWD/qleveldb.h \
    $$PWD/qleveldbbatch.h \
    $$PWD/qleveldboptions.h \
    $$PWD/qleveldbreadstream.h \
    $$PWD/qleveldb.h

PRIVATE_HEADERS += \
    $$PWD/qleveldbglobal.h

SOURCES += \
    $$PWD/qleveldb.cpp \
    $$PWD/qleveldbbatch.cpp \
    $$PWD/qleveldbglobal.cpp \
    $$PWD/qleveldboptions.cpp \
    $$PWD/qleveldbreadstream.cpp \
    $$PWD/qqmlleveldb.cpp \
    $$PWD/qqmlleveldbsettings.cpp \
    $$PWD/qqmlleveldbreadstream.cpp

qtHaveModule(qml) {
    QT += qml

    PUBLIC_HEADERS += \
        $$PWD/qqmlleveldb.h \
        $$PWD/qqmlleveldbsettings.h \
        $$PWD/qqmlleveldbreadstream.h
}

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS
