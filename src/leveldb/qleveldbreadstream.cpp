#include "../../3rdparty/leveldb/include/leveldb/db.h"
#include "../../3rdparty/leveldb/include/leveldb/iterator.h"
#include "../../3rdparty/leveldb/include/leveldb/options.h"
#include "qleveldbglobal.h"
#include "qleveldbreadstream.h"

QT_BEGIN_NAMESPACE

/*!
    \class QLevelDBReadStream
    \inmodule QtLevelDB
    \since 5.5

    \brief Provides a way to scan the database, receiving a stream of key/values in a ordered manner.
*/

/*!
    Constructs an new QLevelDBReadStream object.
*/
QLevelDBReadStream::QLevelDBReadStream(QWeakPointer<leveldb::DB> db, QObject *parent)
    : QObject(parent)
    , m_shouldStop(false)
    , m_db(db)
{

}

/*!
    Constructs an new QLevelDBReadStream object. startKey and endKey indicate
    the boundaries of the stream.
*/
QLevelDBReadStream::QLevelDBReadStream(QWeakPointer<leveldb::DB> db, QString startKey, QString endKey, QObject *parent)
    : QObject(parent)
    , m_shouldStop(false)
    , m_startKey(startKey)
    , m_endKey(endKey)
    , m_db(db)
{

}


QLevelDBReadStream::~QLevelDBReadStream()
{

}

/*!
    Start streaming key/value pairs. Theses pairs will be emited in the QLevelDBReadStream::nextKeyValue() signal.
*/
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

/*!
    Stops key/value streaming.
*/
void QLevelDBReadStream::stop()
{
    m_shouldStop = true;
}

/*!
    First key that will be streamed.
*/
QString QLevelDBReadStream::startKey() const
{
    return m_startKey;
}

/*!
    Last key that will be streamed.
*/
QString QLevelDBReadStream::endKey() const
{
    return m_endKey;
}

QT_END_NAMESPACE
//#include "moc_qleveldbreadstream.cpp"
