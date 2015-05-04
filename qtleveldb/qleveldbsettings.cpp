#include "qleveldbsettings.h"
#include <QMetaObject>
#include <QMetaMethod>
#include <QStringBuilder>
#include <QJsonDocument>
#include <QJSValue>
#include <QStandardPaths>
#include "global.h"
/*!
  \qmltype Settings
  \inqmlmodule QtLevelDB
  \brief A high-level API to store properties in leveldb.

  This supose to behave the same ways as Settings in qt.labs.settings
*/
QLevelDBSettings::QLevelDBSettings(QObject *parent)
    : QLevelDB(parent)
{
    setSource(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/settings.db"));
    connect(this, &QLevelDBSettings::sourceChanged, this, &QLevelDBSettings::initProperties);
    connect(this, &QLevelDBSettings::propertyChanged, this, &QLevelDBSettings::onPropertyChanged);
}


void QLevelDBSettings::classBegin()
{
}

void QLevelDBSettings::componentComplete()
{
    QLevelDB::componentComplete();
    initProperties();
}

void QLevelDBSettings::initProperties()
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

        const QVariant value = get(property.name());
        const QVariant oldValue = property.read(this);
        if (value.isValid() && oldValue != value) {
            property.write(this, value);
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
void QLevelDBSettings::onMyPropertyChanged()
{
    if (m_levelDB.isNull())
        return;
    QMetaProperty property = m_connectedProperties[senderSignalIndex()];
    QString propertyName = property.name();
    QVariant value = property.read(this);
    put(propertyName, value);
}

void QLevelDBSettings::onPropertyChanged(QString key, QVariant value)
{
    Q_UNUSED(value);
    const QMetaObject *ob = metaObject();
    int i = ob->indexOfProperty(key.toLocal8Bit().constData());
    QMetaProperty prop = ob->property(i);
    prop.write(this, value);
}

