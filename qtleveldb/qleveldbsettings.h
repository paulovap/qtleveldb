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
#include <leveldb/db.h>


class QLevelDBSettings : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_INTERFACES(QQmlParserStatus)
public:
    explicit QLevelDBSettings(QObject *parent = 0);

    QUrl source() const;
    void setSource(QUrl source);
protected:
    void classBegin();
    void componentComplete();
signals:
    void sourceChanged();
public slots:

private:
    Q_DISABLE_COPY(QLevelDBSettings)

    QList<QMetaObject::Connection> m_connections;
    QHash<int, QMetaProperty> m_connectedProperties;
    leveldb::DB *m_levelDB;
    bool m_opened;
    QUrl m_source;
    void reset();
    bool openDatabase(QString localPath);

    void initProperties();
private slots:
    void onPropertyChanged();

    QVariant getDB(QString key);
    void putDB(QString key, QVariant value);
};

QML_DECLARE_TYPE(QLevelDBSettings)
#endif // QLEVELDBSETTINGS_H
