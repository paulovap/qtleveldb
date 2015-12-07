# -------------------------------------------------------------------
# This file contains shared rules used both when building leveldb
# itself, and by targets that use leveldb.
#
# See 'Tools/qmake/README' for an overview of the build system
# -------------------------------------------------------------------

macx: DEFINES += OS_MACOSX
ios: DEFINES += OS_MACOSX

android {
    DEFINES += OS_ANDROID
} else{

 linux {
    DEFINES += OS_LINUX
    QMAKE_CFLAGS += -fno-builtin-memcmp -pthread
    QMAKE_CXXFLAGS += -fno-builtin-memcmp -pthread
    QMAKE_LFLAGS += -pthread -lrt
}
}

DEFINES += LEVELDB_PLATFORM_QT

android:
freebsd*: DEFINES += OS_FREEBSD

SOURCE_DIR = $$PWD/leveldb
INCLUDEPATH += \
    $$SOURCE_DIR/include \
    $$SOURCE_DIR

PRIVATE_HEADERS += \
    $$SOURCE_DIR/db/builder.h \
    $$SOURCE_DIR/db/dbformat.h \
    $$SOURCE_DIR/db/db_impl.h \
    $$SOURCE_DIR/db/db_iter.h \
    $$SOURCE_DIR/db/filename.h \
    $$SOURCE_DIR/db/log_format.h \
    $$SOURCE_DIR/db/log_reader.h \
    $$SOURCE_DIR/db/log_writer.h \
    $$SOURCE_DIR/db/memtable.h \
    $$SOURCE_DIR/db/skiplist.h \
    $$SOURCE_DIR/db/snapshot.h \
    $$SOURCE_DIR/db/table_cache.h \
    $$SOURCE_DIR/db/version_edit.h \
    $$SOURCE_DIR/db/version_set.h \
    $$SOURCE_DIR/db/write_batch_internal.h \
    $$SOURCE_DIR/port/port.h \
    $$SOURCE_DIR/port/port_qt.h \
    $$SOURCE_DIR/port/thread_annotations.h \
    $$SOURCE_DIR/port/win/stdint.h \
    $$SOURCE_DIR/helpers/memenv/memenv.h \
    $$SOURCE_DIR/table/block_builder.h \
    $$SOURCE_DIR/table/block.h \
    $$SOURCE_DIR/table/filter_block.h \
    $$SOURCE_DIR/table/format.h \
    $$SOURCE_DIR/table/iterator_wrapper.h \
    $$SOURCE_DIR/table/merger.h \
    $$SOURCE_DIR/table/two_level_iterator.h \
    $$SOURCE_DIR/include/leveldb/cache.h \
    $$SOURCE_DIR/include/leveldb/c.h \
    $$SOURCE_DIR/include/leveldb/comparator.h \
    $$SOURCE_DIR/include/leveldb/db.h \
    $$SOURCE_DIR/include/leveldb/env.h \
    $$SOURCE_DIR/include/leveldb/filter_policy.h \
    $$SOURCE_DIR/include/leveldb/iterator.h \
    $$SOURCE_DIR/include/leveldb/options.h \
    $$SOURCE_DIR/include/leveldb/slice.h \
    $$SOURCE_DIR/include/leveldb/status.h \
    $$SOURCE_DIR/include/leveldb/table_builder.h \
    $$SOURCE_DIR/include/leveldb/table.h \
    $$SOURCE_DIR/include/leveldb/write_batch.h \
    $$SOURCE_DIR/util/arena.h \
    $$SOURCE_DIR/util/coding.h \
    $$SOURCE_DIR/util/crc32c.h \
    $$SOURCE_DIR/util/hash.h \
    $$SOURCE_DIR/util/histogram.h \
    $$SOURCE_DIR/util/logging.h \
    $$SOURCE_DIR/util/mutexlock.h \
    $$SOURCE_DIR/util/qt_logger.h \
    $$SOURCE_DIR/util/random.h


MEMENV_SOURCES=helpers/memenv/memenv.cc
SOURCES += \
    $$SOURCE_DIR/db/builder.cc \
    $$SOURCE_DIR/db/c.cc \
    $$SOURCE_DIR/db/dbformat.cc \
    $$SOURCE_DIR/db/db_impl.cc \
    $$SOURCE_DIR/db/db_iter.cc \
    $$SOURCE_DIR/db/filename.cc \
#    $$SOURCE_DIR/db/leveldb_main.cc \
    $$SOURCE_DIR/db/log_reader.cc \
    $$SOURCE_DIR/db/log_writer.cc \
    $$SOURCE_DIR/db/memtable.cc \
    $$SOURCE_DIR/db/repair.cc \
    $$SOURCE_DIR/db/table_cache.cc \
    $$SOURCE_DIR/db/version_edit.cc \
    $$SOURCE_DIR/db/version_set.cc \
    $$SOURCE_DIR/db/write_batch.cc \
    $$SOURCE_DIR/table/block_builder.cc \
    $$SOURCE_DIR/table/block.cc \
    $$SOURCE_DIR/table/filter_block.cc \
    $$SOURCE_DIR/table/format.cc \
    $$SOURCE_DIR/table/iterator.cc \
    $$SOURCE_DIR/table/merger.cc \
    $$SOURCE_DIR/table/table_builder.cc \
    $$SOURCE_DIR/table/table.cc \
    $$SOURCE_DIR/table/two_level_iterator.cc \
    $$SOURCE_DIR/helpers/memenv/memenv.cc \
    $$SOURCE_DIR/util/arena.cc \
    $$SOURCE_DIR/util/bloom.cc \
    $$SOURCE_DIR/util/cache.cc \
    $$SOURCE_DIR/util/coding.cc \
    $$SOURCE_DIR/util/comparator.cc \
    $$SOURCE_DIR/util/crc32c.cc \
    $$SOURCE_DIR/util/env.cc \
    $$SOURCE_DIR/util/env_qt.cc \
    $$SOURCE_DIR/util/filter_policy.cc \
    $$SOURCE_DIR/util/hash.cc \
    $$SOURCE_DIR/util/histogram.cc \
    $$SOURCE_DIR/util/logging.cc \
    $$SOURCE_DIR/util/options.cc \
    $$SOURCE_DIR/util/status.cc

