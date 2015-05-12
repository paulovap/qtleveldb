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
    : QLevelDB(parent)
    , m_initialized(false)
    , m_source(QUrl())
{
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
        setFilename(m_source.toLocalFile());
        if (m_initialized)
            open();
    }
}

QQmlLevelDBReadStream *QQmlLevelDB::readStream(QString startKey, QString endKey)
{
    QQmlLevelDBReadStream *readStream = new QQmlLevelDBReadStream(m_levelDB.toWeakRef(), this);
    readStream->setEndKey(endKey);
    readStream->setStartKey(startKey);
    QQmlEngine::setObjectOwnership(readStream, QQmlEngine::JavaScriptOwnership);
    return readStream;
}

void QQmlLevelDB::classBegin()
{
}

void QQmlLevelDB::componentComplete()
{
    m_initialized = true;
    open();
}

QT_END_NAMESPACE
