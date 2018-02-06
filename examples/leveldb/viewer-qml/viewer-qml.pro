TEMPLATE = app
TARGET = viewer-qml
QT += concurrent qml widgets quick leveldb
CONFIG += c++11

RESOURCES += qml.qrc

SOURCES += \
    main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/leveldb/viewer-qml
INSTALLS += target


