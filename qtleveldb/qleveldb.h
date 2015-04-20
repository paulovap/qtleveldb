#ifndef QLEVELDB_H
#define QLEVELDB_H

#include <QObject>
#include <QUrl>
#include <leveldb/db.h>
class Options;

class QLevelDB : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(Options *options READ options)
public:
    explicit QLevelDB(QObject *parent = 0);

    QUrl source() const;
    void setSource(QUrl source);

    Options *options() const;
signals:
    void sourceChanged();
public slots:

private:
    QUrl m_source;
    Options *m_options;
    leveldb::DB *m_levelDb;

    bool openDatabase(QString localPath);
};

class Options : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool createIfMissing READ createIfMissing WRITE setCreateIfMissing)
    Q_PROPERTY(bool errorIfExists READ errorIfExists WRITE setErrorIfExists)
    Q_PROPERTY(bool paranoidChecks READ paranoidChecks WRITE setParanoidChecks)
    Q_PROPERTY(CompressionType compressionType READ compressionType WRITE setCompressionType)
    Q_ENUMS(CompressionType)

public:
    explicit Options(QObject *parent = 0);

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

private:
    leveldb::Options m_options;
};

#endif // QLEVELDB_H
