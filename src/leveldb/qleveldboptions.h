#ifndef QLEVELDBOPTIONS_H
#define QLEVELDBOPTIONS_H
#include "qleveldbglobal.h"
#include <QObject>

QT_BEGIN_NAMESPACE

namespace leveldb {
class DB;
struct Options;
}

class Q_LEVELDB_EXPORT QLevelDBOptions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool createIfMissing READ createIfMissing WRITE setCreateIfMissing)
    Q_PROPERTY(bool errorIfExists READ errorIfExists WRITE setErrorIfExists)
    Q_PROPERTY(bool paranoidChecks READ paranoidChecks WRITE setParanoidChecks)
    Q_PROPERTY(CompressionType compressionType READ compressionType WRITE setCompressionType)
    Q_ENUMS(CompressionType)


public:
    explicit QLevelDBOptions(QObject *parent = 0);

    enum CompressionType{
        NoCompression     = 0x0,
        SnappyCompression = 0x1
    };

    bool createIfMissing() const;
    bool errorIfExists() const;
    bool paranoidChecks() const;
    CompressionType compressionType() const;

    void setCreateIfMissing(bool value);
    void setErrorIfExists(bool value);
    void setParanoidChecks(bool value);
    void setCompressionType(CompressionType type);
private:
    Q_DISABLE_COPY(QLevelDBOptions)
    bool m_createIfMissing;
    bool m_errorIfExists;
    bool m_paranoidChecks;
    CompressionType m_compressionType;
};

QT_END_NAMESPACE
#endif // QLEVELDBOPTIONS_H
