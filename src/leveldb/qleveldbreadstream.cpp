#include "../../3rdparty/leveldb/include/leveldb/db.h"
#include "../../3rdparty/leveldb/include/leveldb/iterator.h"
#include "../../3rdparty/leveldb/include/leveldb/options.h"
#include "../../3rdparty/leveldb/include/leveldb/comparator.h"

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
    , m_length(-1)
    , m_db(db)
    , m_byteWiseComparator(leveldb::BytewiseComparator())
{

}

/*!
    Constructs an new QLevelDBReadStream object. startKey and endKey indicate
    the boundaries of the stream.
*/
QLevelDBReadStream::QLevelDBReadStream(QWeakPointer<leveldb::DB> db, QString startKey, int length, QObject *parent)
    : QObject(parent)
    , m_shouldStop(false)
    , m_startKey(startKey)
    , m_length(length)
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
    emit streamStarted();
    start([this](QString key, QVariant value){
        emit nextKeyValue(key, value);
        return !m_shouldStop;
    });
    emit streamEnded();
    m_shouldStop = false;
    return true;
}

bool QLevelDBReadStream::start(std::function<bool (QString, QVariant)> callback)
{
    if (m_db.isNull())
        return false;
    if (m_length == 0)
        return true;

    auto strongDB = m_db.toStrongRef();

    leveldb::ReadOptions options;
    leveldb::Iterator *it = strongDB.data()->NewIterator(options);
    int length = m_length;

    if (!it)
        return false;

    if (!m_startKey.isEmpty())
        it->Seek(leveldb::Slice(m_startKey.toStdString()));
    else
        it->SeekToFirst();

    for ( ;it->Valid(); it->Next()){

        QString key = QString::fromStdString(it->key().ToString());
        QVariant value = jsonToVariant(QString::fromStdString(it->value().ToString()));

        length--;
        bool shouldContinue = callback(key, value);

        if (!shouldContinue || (m_length != -1 && length <= 0))
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

QT_END_NAMESPACE
//#include "moc_qleveldbreadstream.cpp"
