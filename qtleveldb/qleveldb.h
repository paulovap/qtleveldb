#ifndef QLEVELDB_H
#define QLEVELDB_H

#include <QObject>
#include <QUrl>
#include <leveldb/db.h>
#include <QtQml>
#include <QtQml/qqmlparserstatus.h>
#include "qleveldboptions.h"
//#include "qleveldbbatch.h"

class QLevelDBBatch;
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

    Q_INVOKABLE QLevelDBBatch* batch();
    Q_INVOKABLE bool del(QString key);

    Q_INVOKABLE QVariant get(QString key, QVariant defaultValue = QVariant());
    Q_INVOKABLE bool put(QString key, QVariant value);
    Q_INVOKABLE bool putSync(QString key, QVariant value);
    Q_INVOKABLE bool destroyDB(QUrl path);
    Q_INVOKABLE bool repairDB(QUrl path);
    void emitPropertyChange(QString key, QVariant value);

signals:
    void openedChanged();
    void sourceChanged();
    void statusChanged();
    void statusTextChanged();
    void propertyChanged(QString key, QVariant value);
protected:
    void classBegin();
    void componentComplete();
    void reset();
    QSharedPointer<leveldb::DB> m_levelDB;
private slots:
    void onBatchWritten(QSet<QString> keys);
private:
    Q_DISABLE_COPY(QLevelDB)
    QLevelDBBatch *m_batch;

    QLevelDBOptions m_options;
    bool m_initialized;
    bool m_opened;
    QUrl m_source;
    Status m_status;
    QString m_statusText;

    void setStatus(Status status);
    void setStatusText(QString text);
    void setOpened(bool opened);
    bool openDatabase(QString localPath);
    Status parseStatusCode(leveldb::Status &status);
    void dispatchPropertyChange(QString key, QVariant value);
};

QML_DECLARE_TYPE(QLevelDB)
#endif // QLEVELDB_H
