#include "qleveldbbatch.h"
#include "global.h"
QLevelDBBatch::QLevelDBBatch(QSharedPointer<leveldb::DB> db, QObject *parent)
    : QObject(parent)
    , m_levelDB(db)
{

}

QLevelDBBatch* QLevelDBBatch::del(QString key)
{
    m_writeBatch.Delete(leveldb::Slice(key.toStdString()));
    return this;
}

QLevelDBBatch* QLevelDBBatch::put(QString key, QVariant value)
{
    QString json = variantToJson(value);
    m_writeBatch.Put(leveldb::Slice(key.toStdString()),
                     leveldb::Slice(json.toStdString()));
    return this;
}

QLevelDBBatch* QLevelDBBatch::clear()
{
    m_writeBatch.Clear();
    return this;
}

int QLevelDBBatch::write()
{
    leveldb::WriteOptions options;

    if(m_levelDB.isNull())
        return static_cast<int>(QLevelDB::Status::NotFound);

    options.sync = true;
    leveldb::Status status = m_levelDB.data()->Write(options, &m_writeBatch);
    return static_cast<int>(parseStatusCode(status));
}

void QLevelDBBatch::classBegin()
{
}

void QLevelDBBatch::componentComplete()
{
}

QLevelDB::Status QLevelDBBatch::parseStatusCode(leveldb::Status &status)
{
    if (status.ok())
            return QLevelDB::Status::Ok;
        if (status.IsCorruption())
            return QLevelDB::Status::Corruption;
        if (status.IsIOError())
            return QLevelDB::Status::IOError;
        if (status.IsNotFound())
            return QLevelDB::Status::NotFound;
        return QLevelDB::Status::Undefined;
}

