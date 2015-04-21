#ifndef QLEVELDB_H
#define QLEVELDB_H

#include <QObject>
#include <QUrl>
#include <leveldb/db.h>
#include <QtQml>
#include <QtQml/qqmlparserstatus.h>

class QLevelDBOptions : public QObject
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
    //Q_DISABLE_COPY(QLevelDBOptions)
    leveldb::Options m_options;
};

class QLevelDB : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
    Q_PROPERTY(bool opened READ opened NOTIFY openedChanged)
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QLevelDBOptions *options READ options)
    Q_ENUMS(Status)

public:
    enum Status {
      Undefined = -1,
      Ok = 0,
      NotFound = 1,
      Corruption = 2,
      NotSupported = 3,
      InvalidArgument = 4,
      IOError = 5
    };

    explicit QLevelDB(QObject *parent = 0);
    ~QLevelDB();

    bool opened() const;
    QUrl source() const;
    Status status() const;
    QString statusText() const;

    void setSource(QUrl source);
    QLevelDBOptions *options();

    Q_INVOKABLE Status del(QString key);
    Q_INVOKABLE Status put(QString key, QString value);
    Q_INVOKABLE Status putSync(QString key, QString value);
    Q_INVOKABLE void get(QString key, const QJSValue &callback=QJSValue::UndefinedValue);
    Q_INVOKABLE Status destroyDB(QUrl path);
    Q_INVOKABLE Status repairDB(QUrl path);
signals:
    void openedChanged();
    void sourceChanged();
    void statusChanged();
    void statusTextChanged();
protected:
    void classBegin();
    void componentComplete();
private:
    Q_DISABLE_COPY(QLevelDB)
    leveldb::DB *m_levelDB;
    QLevelDBOptions m_options;
    bool m_opened;
    QUrl m_source;
    Status m_status;
    QString m_statusText;

    void setStatus(Status status);
    void setStatusText(QString text);
    void setOpened(bool opened);
    bool openDatabase(QString localPath);
    void reset();
    Status parseStatusCode(leveldb::Status &status);
};

QML_DECLARE_TYPE(QLevelDB)
QML_DECLARE_TYPE(QLevelDBOptions)
#endif // QLEVELDB_H
