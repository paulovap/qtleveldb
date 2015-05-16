#include "qleveldbglobal.h"
#include "qqmlleveldbsettings.h"
#include <QMetaObject>
#include <QMetaMethod>
#include <QStringBuilder>
#include <QJsonDocument>
#include <QJSValue>
#include <QStandardPaths>

QT_BEGIN_NAMESPACE

/*!
  \qmltype Settings
  \inqmlmodule QtLevelDB
  \brief A high-level API to store properties in leveldb.

  This supose to behave the same ways as Settings in qt.labs.settings
*/
QQmlLevelDBSettings::QQmlLevelDBSettings(QObject *parent)
    : QQmlLevelDB(parent)
{
    setSource(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QStringLiteral("/settings.db")));
    connect(this, &QQmlLevelDBSettings::sourceChanged, this, &QQmlLevelDBSettings::initProperties);
    connect(this, &QQmlLevelDBSettings::keyValueChanged, this, &QQmlLevelDBSettings::onPropertyChanged);
}


void QQmlLevelDBSettings::classBegin()
{
}

void QQmlLevelDBSettings::componentComplete()
{
    QQmlLevelDB::componentComplete();
    initProperties();
}

void QQmlLevelDBSettings::initProperties()
{
    for(auto connection : m_connections){
        QObject::disconnect(connection);
    }
    m_connections.clear();
    m_connectedProperties.clear();

    const QMetaObject *mo = metaObject();
    const int slotIndex = mo->indexOfSlot("onMyPropertyChanged()");
    const int offset = mo->propertyOffset();
    const int count = mo->propertyCount();
    for (int i = offset; i < count; ++i) {
        QMetaProperty property = mo->property(i);

        if (!property.isWritable()){
            //we ignore read only properties
            continue;
        }
        const QVariant value = get(QString::fromLocal8Bit(property.name()));
        const QVariant oldValue = property.read(this);

        if (value.isValid() && oldValue != value) {
            property.write(this, value);
        }else if (oldValue.isValid()){
            //qml static initialization
            put(QString::fromLocal8Bit(property.name()), oldValue);
        }

        if (property.hasNotifySignal()){
            m_connectedProperties.insert(property.notifySignalIndex(), property);
            m_connections.append(QMetaObject::connect(this ,property.notifySignalIndex(),
                                                      this, slotIndex));
        }
    }
}

/**
 * Updating database with the new property value.
 * TODO: synchronize all instances of Settings with the new value
 */
void QQmlLevelDBSettings::onMyPropertyChanged()
{
    if (!opened())
        return;
    QMetaProperty property = m_connectedProperties[senderSignalIndex()];
    QString propertyName = QString::fromLocal8Bit(property.name());
    QVariant value = property.read(this);
    put(propertyName, value);
}

void QQmlLevelDBSettings::onPropertyChanged(QString key, QVariant value)
{
    Q_UNUSED(value);
    const QMetaObject *ob = metaObject();
    int i = ob->indexOfProperty(key.toLocal8Bit().constData());
    QMetaProperty prop = ob->property(i);
    prop.write(this, value);
}

QT_END_NAMESPACE
