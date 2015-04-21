#include "qleveldb.h"
#include <qqmlinfo.h>
#include <QJsonDocument>

QLevelDB::QLevelDB(QObject *parent)
    : QObject(parent)
    , m_levelDB(nullptr)
    , m_opened(false)
    , m_status(Status::Undefined)
    , m_statusText("")
{

}

QLevelDB::~QLevelDB()
{
    if (m_levelDB != nullptr)
        delete m_levelDB;
}

bool QLevelDB::opened() const
{
    return m_opened;
}

QUrl QLevelDB::source() const
{
    return m_source;
}

QLevelDB::Status QLevelDB::status() const
{
    return m_status;
}

QString QLevelDB::statusText() const
{
    return m_statusText;
}

void QLevelDB::setSource(QUrl source)
{
    if(!source.isLocalFile())
        qDebug() << "Error: url not a local";
    if(m_source != source){
        m_source = source;
        emit sourceChanged();
        openDatabase(source.toLocalFile());
    }
}

QLevelDBOptions *QLevelDB::options()
{
    leveldb::WriteOptions options;
    return &m_options;
}

QLevelDB::Status QLevelDB::del(QString key)
{
    leveldb::WriteOptions options;
    leveldb::Status status = m_levelDB->Delete(options, leveldb::Slice(key.toStdString()));
    return parseStatusCode(status);
}

QLevelDB::Status QLevelDB::put(QString key, QString value)
{
    leveldb::WriteOptions options;
    if (m_opened && m_levelDB){
        leveldb::Status status = m_levelDB->Put(options,
                       leveldb::Slice(key.toStdString()),
                       leveldb::Slice(value.toStdString()));
        return parseStatusCode(status);
    }
    return Status::NotFound;
}

QLevelDB::Status QLevelDB::putSync(QString key, QString value)
{
    leveldb::WriteOptions options;
    options.sync = true;
    if (m_opened && m_levelDB){
        leveldb::Status status = m_levelDB->Put(options,
                       leveldb::Slice(key.toStdString()),
                       leveldb::Slice(value.toStdString()));
        return parseStatusCode(status);
    }
    return Status::NotFound;
}

void QLevelDB::get(QString key, const QJSValue &callback)
{
    if (!callback.isCallable()){
        qmlInfo(this) << "passing a invalid argument as callback";
        return;
    }
    QJSValue back = callback;
    QJSValueList list;
    leveldb::ReadOptions options;
    std::string value = "";
    if (m_opened && m_levelDB){
        leveldb::Status status = m_levelDB->Get(options,
                                                leveldb::Slice(key.toStdString()),
                                                &value);

        list << callback.engine()->toScriptValue(static_cast<int>(parseStatusCode(status)));
        list << callback.engine()->toScriptValue(QString::fromStdString(value));
        back.call(list);
    }
    list<< callback.engine()->toScriptValue(static_cast<int>(Status::Undefined));
    back.call(list);
}

QLevelDB::Status QLevelDB::destroyDB(QUrl path)
{
    if (!path.isLocalFile())
        return Status::InvalidArgument;
    leveldb::Options options;
    leveldb::Status status = leveldb::DestroyDB(path.toLocalFile().toStdString(), options);
    return parseStatusCode(status);
}

QLevelDB::Status QLevelDB::repairDB(QUrl path)
{
    if (!path.isLocalFile())
        return Status::InvalidArgument;
    leveldb::Options options;
    leveldb::Status status = leveldb::RepairDB(path.toLocalFile().toStdString(), options);
    return parseStatusCode(status);
}

void QLevelDB::classBegin()
{
}

void QLevelDB::componentComplete()
{
    openDatabase(m_source.toLocalFile());
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
    if (m_levelDB)
        reset();
    leveldb::Options options = m_options.leveldbOptions();
    leveldb::Status status = leveldb::DB::Open(options,
                                               localPath.toStdString(),
                                               &m_levelDB);

    setOpened(status.ok());
    setStatusText(QString::fromStdString(status.ToString()));
    Status code = parseStatusCode(status);
    setStatus(code);
    return m_opened;
}

void QLevelDB::reset()
{
    if (m_levelDB)
        delete m_levelDB;
    setStatus(Status::Undefined);
    setOpened(false);
    setStatusText(QString());
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



QLevelDBOptions::QLevelDBOptions(QObject *parent) : QObject(parent)
{
}

leveldb::Options QLevelDBOptions::leveldbOptions() const
{
    return m_options;
}
bool QLevelDBOptions::createIfMissing() const
{
    return m_options.create_if_missing;
}

bool QLevelDBOptions::errorIfExists() const
{
    return m_options.error_if_exists;
}

bool QLevelDBOptions::paranoidChecks() const
{
    return m_options.paranoid_checks;
}

QLevelDBOptions::CompressionType QLevelDBOptions::compressionType() const
{
    return m_options.compression == leveldb::CompressionType::kNoCompression ?
                CompressionType::NoCompression : CompressionType::SnappyCompression;
}

void QLevelDBOptions::setCreateIfMissing(bool value)
{
    m_options.create_if_missing = value;
}

void QLevelDBOptions::setErrorIfExists(bool value)
{
    m_options.error_if_exists = value;
}

void QLevelDBOptions::setParanoidChecks(bool value)
{
    m_options.paranoid_checks = value;
}

void QLevelDBOptions::setCompressionType(QLevelDBOptions::CompressionType type)
{
    m_options.compression = type == CompressionType::SnappyCompression ?
                leveldb::CompressionType::kSnappyCompression :
                leveldb::CompressionType::kNoCompression;
}


