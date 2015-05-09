#include "qleveldboptions.h"

QT_BEGIN_NAMESPACE

QLevelDBOptions::QLevelDBOptions(QObject *parent) : QObject(parent)
{
    m_options.create_if_missing = true;
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

void QLevelDBOptions::classBegin()
{

}

void QLevelDBOptions::componentComplete()
{

}

QT_END_NAMESPACE
//#include "moc_qleveldboptions.cpp"
