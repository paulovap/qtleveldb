TEMPLATE = subdirs

SUBDIRS += leveldb

qtHaveModule(quick) {
    SUBDIRS += imports
    imports.depends = leveldb
}
