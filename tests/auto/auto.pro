TEMPLATE = subdirs

SUBDIRS += cmake cpp

qtHaveModule(quick) {
    SUBDIRS += qml
}

