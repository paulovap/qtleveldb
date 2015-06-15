#include "../../3rdparty/leveldb/include/leveldb/options.h"
#include "qleveldboptions.h"

QT_BEGIN_NAMESPACE

/*!
    \class QLevelDBOptions
    \inmodule QtLevelDB
    \since 5.5

    \brief The QLevelDBOptions manipulates options used when opening a LevelDB Database.
*/

/*!
    Constructs an new QLevelDBOptions object.
*/
QLevelDBOptions::QLevelDBOptions(QObject *parent)
    : QObject(parent)
    , m_createIfMissing(true)
    , m_errorIfExists(false)
    , m_paranoidChecks(false)
    , m_compressionType(CompressionType::SnappyCompression)
{
}

/*!
    If true creates a new database if the specified one does no exist.
*/
bool QLevelDBOptions::createIfMissing() const
{
    return m_createIfMissing;
}

/*!
    If true QLevelDB::open() raises an error if database already exists
*/
bool QLevelDBOptions::errorIfExists() const
{
    return m_errorIfExists;
}

/*!
    Increase leveldb checks and raise an error as soon as it detects an internal corruption. In case of corruption
    of data, QLevelDB::repairDB() might recover the data
*/
bool QLevelDBOptions::paranoidChecks() const
{
    return m_paranoidChecks;
}

/*!
    Indicates whether snappy compression should be used of not
*/
QLevelDBOptions::CompressionType QLevelDBOptions::compressionType() const
{
    return m_compressionType;
}

void QLevelDBOptions::setCreateIfMissing(bool value)
{
    m_createIfMissing = value;
}

void QLevelDBOptions::setErrorIfExists(bool value)
{
    m_errorIfExists = value;
}

void QLevelDBOptions::setParanoidChecks(bool value)
{
    m_paranoidChecks = value;
}

void QLevelDBOptions::setCompressionType(QLevelDBOptions::CompressionType type)
{
    m_compressionType = type;
}

QT_END_NAMESPACE

