#include <QtQml/qqmlextensionplugin.h>
#include <QtQml>
#include "qleveldb.h"
QT_BEGIN_NAMESPACE

class QtLeveldbPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    virtual void registerTypes(const char *uri) Q_DECL_OVERRIDE
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtLevelDB"));

        qmlRegisterType<QLevelDB>(uri, 1, 0, "LevelDB");
        qmlRegisterUncreatableType<Options>(uri, 1, 0, "Options", QObject::tr("Cannot create separate instance of Options"));
//        qmlRegisterUncreatableType<QQuickWebEngineLoadRequest>(uri, 1, 0, "WebEngineLoadRequest", QObject::tr("Cannot create separate instance of WebEngineLoadRequest"));
//        qmlRegisterUncreatableType<QQuickWebEngineNavigationRequest>(uri, 1, 0, "WebEngineNavigationRequest", QObject::tr("Cannot create separate instance of WebEngineNavigationRequest"));

//        qmlRegisterType<QQuickWebEngineView, 1>(uri, 1, 1, "WebEngineView");
//        qmlRegisterType<QQuickWebEngineProfile>(uri, 1, 1, "WebEngineProfile");
//        qmlRegisterType<QQuickWebEngineScript>(uri, 1, 1, "WebEngineScript");
//        qmlRegisterUncreatableType<QQuickWebEngineCertificateError>(uri, 1, 1, "WebEngineCertificateError", QObject::tr("Cannot create separate instance of WebEngineCertificateError"));
//        qmlRegisterUncreatableType<QQuickWebEngineDownloadItem>(uri, 1, 1, "WebEngineDownloadItem",
//            QObject::tr("Cannot create a separate instance of WebEngineDownloadItem"));
//        qmlRegisterUncreatableType<QQuickWebEngineNewViewRequest>(uri, 1, 1, "WebEngineNewViewRequest", QObject::tr("Cannot create separate instance of WebEngineNewViewRequest"));
//        qmlRegisterUncreatableType<QQuickWebEngineSettings>(uri, 1, 1, "WebEngineSettings", QObject::tr("Cannot create a separate instance of WebEngineSettings"));
//        qmlRegisterSingletonType<QQuickWebEngineSingleton>(uri, 1, 1, "WebEngine", webEngineSingletonProvider);
//        qmlRegisterUncreatableType<QQuickWebEngineHistory>(uri, 1, 1, "NavigationHistory",
//            QObject::tr("Cannot create a separate instance of NavigationHistory"));
//        qmlRegisterUncreatableType<QQuickWebEngineHistoryListModel>(uri, 1, 1, "NavigationHistoryListModel",
//            QObject::tr("Cannot create a separate instance of NavigationHistory"));
//        qmlRegisterUncreatableType<QQuickWebEngineFullScreenRequest>(uri, 1, 1, "FullScreenRequest",
//            QObject::tr("Cannot create a separate instance of FullScreenRequest"));
    }
};

QT_END_NAMESPACE

#include "plugin.moc"
