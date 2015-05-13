#ifndef QQMLLEVELDBSETTINGS_H
#define QQMLLEVELDBSETTINGS_H

#include <QObject>
#include <QUrl>
#include <QList>
#include <QMetaObject>
#include <QMetaProperty>
#include <QHash>
#include <QQmlParserStatus>
#include <QQmlEngine>
#include <QtQml>

#include "qqmlleveldb.h"
#include "qleveldbglobal.h"

QT_BEGIN_NAMESPACE
namespace leveldb {
class DB;
}
class Q_LEVELDB_EXPORT QQmlLevelDBSettings : public QQmlLevelDB
{
    Q_OBJECT

public:
    explicit QQmlLevelDBSettings(QObject *parent = 0);
protected:
    void classBegin();
    void componentComplete();
private:
    Q_DISABLE_COPY(QQmlLevelDBSettings)

    QList<QMetaObject::Connection> m_connections;
    QHash<int, QMetaProperty> m_connectedProperties;
    void initProperties();
private slots:
    void onMyPropertyChanged();
    void onPropertyChanged(QString key, QVariant value);

};

QT_END_NAMESPACE
QML_DECLARE_TYPE(QQmlLevelDBSettings)
#endif // QQMLLEVELDBSETTINGS_H
