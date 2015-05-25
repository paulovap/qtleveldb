#ifndef QLEVELDBREADSTREAM_H
#define QLEVELDBREADSTREAM_H
#include <QObject>
#include <QVariant>
#include <qsharedpointer.h>
#include <functional>
#include "qleveldbglobal.h"

QT_BEGIN_NAMESPACE

namespace leveldb {
class DB;
class Iterator;
class Comparator;
}

class Q_LEVELDB_EXPORT QLevelDBReadStream : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString startKey READ startKey FINAL)
public:
    explicit QLevelDBReadStream(QWeakPointer<leveldb::DB> db, QObject *parent = 0);
    QLevelDBReadStream(QWeakPointer<leveldb::DB> db, QString startKey, int length = -1, QObject *parent = 0);
    ~QLevelDBReadStream();
    bool start();
    bool start(std::function<bool (QString key, QVariant value)> callback);
    void stop();

    QString startKey() const;
    signals:
    void streamStarted();
    void streamEnded();
    void nextKeyValue(QString key, QVariant value);
public slots:
private:
    Q_DISABLE_COPY(QLevelDBReadStream)
    bool m_shouldStop;
    QString m_startKey;
    int m_length;
    QWeakPointer<leveldb::DB> m_db;
    const leveldb::Comparator* m_byteWiseComparator;
};

QT_END_NAMESPACE

#endif // QLEVELDBREADSTREAM_H
