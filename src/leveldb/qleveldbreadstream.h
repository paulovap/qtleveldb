#ifndef QLEVELDBREADSTREAM_H
#define QLEVELDBREADSTREAM_H
#include <QObject>
#include <QVariant>
#include <QJSValue>
#include <qsharedpointer.h>
#include <qleveldbglobal.h>
#include <../3rdparty/leveldb/include/leveldb/iterator.h>
#include <../3rdparty/leveldb/include/leveldb/db.h>


QT_BEGIN_NAMESPACE

class Q_LEVELDB_EXPORT QLevelDBReadStream : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString startKey READ startKey WRITE setStartKey FINAL)
    Q_PROPERTY(QString endKey READ endKey WRITE setEndKey FINAL)
public:
    explicit QLevelDBReadStream(QWeakPointer<leveldb::DB> db, QObject *parent = 0);
    ~QLevelDBReadStream();
    bool start();
    void stop();

    QString startKey() const;
    void setStartKey(QString key);
    QString endKey() const;
    void setEndKey(QString value);
signals:
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
