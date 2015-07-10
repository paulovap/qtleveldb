#include "../../3rdparty/leveldb/include/leveldb/db.h"
#include "../../3rdparty/leveldb/include/leveldb/write_batch.h"
#include "../../3rdparty/leveldb/include/leveldb/options.h"
#include <qleveldbglobal.h>
#include "qleveldbbatch.h"

QT_BEGIN_NAMESPACE

/*!
    \class QLevelDBBatch
    \inmodule QtLevelDB
    \since 5.5

    \brief Apply bulk operations in a LevelDB Database, rolling back in case of error.
*/

/*!
    Constructs an new QLevelDBBatch object.
*/
QLevelDBBatch::QLevelDBBatch(QWeakPointer<leveldb::DB> db, QObject *parent)
    : QObject(parent)
    , m_levelDB(db)
    , m_writeBatch(new leveldb::WriteBatch())
{

}

QLevelDBBatch::~QLevelDBBatch()
{
    if(m_writeBatch)
        delete m_writeBatch;
}

/*!
    Deletes a key/value.
*/
QLevelDBBatch* QLevelDBBatch::del(QString key)
{
    m_operations.insert(key);

    QMutexLocker l(&m_mutex);
    m_writeBatch->Delete(leveldb::Slice(key.toStdString()));
    return this;
}

/*!
    Inserts or update a key/value.
*/
QLevelDBBatch* QLevelDBBatch::put(QString key, QVariant value)
{
    QString json = variantToJson(value);
    m_operations.insert(key);

    QMutexLocker l(&m_mutex);
    m_writeBatch->Put(leveldb::Slice(key.toStdString()),
                     leveldb::Slice(json.toStdString()));
    return this;
}

/*!
    Remove all peding operations.
*/
QLevelDBBatch* QLevelDBBatch::clear()
{
    m_operations.clear();

    QMutexLocker l(&m_mutex);
    m_writeBatch->Clear();
    return this;
}

/*!
    Apply all operations in database.
*/
bool QLevelDBBatch::write()
{
    leveldb::WriteOptions options;
    options.sync = true;
    if(m_levelDB.isNull())
        return static_cast<int>(QLevelDB::Status::NotFound);
    leveldb::Status status = m_levelDB.data()->Write(options, m_writeBatch);
    if(status.ok()){
        emit batchWritten(m_operations);
    }
    return status.ok();
}

QT_END_NAMESPACE

