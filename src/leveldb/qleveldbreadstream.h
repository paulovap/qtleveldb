#ifndef QLEVELDBREADSTREAM_H
#define QLEVELDBREADSTREAM_H
#include <QObject>
#include <QVariant>
#include <qsharedpointer.h>
#include "qleveldbglobal.h"

QT_BEGIN_NAMESPACE

namespace leveldb {
class DB;
class Iterator;
}

class Q_LEVELDB_EXPORT QLevelDBReadStream : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString startKey READ startKey FINAL)
    Q_PROPERTY(QString endKey READ endKey FINAL)
public:
    explicit QLevelDBReadStream(QWeakPointer<leveldb::DB> db, QObject *parent = 0);
    QLevelDBReadStream(QWeakPointer<leveldb::DB> db, QString startKey, QString endKey = QString(), QObject *parent = 0);
    ~QLevelDBReadStream();
    bool start();
    void stop();

    QString startKey() const;
    QString endKey() const;
    signals:
    void streamStarted();
    void streamEnded();
    void nextKeyValue(QString key, QVariant value);
public slots:
private:
    Q_DISABLE_COPY(QLevelDBReadStream)
    bool m_shouldStop;
    QString m_startKey;
    QString m_endKey;
    QWeakPointer<leveldb::DB> m_db;
};

QT_END_NAMESPACE

#endif // QLEVELDBREADSTREAM_H
