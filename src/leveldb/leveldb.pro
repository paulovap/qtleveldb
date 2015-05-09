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
    $$PWD/qleveldbreadstream.h
PRIVATE_HEADERS += \
    $$PWD/qleveldbglobal.h

SOURCES += \
    $$PWD/qleveldbglobal.cpp \
    $$PWD/qleveldb.cpp \
    $$PWD/qleveldbbatch.cpp \
    $$PWD/qleveldboptions.cpp \
    $$PWD/qleveldbsettings.cpp \
    $$PWD/qleveldbreadstream.cpp

qtHaveModule(qml) {
    QT += qml

#    SOURCES += \
#        qqmlwebchannel.cpp \
#        qqmlwebchannelattached.cpp

    PUBLIC_HEADERS += \
        qleveldbsettings.h

#    PRIVATE_HEADERS += \
#        qqmlwebchannelattached_p.h
}

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS
