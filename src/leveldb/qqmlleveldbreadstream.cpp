#include "qqmlleveldbreadstream.h"
#include <QtGlobal>
#include <QJSValueList>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDebug>
QT_BEGIN_NAMESPACE

QQmlLevelDBReadStream::QQmlLevelDBReadStream(QWeakPointer<leveldb::DB> db, QObject *parent)
    : QLevelDBReadStream(db, parent)
{
    connect(this, &QQmlLevelDBReadStream::nextKeyValue, this, &QQmlLevelDBReadStream::onNextKeyValue);
}

QQmlLevelDBReadStream::QQmlLevelDBReadStream(QWeakPointer<leveldb::DB> db, QString startKey, int length, QObject *parent)
    : QLevelDBReadStream(db, startKey, length, parent)
{
    connect(this, &QQmlLevelDBReadStream::nextKeyValue, this, &QQmlLevelDBReadStream::onNextKeyValue);
}

void QQmlLevelDBReadStream::start(QJSValue callback)
{
    m_callback = callback;
    QLevelDBReadStream::start();
}

void QQmlLevelDBReadStream::stop()
{
    QLevelDBReadStream::stop();
}

void QQmlLevelDBReadStream::classBegin()
{

}

void QQmlLevelDBReadStream::componentComplete()
{

}

void QQmlLevelDBReadStream::onNextKeyValue(QString key, QVariant value)
{
    if (m_callback.isCallable()){
        QJSValueList list;
        list << QJSValue(key);
        list << m_engine.toScriptValue<QVariant>(value);
        m_callback.call(list);
    }
}

QT_END_NAMESPACE
