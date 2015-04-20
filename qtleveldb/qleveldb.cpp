#include "qleveldb.h"
#include <QDebug>
QLevelDB::QLevelDB(QObject *parent)
    : QObject(parent)
    , m_options(new Options(this))
{

}

QUrl QLevelDB::source() const
{
    return m_source;
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

Options *QLevelDB::options() const
{
    return m_options;
}

bool QLevelDB::openDatabase(QString localPath)
{
    leveldb::Options options = m_options->leveldbOptions();
    leveldb::Status status = leveldb::DB::Open(options,
                                               localPath.toLocal8Bit().constData(),
                                               &m_levelDb);
    return status.ok();
}



Options::Options(QObject *parent) : QObject(parent)
{
}

leveldb::Options Options::leveldbOptions() const
{
    return m_options;
}
bool Options::createIfMissing() const
{
    return m_options.create_if_missing;
}

bool Options::errorIfExists() const
{
    return m_options.error_if_exists;
}

bool Options::paranoidChecks() const
{
    return m_options.paranoid_checks;
}

Options::CompressionType Options::compressionType() const
{
    return m_options.compression == leveldb::CompressionType::kNoCompression ?
                CompressionType::NoCompression : CompressionType::SnappyCompression;
}

void Options::setCreateIfMissing(bool value)
{
    m_options.create_if_missing = value;
}

void Options::setErrorIfExists(bool value)
{
    m_options.error_if_exists = value;
}

void Options::setParanoidChecks(bool value)
{
    m_options.paranoid_checks = value;
}

void Options::setCompressionType(Options::CompressionType type)
{
    m_options.compression = type == CompressionType::SnappyCompression ?
                leveldb::CompressionType::kSnappyCompression :
                leveldb::CompressionType::kNoCompression;
}


