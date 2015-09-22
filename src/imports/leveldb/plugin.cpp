#include <qqml.h>
#include <QtQml/QQmlExtensionPlugin>
#include <qleveldbbatch.h>
#include <qleveldboptions.h>
#include <qqmlleveldbsettings.h>
#include <qqmlleveldb.h>

QT_BEGIN_NAMESPACE

class QtLevelDBPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtLevelDB"));

        qmlRegisterType<QQmlLevelDB>(uri, 1, 0, "LevelDB");
        qmlRegisterType<QQmlLevelDBSettings>(uri, 1, 0, "Settings");
        qmlRegisterUncreatableType<QLevelDBOptions>(uri, 1, 0, "Options", QObject::tr("Cannot create separate instance of Options"));
        qmlRegisterUncreatableType<QLevelDBBatch>(uri, 1, 0, "Batch", QObject::tr("Cannot create separate instance of Batch"));
    }
};

QT_END_NAMESPACE

#include "plugin.moc"
