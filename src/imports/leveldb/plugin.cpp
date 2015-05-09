#include <QtQml/qqmlextensionplugin.h>
#include <QtQml>
#include <qleveldb.h>
#include <qleveldbbatch.h>
#include <qleveldboptions.h>
#include <qleveldbsettings.h>
#include <qleveldbreadstream.h>

QT_BEGIN_NAMESPACE

class QtLevelDBPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    virtual void registerTypes(const char *uri) Q_DECL_OVERRIDE
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtLevelDB"));

        qmlRegisterType<QLevelDB>(uri, 1, 0, "LevelDB");
        qmlRegisterType<QLevelDBSettings>(uri, 1, 0, "Settings");
        qmlRegisterUncreatableType<QLevelDBOptions>(uri, 1, 0, "Options", QObject::tr("Cannot create separate instance of Options"));
        qmlRegisterUncreatableType<QLevelDBBatch>(uri, 1, 0, "Batch", QObject::tr("Cannot create separate instance of Batch"));
        qmlRegisterUncreatableType<QLevelDBReadStream>(uri, 1, 0, "LevelDBReadStream", QObject::tr("Cannot create separate instance of LevelDBReadStream"));
    }
};

QT_END_NAMESPACE

#include "plugin.moc"
