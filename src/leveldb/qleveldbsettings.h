#ifndef QLEVELDBSETTINGS_H
#define QLEVELDBSETTINGS_H

#include <QObject>
#include <QUrl>
#include <QList>
#include <QMetaObject>
#include <QMetaProperty>
#include <QHash>
#include <QQmlParserStatus>
#include <QQmlEngine>
#include <QtQml>
#include <../3rdparty/leveldb/include/leveldb/db.h>
#include <QtLevelDB/QLevelDB>
#include <QtLevelDB/qleveldbglobal.h>

QT_BEGIN_NAMESPACE

class Q_LEVELDB_EXPORT QLevelDBSettings : public QLevelDB
{
    Q_OBJECT

public:
    explicit QLevelDBSettings(QObject *parent = 0);
protected:
    void classBegin();
    void componentComplete();
private:
    Q_DISABLE_COPY(QLevelDBSettings)

    QList<QMetaObject::Connection> m_connections;
    QHash<int, QMetaProperty> m_connectedProperties;
private slots:
    void initProperties();
private slots:
    void onMyPropertyChanged();
    void onPropertyChanged(QString key, QVariant value);

};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QLevelDBSettings)
#endif // QLEVELDBSETTINGS_H
