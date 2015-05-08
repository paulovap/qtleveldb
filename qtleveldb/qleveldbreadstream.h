#ifndef QLEVELDBREADSTREAM_H
#define QLEVELDBREADSTREAM_H

#include <QObject>
#include <QVariant>
#include <qsharedpointer.h>
#include <leveldb/iterator.h>
#include <leveldb/db.h>
#include <QJSValue>
//TODO: expose to QML
class QLevelDBReadStream : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString startKey READ startKey WRITE setStartKey FINAL)
    Q_PROPERTY(QString endKey READ endKey WRITE setEndKey FINAL)
public:
    explicit QLevelDBReadStream(QWeakPointer<leveldb::DB> db, QObject *parent = 0);

    Q_INVOKABLE bool start(QJSValue callback = QJSValue());
    Q_INVOKABLE void stop();

    QString startKey() const;
    void setStartKey(QString key);
    QString endKey() const;
    void setEndKey(QString value);
signals:
    void nextKeyValue(QString key, QVariant value);
public slots:
private:
    bool m_shouldStop;
    QString m_startKey;
    QString m_endKey;
    QWeakPointer<leveldb::DB> m_db;
};

#endif // QLEVELDBREADSTREAM_H
