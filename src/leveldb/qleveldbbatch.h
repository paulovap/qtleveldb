#ifndef QLEVELDBBATCH_H
#define QLEVELDBBATCH_H

#include <QMutex>
#include <QObject>
#include <QSet>
#include "qleveldbglobal.h"
#include "qleveldb.h"

QT_BEGIN_NAMESPACE

namespace leveldb {
class DB;
class WriteBatch;
}

class Q_LEVELDB_EXPORT QLevelDBBatch : public QObject
{
    Q_OBJECT
public:
    explicit QLevelDBBatch(QWeakPointer<leveldb::DB> db, QObject *parent = 0);
    ~QLevelDBBatch();
    Q_INVOKABLE QLevelDBBatch* del(QString key);
    Q_INVOKABLE QLevelDBBatch* put(QString key, QVariant value);
    Q_INVOKABLE QLevelDBBatch* clear();
    //TODO: change it to LevelDB::Status
    Q_INVOKABLE bool write();
signals:
    void batchWritten(QSet<QString> keys);
private:
    Q_DISABLE_COPY(QLevelDBBatch)
    QSharedPointer<leveldb::DB> m_levelDB;
    leveldb::WriteBatch *m_writeBatch;
    QSet<QString> m_operations;
    QMutex m_mutex;
};

QT_END_NAMESPACE
#endif // QLEVELDBBATCH_H
