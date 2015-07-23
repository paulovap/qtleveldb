#include "qleveldbglobal.h"
#include "qqmlleveldb.h"
#include "qleveldbbatch.h"
#include <qqmlinfo.h>
#include <QJsonDocument>
#include <QQmlEngine>

QT_BEGIN_NAMESPACE

/*!
  \qmltype LevelDB
  \inqmlmodule QtLevelDB
  \since 1.0
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

/*!
     \qmlproperty url LevelDB::source

     This property specifies the URL of datavase. The URL must point to a
     local file that contains LeveldDB's data.
*/
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

/*!
    \readonly
    \qmlattachedproperty enumeration LevelDB::status

    This property holds the status of the item. It can have one of the following values:
    \list
    \li \c LevelDB.Undefined: No initialized or undefined behavior
    \li \c LevelDB.Ok: operation return success
    \li \c LevelDB.NotFound: Key not found during operation
    \li \c LevelDB.InvalidArgument: Invalid argument in a operation
    \li \c LevelDB.IOError: Io error during operation
    \endlist
*/
QLevelDB::Status QQmlLevelDB::status()
{
    return m_leveldb.status();
}

/*!
     \qmlproperty string LevelDB::lastError

     A string that indicates the status result of the last operation.
*/
QString QQmlLevelDB::lastError()
{
    return m_leveldb.lastError();
}

/*!
     \qmlproperty bool LevelDB::opened

     Indicate wheter the database is open or not.
*/
bool QQmlLevelDB::opened()
{
    return m_leveldb.opened();
}

/*!
     \qmlproperty QLevelDBOptions* LevelDB::options

     This property holds the configuration used to open the database.
     It cannot be changed once the database it opened
*/
QLevelDBOptions *QQmlLevelDB::options()
{
    return m_leveldb.options();
}

/*!
     \qmlproperty QLevelDBBatch* LevelDB::batch

     Return batch object for bulk operations.
*/
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

bool QQmlLevelDB::readStream(QJSValue callback, QString startKey, int length)
{
    if (!callback.isCallable())
        return false;

    std::function<bool(QString, QVariant)> func = [this, &callback](QString key, QVariant value) {
        QJSEngine *engine = qjsEngine(this);
        QJSValueList list;
        list << QJSValue(key);
        list << engine->toScriptValue<QVariant>(value);
        bool result = callback.call(list).toBool();
        return result;
    };
    return m_leveldb.readStream(func, startKey, length);
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
