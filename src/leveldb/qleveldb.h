#ifndef QLEVELDB_H
#define QLEVELDB_H

#include <QObject>

#include "qleveldbglobal.h"
#include "qleveldboptions.h"
#include "qleveldbreadstream.h"

QT_BEGIN_NAMESPACE

namespace leveldb {
class DB;
class Status;
}
class QLevelDBBatch;

class Q_LEVELDB_EXPORT QLevelDB : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename NOTIFY filenameChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
    Q_PROPERTY(bool opened READ opened NOTIFY openedChanged)
    Q_PROPERTY(QLevelDBOptions* options READ options)
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
    explicit QLevelDB(QString filename, QObject *parent = 0);
    ~QLevelDB();

    QString filename();
    bool opened();
    Status status();
    QString lastError();
    QLevelDBOptions *options();

    Status open();
    void close();

    QLevelDBReadStream* readStream(const QString startKey =  QString(), const QString endKey = QString());

    Q_INVOKABLE QLevelDBBatch* batch();
    Q_INVOKABLE bool del(QString key);
    Q_INVOKABLE QVariant get(QString key, QVariant defaultValue = QVariant());
    Q_INVOKABLE bool put(QString key, QVariant value);
    Q_INVOKABLE bool putSync(QString key, QVariant value);
    Q_INVOKABLE bool destroyDB(QString filename);
    Q_INVOKABLE bool repairDB(QString filename);
signals:
    void filenameChanged();
    void openedChanged();
    void statusChanged();
    void lastErrorChanged();
    void keyValueChanged(QString key, QVariant value);
private slots:
    void onBatchWritten(QSet<QString> keys);
protected:
    void setFilename(QString filename);
    QSharedPointer<leveldb::DB> m_levelDB;
private:
    Q_DISABLE_COPY(QLevelDB)
    QString m_filename;
    QLevelDBBatch *m_batch;
    QLevelDBOptions m_options;
    bool m_opened;
    Status m_status;
    QString m_lastError;

    void setStatus(Status status);
    void setLastError(QString text);
    void setOpened(bool opened);
    Status parseStatusCode(leveldb::Status &status);
    void dispatchPropertyChange(QString key, QVariant value);
};

QT_END_NAMESPACE

#endif // QLEVELDB_H
