#include "qleveldb.h"
#include "qleveldbbatch.h"
#include "global.h"
#include <qqmlinfo.h>
#include <qqmlengine.h>
#include <QJsonDocument>
#include <QQmlEngine>

static QHash<QString, QWeakPointer<leveldb::DB> > leveldbInstances;
static QMultiHash<QString, QLevelDB*> qLeveldbInstances;
/*!
  \qmltype LevelDB
  \inqmlmodule QtLevelDB
  \brief Low level API to access LevelDB database.

*/
QLevelDB::QLevelDB(QObject *parent)
    : QObject(parent)
    , m_batch(nullptr)
    , m_initialized(false)
    , m_opened(false)
    , m_status(Status::Undefined)
    , m_statusText("")
{
}

QLevelDB::~QLevelDB()
{
    reset();
}

void QLevelDB::classBegin()
{
}

void QLevelDB::componentComplete()
{
    openDatabase(m_source.toLocalFile());
    m_initialized = true;
}

/*!
  \qmlproperty boolean LevelDB::opened
  \readonly
  \brief show when the database is ready to do operations
*/
bool QLevelDB::opened() const
{
    return m_opened;
}

/*!
  \qmlproperty url LevelDB::source
  \brief url that points to the leveldb database folder.

  For now it's only possible to use local file paths.
*/
QUrl QLevelDB::source() const
{
    return m_source;
}


/*!
  \qmlproperty enumeration LevelDB::status
  \brief This property holds information error in case the database is unable to load.
  \table
  \header
    \li {2, 1} \e {LevelDB::Status} is an enumeration:
  \header
    \li Type
    \li Description
  \row
    \li Layer.Undefined (default)
    \li Database is in a unkown state(probably unintialized)
  \row
    \li Layout.Ok
    \li Operation runs ok
  \row
    \li Layout.NotFound
    \li A value is not found for a Key, during get()
  \row
    \li Layout.InvalidArgument
    \li TODO
  \row
    \li Layout.IOError
    \li TODO
  \endtable
*/
QLevelDB::Status QLevelDB::status() const
{
    return m_status;
}

/*!
  \qmlproperty string LevelDB::statusText
  \brief String information about an error when it occurs
*/
QString QLevelDB::statusText() const
{
    return m_statusText;
}

void QLevelDB::setSource(QUrl source)
{

    if (m_source != source){
        m_source = source;
        emit sourceChanged();
        if (m_initialized)
            openDatabase(source.toLocalFile());
    }
}

QLevelDBOptions *QLevelDB::options()
{
    leveldb::WriteOptions options;
    return &m_options;
}

/*!
  \qmlmethod QLevelDBBatch* QLevelDB::batch()
  \brief Return an batch item to do batch operations

   The rayCast method is only useful with physics is enabled.
*/
QLevelDBBatch* QLevelDB::batch()
{
    if (m_batch)
        m_batch->deleteLater();
    m_batch = new QLevelDBBatch(m_levelDB, this);
    connect(m_batch, &QLevelDBBatch::batchWritten, this, &QLevelDB::onBatchWritten);
    return m_batch;
}

/*!
 * \qmlmethod bool QLevelDB::del(QString key)
*/
bool QLevelDB::del(QString key)
{
    leveldb::WriteOptions options;
    leveldb::Status status = m_levelDB.data()->Delete(options, leveldb::Slice(key.toStdString()));

    return status.ok();
}

bool QLevelDB::put(QString key, QVariant value)
{
    QVariant oldValue = get(key);
    //avoid inifine loops
    if(oldValue == value)
        return true;
    leveldb::WriteOptions options;
    QString json = variantToJson(value);
    if (m_opened && m_levelDB){
        leveldb::Status status = m_levelDB->Put(options,
                                                leveldb::Slice(key.toStdString()),
                                                leveldb::Slice(json.toStdString()));
        if(status.ok()){
            dispatchPropertyChange(key, value);
        }
        return status.ok();
    }
    return false;
}

bool QLevelDB::putSync(QString key, QVariant value)
{
    QVariant oldValue = get(key);
    //avoid inifine loops
    if(oldValue == value)
        return true;
    leveldb::WriteOptions options;
    QString json = variantToJson(value);
    options.sync = true;
    if (m_opened && m_levelDB){
        leveldb::Status status = m_levelDB->Put(options,
                                                leveldb::Slice(key.toStdString()),
                                                leveldb::Slice(json.toStdString()));
        if(status.ok()){
            dispatchPropertyChange(key, value);
        }
        return status.ok();
    }
    return false;
}

QVariant QLevelDB::get(QString key, QVariant defaultValue)
{
    leveldb::ReadOptions options;
    std::string value = "";
    if (m_opened && !m_levelDB.isNull()){
        leveldb::Status status = m_levelDB.data()->Get(options,
                                                leveldb::Slice(key.toStdString()),
                                                &value);
        if (status.ok())
            return jsonToVariant(QString::fromStdString(value));
    }
    return defaultValue;
}

bool QLevelDB::destroyDB(QUrl path)
{
    if (!path.isLocalFile())
        return Status::InvalidArgument;
    if(m_source == path){
        reset();
    }
    leveldb::Options options;
    leveldb::Status status = leveldb::DestroyDB(path.toLocalFile().toStdString(), options);
    return status.ok();
}

bool QLevelDB::repairDB(QUrl path)
{
    if (!path.isLocalFile())
        return Status::InvalidArgument;
    leveldb::Options options;
    leveldb::Status status = leveldb::RepairDB(path.toLocalFile().toStdString(), options);
    return status.ok();
}

void QLevelDB::setStatus(QLevelDB::Status status)
{
    if (m_status != status){
        m_status = status;
        emit statusChanged();
    }
}

void QLevelDB::setStatusText(QString text)
{
    if (m_statusText != text){
        m_statusText = text;
        emit statusTextChanged();
    }
}

void QLevelDB::setOpened(bool opened)
{
    if (m_opened != opened){
        m_opened = opened;
        emit openedChanged();
    }
}

bool QLevelDB::openDatabase(QString localPath)
{
    reset();

    if (leveldbInstances.contains(localPath) && !leveldbInstances[localPath].isNull()){
        //if there is an instance running, just grab the db
            m_levelDB = leveldbInstances[localPath].toStrongRef();
            setOpened(true);
            setStatus(Ok);
            setStatusText("ok");
    }else {
        //else create a DB object
        leveldb::DB *db;
        leveldb::Options options = m_options.leveldbOptions();
        leveldb::Status status = leveldb::DB::Open(options,
                                                   localPath.toStdString(),
                                                   &db);
        if(status.ok()){
            m_levelDB.reset(db);
            leveldbInstances.insert(localPath, m_levelDB.toWeakRef());
        }
        setOpened(status.ok());
        setStatusText(QString::fromStdString(status.ToString()));
        Status code = parseStatusCode(status);
        setStatus(code);
    }
    if (m_opened){
        qLeveldbInstances.insertMulti(localPath, this);
    }
    return m_opened;
}

void QLevelDB::reset()
{
    m_levelDB.clear();
    QWeakPointer<leveldb::DB> pointer = leveldbInstances[m_source.toLocalFile()];
    if (pointer.isNull())
        leveldbInstances.remove(m_source.toLocalFile());

    for (auto key : qLeveldbInstances.keys()){
        qLeveldbInstances.remove(key, this);
    }
    setStatus(Status::Undefined);
    setOpened(false);
    setStatusText(QString());
}

void QLevelDB::onBatchWritten(QSet<QString> keys)
{
    for (auto key : keys){
        dispatchPropertyChange(key, get(key, QVariant()));
    }
}

QLevelDB::Status QLevelDB::parseStatusCode(leveldb::Status &status)
{
    if (status.ok())
        return Status::Ok;
    if (status.IsCorruption())
        return Status::Corruption;
    if (status.IsIOError())
        return Status::IOError;
    if (status.IsNotFound())
        return Status::NotFound;
    return Status::Undefined;
}

void QLevelDB::dispatchPropertyChange(QString key, QVariant value)
{
    QString local = m_source.toLocalFile();
    QMultiHash<QString, QLevelDB*>::iterator i = qLeveldbInstances.find(local);
    while (i != qLeveldbInstances.end() && i.key() == local) {
       emit i.value()->propertyChanged(key, value);
        ++i;
    }
}

