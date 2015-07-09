#ifndef QQMLLEVELDB_H
#define QQMLLEVELDB_H
#include <QObject>
#include <QUrl>
#include <QJSEngine>
#include <QtQml>
#include <QtQml/qqmlparserstatus.h>

#include "qleveldbglobal.h"
#include "qleveldboptions.h"
#include "qleveldb.h"

QT_BEGIN_NAMESPACE

class QLevelDBBatch;

class Q_LEVELDB_EXPORT QQmlLevelDB : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QLevelDB::Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
    Q_PROPERTY(bool opened READ opened NOTIFY openedChanged)
    Q_PROPERTY(QLevelDBOptions* options READ options)
    Q_INTERFACES(QQmlParserStatus)
    Q_ENUMS(QLevelDB::Status)
public:
    explicit QQmlLevelDB(QObject *parent = nullptr);

    QUrl source();
    void setSource(QUrl source);

    QLevelDB::Status status();
    QString lastError();
    bool opened();
    QLevelDBOptions* options();

    Q_INVOKABLE QLevelDBBatch* batch();
    Q_INVOKABLE bool del(QString key);
    Q_INVOKABLE QVariant get(QString key, QVariant defaultValue = QVariant());
    Q_INVOKABLE bool put(QString key, QVariant value);
    Q_INVOKABLE bool putSync(QString key, QVariant value);
    Q_INVOKABLE bool destroyDB(QUrl url);
    Q_INVOKABLE bool repairDB(QUrl url);
    Q_INVOKABLE bool readStream(QJSValue callback, QString startKey = QString(), int length = -1);
signals:
    void sourceChanged();
    void statusChanged();
    void lastErrorChanged();
    void openedChanged();
    void keyValueChanged(QString key, QVariant value);
protected:
    void classBegin();
    void componentComplete();
private:
    Q_DISABLE_COPY(QQmlLevelDB)
    bool m_initialized;
    QUrl m_source;
    QLevelDB m_leveldb;
};

QT_END_NAMESPACE
QML_DECLARE_TYPE(QQmlLevelDB)

#endif // QQMLLEVELDB_H
