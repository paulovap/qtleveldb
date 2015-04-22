#ifndef QLEVELDBOPTIONS_H
#define QLEVELDBOPTIONS_H

#include <QObject>
#include <QQmlParserStatus>
#include <QtQml>
#include <leveldb/db.h>

class QLevelDBOptions : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
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

    leveldb::Options leveldbOptions() const;
    bool createIfMissing() const;
    bool errorIfExists() const;
    bool paranoidChecks() const;
    CompressionType compressionType() const;

    void setCreateIfMissing(bool value);
    void setErrorIfExists(bool value);
    void setParanoidChecks(bool value);
    void setCompressionType(CompressionType type);
protected:
    void classBegin();
    void componentComplete();
private:
    //Q_DISABLE_COPY(QLevelDBOptions)
    leveldb::Options m_options;
};

//QML_DECLARE_TYPE(QLevelDBOptions)
#endif // QLEVELDBOPTIONS_H
