#ifndef QQMLLEVELDBREADSTREAM_H
#define QQMLLEVELDBREADSTREAM_H

#include <QObject>
#include <QJSValue>
#include <QJSEngine>
#include <QWeakPointer>
#include <qqmlparserstatus.h>
#include <QtQml>

#include "qleveldbglobal.h"
#include "qleveldbreadstream.h"

QT_BEGIN_NAMESPACE

namespace leveldb {
class DB;
}
class Q_LEVELDB_EXPORT QQmlLevelDBReadStream : public QLevelDBReadStream, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
public:
    explicit QQmlLevelDBReadStream(QWeakPointer<leveldb::DB> db, QObject *parent = 0);

    Q_INVOKABLE void start(QJSValue callback);
    Q_INVOKABLE void stop();
protected:
    void classBegin();
    void componentComplete();
private slots:
    void onNextKeyValue(QString key, QVariant value);
private:
    Q_DISABLE_COPY(QQmlLevelDBReadStream)
    QJSEngine m_jsEngine;
    QJSValue m_callback;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QQmlLevelDBReadStream)
#endif // QQMLLEVELDBREADSTREAM_H
