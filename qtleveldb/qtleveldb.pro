TEMPLATE = lib

CONFIG +=qt plugin c++11
QT += core quick

TARGET = qtleveldb

include(qtleveldb.pri)
include(plugin/plugin.pri)

API_VER=1.0
TARGETPATH = QtLevelDB
DESTDIR = $$OUT_PWD/imports/QtLevelDB/

!isEmpty(QTPATH): target.path = $$QTPATH/imports/$$TARGETPATH
else: target.path = $$[QT_INSTALL_QML]/$$replace(TARGETPATH, \\., /).$$API_VER
;

# Copy qml files post build
win32 {
    DESTDIR ~= s,/,\\,g
    QMAKE_POST_LINK += $$[QT_INSTALL_BINS]\qmlplugindump -notrelocatable QtLevelDB $$API_VER $$OUT_PWD\imports  > $$DESTDIR\plugins.qmltypes
}
unix {
    QMAKE_POST_LINK += $$[QT_INSTALL_BINS]/qmlplugindump -notrelocatable QtLevelDB $$API_VER $$OUT_PWD/imports  > $$DESTDIR/plugins.qmltypes
}

qmltypes.path = $$target.path
qmltypes.files += $$DESTDIR/plugins.qmltypes
export(qmltypes.files)

qmlpluginfiles.path = $$target.path
qmlpluginfiles.files = $$QMLFILES

QMAKE_EXTRA_TARGETS += qmltypes qmlpluginfiles
INSTALLS += target qmltypes qmlpluginfiles
