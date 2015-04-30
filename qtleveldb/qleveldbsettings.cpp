#include "qleveldbsettings.h"
#include <QMetaObject>
#include <QMetaMethod>
#include <QStringBuilder>
#include <QJsonDocument>
#include <QJSValue>

#include "global.h"
/*!
  \qmltype Settings
  \inqmlmodule QtLevelDB
  \brief A high-level API to store properties in leveldb.

  This supose to behave the same ways as Settings in qt.labs.settings
*/
QLevelDBSettings::QLevelDBSettings(QObject *parent)
    : QObject(parent)
    , m_levelDB(nullptr)
    , m_opened(false)
{
}


void QLevelDBSettings::classBegin()
{
}

void QLevelDBSettings::componentComplete()
{
    openDatabase(m_source.toLocalFile());
    initProperties();
}

/*!
  \qmlproperty url Settings::source
  \brief url that points to the leveldb database folder.

  For now it's only possible to use local file paths.
*/
QUrl QLevelDBSettings::source() const
{
    return m_source;
}

void QLevelDBSettings::setSource(QUrl source)
{
    if(m_source != source){
        m_source = source;
        emit sourceChanged();
        openDatabase(source.toLocalFile());
        initProperties();
    }
}

void QLevelDBSettings::reset()
{
    if (m_levelDB)
        delete m_levelDB;
}

bool QLevelDBSettings::openDatabase(QString localPath)
{
    if (m_levelDB)
        reset();
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options,
                                               localPath.toStdString(),
                                               &m_levelDB);
    m_opened = status.ok();
    return status.ok();
}

void QLevelDBSettings::initProperties()
{
    for(auto connection : m_connections){
        disconnect(connection);
    }
    m_connections.clear();
    m_connectedProperties.clear();

    const QMetaObject *mo = metaObject();
    const int slotIndex = mo->indexOfSlot("onPropertyChanged()");
    const int offset = mo->propertyOffset();
    const int count = mo->propertyCount();
    for (int i = offset; i < count; ++i) {
        QMetaProperty property = mo->property(i);

        const QVariant value = getDB(property.name());
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
void QLevelDBSettings::onPropertyChanged()
{
    if (!m_levelDB)
        return;
    QMetaProperty property = m_connectedProperties[senderSignalIndex()];
    QString propertyName = property.name();
    QVariant value = property.read(this);
    putDB(propertyName, value);
}


QVariant QLevelDBSettings::getDB(QString key)
{
    if(!m_levelDB || !m_opened)
        return QVariant();
    std::string value;
    leveldb::Status status = m_levelDB->Get(leveldb::ReadOptions(),
                                            leveldb::Slice(key.toStdString()),
                                            &value);
    if (status.ok())
        return jsonToVariant(QString::fromStdString(value));
    return QVariant();
}

void QLevelDBSettings::putDB(QString key, QVariant value)
{
    if(!m_levelDB || !m_opened)
        return;
    QString valueJson = variantToJson(value);
    m_levelDB->Put(leveldb::WriteOptions(),
                   leveldb::Slice(key.toStdString()),
                   leveldb::Slice(valueJson.toStdString()));
}

