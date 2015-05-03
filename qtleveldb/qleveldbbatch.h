#ifndef QLEVELDBBATCH_H
#define QLEVELDBBATCH_H

#include <QObject>
#include <QtQml>
#include <QQmlParserStatus>
#include "qleveldb.h"
#include <leveldb/write_batch.h>
#include <leveldb/db.h>

class QLevelDBBatch : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
public:
    explicit QLevelDBBatch(QSharedPointer<leveldb::DB> db, QObject *parent = 0);

    Q_INVOKABLE QLevelDBBatch* del(QString key);
    Q_INVOKABLE QLevelDBBatch* put(QString key, QVariant value);
    Q_INVOKABLE QLevelDBBatch* clear();
    //TODO: change it to LevelDB::Status
    Q_INVOKABLE bool write();
signals:
    void batchWritten(QSet<QString> keys);
protected:
    void classBegin();
    void componentComplete();

private:
    QSharedPointer<leveldb::DB> m_levelDB;
    leveldb::WriteBatch m_writeBatch;
    QSet<QString> m_operations;
};

QML_DECLARE_TYPE(QLevelDBBatch)
#endif // QLEVELDBBATCH_H
