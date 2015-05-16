#include "qleveldbglobal.h"
#include "qqmlleveldb.h"
#include "qqmlleveldbreadstream.h"
#include "qleveldbbatch.h"
#include <qqmlinfo.h>
#include <QJsonDocument>
#include <QQmlEngine>

QT_BEGIN_NAMESPACE

/*!
  \qmltype LevelDB
  \inqmlmodule QtLevelDB
  \brief Low level API to access LevelDB database.

*/
QQmlLevelDB::QQmlLevelDB(QObject *parent)
    : QObject(parent)
    , m_initialized(false)
    , m_source(QUrl())
{
    connect(&m_leveldb, &QLevelDB::statusChanged, this, &QQmlLevelDB::statusChanged);
    connect(&m_leveldb, &QLevelDB::lastErrorChanged, this, &QQmlLevelDB::lastErrorChanged);
    connect(&m_leveldb, &QLevelDB::openedChanged, this, &QQmlLevelDB::openedChanged);
    connect(&m_leveldb, &QLevelDB::keyValueChanged, this, &QQmlLevelDB::keyValueChanged);
}

QUrl QQmlLevelDB::source()
{
    return m_source;
}

void QQmlLevelDB::setSource(QUrl source)
{
    if (m_source != source){
        m_source = source;
        emit sourceChanged();
        m_leveldb.setFilename(m_source.toLocalFile());
        if (m_initialized)
            m_leveldb.open();
    }
}

QLevelDB::Status QQmlLevelDB::status()
{
    return m_leveldb.status();
}

QString QQmlLevelDB::lastError()
{
    return m_leveldb.lastError();
}

bool QQmlLevelDB::opened()
{
    return m_leveldb.opened();
}

QLevelDBOptions *QQmlLevelDB::options()
{
    return m_leveldb.options();
}

QLevelDBBatch *QQmlLevelDB::batch()
{
    return m_leveldb.batch();
}

bool QQmlLevelDB::del(QString key)
{
    return m_leveldb.del(key);
}

QVariant QQmlLevelDB::get(QString key, QVariant defaultValue)
{
    return m_leveldb.get(key, defaultValue);
}

bool QQmlLevelDB::put(QString key, QVariant value)
{
    return m_leveldb.put(key, value);
}

bool QQmlLevelDB::putSync(QString key, QVariant value)
{
    return m_leveldb.putSync(key, value);
}

bool QQmlLevelDB::destroyDB(QUrl url)
{
    return m_leveldb.destroyDB(url.toLocalFile());
}

bool QQmlLevelDB::repairDB(QUrl url)
{
    return m_leveldb.repairDB(url.toLocalFile());
}
//TODO: maybe readStream should be owned by *this* and deleted when database is closed or source changed.
QQmlLevelDBReadStream *QQmlLevelDB::readStream(QString startKey, QString endKey)
{
    QQmlLevelDBReadStream *readStream = new QQmlLevelDBReadStream(m_leveldb.dbNativeHandler(), startKey, endKey, this);
    QQmlEngine::setObjectOwnership(readStream, QQmlEngine::JavaScriptOwnership);
    return readStream;
}

void QQmlLevelDB::classBegin()
{
}

void QQmlLevelDB::componentComplete()
{
    m_initialized = true;
    m_leveldb.open();
}

QT_END_NAMESPACE
