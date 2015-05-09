#include <qleveldbglobal.h>
#include "qleveldbbatch.h"

QT_BEGIN_NAMESPACE

QLevelDBBatch::QLevelDBBatch(QWeakPointer<leveldb::DB> db, QObject *parent)
    : QObject(parent)
    , m_levelDB(db)
{

}

QLevelDBBatch* QLevelDBBatch::del(QString key)
{
    m_operations.insert(key);
    m_writeBatch.Delete(leveldb::Slice(key.toStdString()));
    return this;
}

QLevelDBBatch* QLevelDBBatch::put(QString key, QVariant value)
{
    QString json = variantToJson(value);
    m_operations.insert(key);
    m_writeBatch.Put(leveldb::Slice(key.toStdString()),
                     leveldb::Slice(json.toStdString()));
    return this;
}

QLevelDBBatch* QLevelDBBatch::clear()
{
    m_writeBatch.Clear();
    m_operations.clear();
    return this;
}

bool QLevelDBBatch::write()
{
    leveldb::WriteOptions options;
    options.sync = true;
    if(m_levelDB.isNull())
        return static_cast<int>(QLevelDB::Status::NotFound);
    leveldb::Status status = m_levelDB.data()->Write(options, &m_writeBatch);
    if(status.ok()){
        emit batchWritten(m_operations);
    }
    return status.ok();
}

void QLevelDBBatch::classBegin()
{
}

void QLevelDBBatch::componentComplete()
{
}
QT_END_NAMESPACE
//#include "moc_qleveldbbatch.cpp"
