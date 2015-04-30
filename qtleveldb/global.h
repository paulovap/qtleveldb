#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QVariant>

QString variantToJson(QVariant variant);
QJsonObject variantToJsonObject(QVariant variant);
QVariant jsonToVariant(QString json);
QVariant jsonToVariant(QJsonObject object);

#endif // GLOBAL_H

