#ifndef QLEVELDB_H
#define QLEVELDB_H

#include <QMutex>
#include <QObject>
#include <functional>
#include <QSharedPointer>
#include <QVariant>
#include <QString>

#include "qleveldbglobal.h"
#include "qleveldboptions.h"

QT_BEGIN_NAMESPACE

namespace leveldb {
class DB;
class Status;
}
class QLevelDBBatch;

class Q_LEVELDB_EXPORT QLevelDB : public QObject
{
    Q_OBJECT
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

    bool readStream(std::function<bool(QString, QVariant)> callback, const QString startKey = QString(), const int length = -1);

    void setFilename(QString filename);
    QLevelDBBatch* batch();
    bool del(QString key);
    QVariant get(QString key, QVariant defaultValue = QVariant());
    bool put(QString key, QVariant value);
    bool putSync(QString key, QVariant value);
    bool destroyDB(QString filename);
    bool repairDB(QString filename);

    QWeakPointer<leveldb::DB> dbNativeHandler();
signals:
    void filenameChanged();
    void openedChanged();
    void statusChanged();
    void lastErrorChanged();
    void keyValueChanged(QString key, QVariant value);
private slots:
    void onBatchWritten(QSet<QString> keys);
protected:
    QSharedPointer<leveldb::DB> m_levelDB;
private:
    Q_DISABLE_COPY(QLevelDB)
    QString m_filename;
    QLevelDBBatch *m_batch;
    QLevelDBOptions m_options;
    bool m_opened;
    Status m_status;
    QString m_lastError;
    QMutex m_mutex;

    void setStatus(Status status);
    void setLastError(QString text);
    void setOpened(bool opened);
    Status parseStatusCode(leveldb::Status &status);
    void dispatchPropertyChange(QString key, QVariant value);
};

QT_END_NAMESPACE

#endif // QLEVELDB_H
