#include "../../3rdparty/leveldb/include/leveldb/db.h"

#include "qleveldb.h"
#include "qleveldbbatch.h"

QT_BEGIN_NAMESPACE

static QHash<QString, QWeakPointer<leveldb::DB> > dbInstances;
static QMultiHash<QString, QLevelDB*> qLevelDBInstances;

/*!
    \class QLevelDB
    \inmodule QtLevelDB
    \since 5.5

    \brief The QLevelDB class enables access to a LevelDB database.

    A QLevelDB classe can be used to insert or access data in a LevelDB database.
*/

/*!
    Constructs an new QLevelDB object.
*/
QLevelDB::QLevelDB(QObject *parent)
    : QObject(parent)
    , m_batch(nullptr)
    , m_opened(false)
    , m_status(Status::Undefined)
{

}

/*!
    Constructs an new QLevelDB object passing the filename of the database to be opened.
*/
QLevelDB::QLevelDB(QString filename, QObject *parent)
    : QLevelDB(parent)
{
    setFilename(filename);
}


QLevelDB::~QLevelDB()
{
    close();
}

/*!
    Indicates whether the database is already open or not.
*/
bool QLevelDB::opened()
{
    return m_opened;
}

/*!
    QLevelDB::Status of the last operation.
*/
QLevelDB::Status QLevelDB::status()
{
    return m_status;
}

/*!
    A text representing the last error in a operation.
*/
QString QLevelDB::lastError()
{
    return m_lastError;
}

/*!
    Path to the database files.
*/
QString QLevelDB::filename()
{
    return m_filename;
}

/*!
    Change the path to the database file. If there is a already a database opened,
    it will be automatically closed.
*/
void QLevelDB::setFilename(const QString filename)
{
    if (m_filename != filename){
        m_filename = filename;
        emit filenameChanged();
        if (opened())
            close();
    }
}

/*!
    Options used when opening the database. For more details see QLevelDBOptions.
*/
QLevelDBOptions *QLevelDB::options()
{
    leveldb::WriteOptions options;
    return &m_options;
}

/*!
    Open the database. Return the results as QLevelDB::Status
*/
QLevelDB::Status QLevelDB::open()
{
    close();

    if (dbInstances.contains(m_filename) && !dbInstances[m_filename].isNull()){
        //if there is an instance running, just grab the db
            m_levelDB = dbInstances[m_filename].toStrongRef();
            setOpened(true);
            setStatus(Ok);
            setLastError(QString());
    }else {
        //create directories in path
        QFileInfo fileInfo(m_filename);
        if(!fileInfo.dir().exists()){
            fileInfo.dir().mkpath(fileInfo.dir().absolutePath());
        }
        //else create a DB object
        leveldb::DB *db;
        leveldb::Options options;

        options.create_if_missing = m_options.createIfMissing();
        options.error_if_exists = m_options.errorIfExists();
        options.compression = static_cast<leveldb::CompressionType>(m_options.compressionType());
        options.paranoid_checks = m_options.paranoidChecks();


        leveldb::Status status = leveldb::DB::Open(options,
                                                   m_filename.toStdString(),
                                                   &db);
        if(status.ok()){
            m_levelDB.reset(db);
            dbInstances.insert(m_filename, m_levelDB.toWeakRef());
        }
        setOpened(status.ok());
        setLastError(QString::fromStdString(status.ToString()));
        Status code = parseStatusCode(status);
        setStatus(code);
    }
    if (m_opened){
        qLevelDBInstances.insertMulti(m_filename, this);
    }
    return m_status;
}

/*!
    Close the database
*/
void QLevelDB::close()
{
    delete m_batch;
    m_batch = nullptr;

    m_levelDB.clear();
    QWeakPointer<leveldb::DB> pointer = dbInstances[m_filename];
    if (pointer.isNull())
        dbInstances.remove(m_filename);

    for (auto key : qLevelDBInstances.keys()){
        qLevelDBInstances.remove(key, this);
    }
    setStatus(Status::Undefined);
    setOpened(false);
    setLastError(QString());
}

/*!
    Returns a QLevelDBBatch for bulk operations. QLevelDBBatch pointer returned will only
    remain valid as long as QLevelDB::close() or QLevelDB::batch() is not called.
*/
QLevelDBBatch* QLevelDB::batch()
{
    if (m_batch)
        delete m_batch;
    m_batch = new QLevelDBBatch(m_levelDB.toWeakRef(), this);
    connect(m_batch, &QLevelDBBatch::batchWritten, this, &QLevelDB::onBatchWritten);
    return m_batch;
}

/*!
    Deletes a Key/Value from database.
*/
bool QLevelDB::del(QString key)
{
    leveldb::WriteOptions options;
    leveldb::Status status = m_levelDB.data()->Delete(options, leveldb::Slice(key.toStdString()));

    return status.ok();
}

/*!
    Add a Key/Value in database. If there is already a value, it will be overriden. This operation is asynchronous.
*/
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

/*!
    Add a Key/Value in database. If there is already a value, it will be overriden. This operation is synchronous.
*/
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

/*!
    Returns a value for a given key. An options defaultValue can be used in case there is no such key.
*/
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

/*!
    Delete the database files from the filesystem.
*/
bool QLevelDB::destroyDB(QString filename)
{
    if(m_filename == filename){
        setFilename(QString());
    }
    leveldb::Options options;
    leveldb::Status status = leveldb::DestroyDB(filename.toStdString(), options);
    setStatus(parseStatusCode(status));
    setLastError(QString::fromStdString(status.ToString()));
    return status.ok();
}

/*!
    Execute LevelDB's repair operation on a database.
*/
bool QLevelDB::repairDB(QString filename)
{
    leveldb::Options options;
    leveldb::Status status = leveldb::RepairDB(filename.toStdString(), options);
    return status.ok();
}

/*!
    Create a QLevelDBReadStream object to read a series of key/values. If no startKey or endKey
    is set it will stream all inserted elements. The order of the stream is sorted by key.
*/
QLevelDBReadStream *QLevelDB::readStream(const QString startKey, const QString endKey)
{
    QLevelDBReadStream *readStream = new QLevelDBReadStream(m_levelDB.toWeakRef(), startKey, endKey, this);
    return readStream;
}

void QLevelDB::setStatus(QLevelDB::Status status)
{
    if (m_status != status){
        m_status = status;
        emit statusChanged();
    }
}

void QLevelDB::setLastError(QString text)
{
    if (m_lastError != text){
        m_lastError = text;
        emit lastErrorChanged();
    }
}

void QLevelDB::setOpened(bool opened)
{
    if (m_opened != opened){
        m_opened = opened;
        emit openedChanged();
    }
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
    QMultiHash<QString, QLevelDB*>::iterator i = qLevelDBInstances.find(m_filename);
    while (i != qLevelDBInstances.end() && i.key() == m_filename) {
       emit i.value()->keyValueChanged(key, value);
        ++i;
    }
}


QT_END_NAMESPACE
