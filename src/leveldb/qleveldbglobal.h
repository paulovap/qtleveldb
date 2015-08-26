#ifndef QTLEVELDB_H
#define QTLEVELDB_H

#include <QtCore/qglobal.h>
#include <QtCore>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_LEVELDB_LIB)
#      define Q_LEVELDB_EXPORT Q_DECL_EXPORT
#    else
#      define Q_LEVELDB_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_LEVELDB_EXPORT
#endif

QString variantToJson(QVariant variant);
QJsonObject variantToJsonObject(QVariant variant);
QVariant jsonToVariant(QString json);
QVariant jsonToVariant(QJsonObject object);

QT_END_NAMESPACE

#endif // QTLEVELDB_H
