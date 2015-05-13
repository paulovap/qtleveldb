#include "../../3rdparty/leveldb/include/leveldb/db.h"
#include "../../3rdparty/leveldb/include/leveldb/iterator.h"
#include "../../3rdparty/leveldb/include/leveldb/options.h"
#include "qleveldbglobal.h"
#include "qleveldbreadstream.h"

QT_BEGIN_NAMESPACE

QLevelDBReadStream::QLevelDBReadStream(QWeakPointer<leveldb::DB> db, QObject *parent)
    : QObject(parent)
    , m_shouldStop(false)
    , m_db(db)
{

}

QLevelDBReadStream::~QLevelDBReadStream()
{

}

bool QLevelDBReadStream::start()
{
    if (m_db.isNull())
        return false;
    auto strongDB = m_db.toStrongRef();

    leveldb::ReadOptions options;
    leveldb::Iterator *it = strongDB.data()->NewIterator(options);

    if (!it)
        return false;
    if (m_startKey.isEmpty())
        it->SeekToFirst();
    else
        it->Seek(leveldb::Slice(m_startKey.toStdString()));

    while(it->Valid() && !m_shouldStop){
        QString key = QString::fromStdString(it->key().ToString());
        QVariant value = jsonToVariant(QString::fromStdString(it->value().ToString()));
        emit nextKeyValue(key, value);
        if (m_endKey.isNull() || m_endKey > key)
            it->Next();
        else
            break;
    }
    delete it;
    return true;
}

void QLevelDBReadStream::stop()
{
    m_shouldStop = true;
}

QString QLevelDBReadStream::startKey() const
{
    return m_startKey;
}

void QLevelDBReadStream::setStartKey(QString key)
{
    m_startKey = key;
}

QString QLevelDBReadStream::endKey() const
{
    return m_endKey;
}

void QLevelDBReadStream::setEndKey(QString key)
{
    m_endKey = key;
}

QT_END_NAMESPACE
//#include "moc_qleveldbreadstream.cpp"
