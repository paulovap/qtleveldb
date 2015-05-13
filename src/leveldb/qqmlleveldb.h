#ifndef QQMLLEVELDB_H
#define QQMLLEVELDB_H
#include <QObject>
#include <QUrl>
#include <QtQml>
#include <QtQml/qqmlparserstatus.h>

#include "qleveldbglobal.h"
#include "qleveldboptions.h"
#include "qleveldbreadstream.h"
#include "qleveldb.h"
#include "qqmlleveldbreadstream.h"

QT_BEGIN_NAMESPACE

class QLevelDBBatch;

class Q_LEVELDB_EXPORT QQmlLevelDB : public QLevelDB, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QLevelDBOptions* options READ options)
    Q_INTERFACES(QQmlParserStatus)
public:
    explicit QQmlLevelDB(QObject *parent = nullptr);

    QUrl source();
    void setSource(QUrl source);
    Q_INVOKABLE QQmlLevelDBReadStream* readStream(QString startKey = QString(), QString endKey = QString());
signals:
    void sourceChanged();
protected:
    void classBegin();
    void componentComplete();
private:
    Q_DISABLE_COPY(QQmlLevelDB)
    bool m_initialized;
    QUrl m_source;
};

QT_END_NAMESPACE
QML_DECLARE_TYPE(QQmlLevelDB)

#endif // QQMLLEVELDB_H
