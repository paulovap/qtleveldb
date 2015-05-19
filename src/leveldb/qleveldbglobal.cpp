
#include <QtLevelDB/qleveldbglobal.h>
#include <QtCore>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJSValue>
#include <QString>

QT_BEGIN_NAMESPACE

QString variantToJson(const QVariant variant)
{
    QJsonDocument doc= QJsonDocument(variantToJsonObject(variant));
    QString result = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    return result;
}

QJsonObject variantToJsonObject(QVariant variant)
{
    QJsonObject doc;
    QVariant object = variant;

    //convert from JSValue to QVariantList or QVarianMap
    if (object.userType() == qMetaTypeId<QJSValue>()){
        QJSValue value = variant.value<QJSValue>();
        object = value.toVariant();
    }

    switch (object.type()) {
    case QVariant::Invalid:
        doc.insert(QStringLiteral("type"), QStringLiteral("invalid"));
        break;
    case QVariant::Uuid:
        doc.insert(QStringLiteral("type"), QStringLiteral("uuid"));
        doc.insert(QStringLiteral("data"), QJsonValue::fromVariant(object.toUuid().toString()));
        break;
    case QVariant::Url:
    case QVariant::String:
    case QVariant::LongLong:
    case QVariant::ULongLong:
    case QVariant::Int:
    case QVariant::UInt:
    case QVariant::Bool:
    case QVariant::Double:
    case QVariant::Char: {
        doc.insert(QStringLiteral("type"), QStringLiteral("primitive"));
        doc.insert(QStringLiteral("data"), QJsonValue::fromVariant(object));
        break;
    }
    case QVariant::ByteArray: {
        QByteArray a = object.toByteArray();
        doc.insert(QStringLiteral("type"), QStringLiteral("bytearray"));
        doc.insert(QStringLiteral("data"), QJsonValue(QString::fromUtf8(a)));
        break;
    }
    case QVariant::KeySequence:
        doc.insert(QStringLiteral("type"), QStringLiteral("keysequence"));
        doc.insert(QStringLiteral("data"), object.toString());
        break;
    case QVariant::Rect: {
        QJsonObject sub;
        QRect r = qvariant_cast<QRect>(object);
        sub.insert(QStringLiteral("x"), r.x());
        sub.insert(QStringLiteral("y"), r.y());
        sub.insert(QStringLiteral("width"), r.width());
        sub.insert(QStringLiteral("height"), r.height());
        doc.insert(QStringLiteral("type"), QStringLiteral("rect"));
        doc.insert(QStringLiteral("data"), sub);
        break;
    }
    case QVariant::RectF: {
        QRectF r = qvariant_cast<QRectF>(object);
        QJsonObject sub;
        sub.insert(QStringLiteral("x"), r.x());
        sub.insert(QStringLiteral("y"), r.y());
        sub.insert(QStringLiteral("width"), r.width());
        sub.insert(QStringLiteral("height"), r.height());
        doc.insert(QStringLiteral("type"), QStringLiteral("rectf"));
        doc.insert(QStringLiteral("data"),sub);
        break;
    }
    case QVariant::Size: {
        QSize s = qvariant_cast<QSize>(object);
        QJsonObject sub;
        sub.insert(QStringLiteral("width"), s.width());
        sub.insert(QStringLiteral("height"), s.height());
        doc.insert(QStringLiteral("type"), QStringLiteral("size"));
        doc.insert(QStringLiteral("data"), sub);
        break;
    }
    case QVariant::SizeF: {
        QSizeF s = qvariant_cast<QSizeF>(object);
        QJsonObject sub;
        sub.insert(QStringLiteral("width"), s.width());
        sub.insert(QStringLiteral("height"), s.height());
        doc.insert(QStringLiteral("type"), QStringLiteral("sizef"));
        break;
    }
    case QVariant::Point: {
        QPoint p = qvariant_cast<QPoint>(object);
        QJsonObject sub;
        sub.insert(QStringLiteral("x"), p.x());
        sub.insert(QStringLiteral("y"), p.y());
        doc.insert(QStringLiteral("type"), QStringLiteral("point"));
        doc.insert(QStringLiteral("data"), sub);
        break;
    }
    case QVariant::PointF: {
        QPointF p = qvariant_cast<QPointF>(object);
        QJsonObject sub;
        sub.insert(QStringLiteral("x"), p.x());
        sub.insert(QStringLiteral("y"), p.y());
        doc.insert(QStringLiteral("type"), QStringLiteral("pointf"));
        doc.insert(QStringLiteral("data"), sub);
        break;
    }
    case QMetaType::QDate: {
        QDate date = qvariant_cast<QDate>(object);
        QJsonObject sub;
        sub.insert(QStringLiteral("day"), date.day());
        sub.insert(QStringLiteral("month"), date.month());
        sub.insert(QStringLiteral("year"), date.year());
        doc.insert(QStringLiteral("type"), QStringLiteral("date"));
        doc.insert(QStringLiteral("data"), sub);
        break;
    }
    case QMetaType::QTime: {
        QTime time = qvariant_cast<QTime>(object);
        QJsonObject sub;
        sub.insert(QStringLiteral("hour"), time.hour());
        sub.insert(QStringLiteral("minute"), time.minute());
        sub.insert(QStringLiteral("second"), time.second());
        sub.insert(QStringLiteral("msec"), time.msec());
        doc.insert(QStringLiteral("type"), QStringLiteral("time"));
        doc.insert(QStringLiteral("data"), sub);
        break;
    }
    case QMetaType::QDateTime:{
        QDateTime datetime = qvariant_cast<QDateTime>(object);
        doc.insert(QStringLiteral("type"), QStringLiteral("datetime"));
        doc.insert(QStringLiteral("data"), datetime.toString(QStringLiteral("yyyy-MM-ddTHH:mm:ss.zzzTZD")));
        break;
    }
    case QMetaType::QVariantHash:
    case QMetaType::QVariantMap: {
        QVariantMap map = object.toMap();
        QJsonObject sub;
        for(auto key : map.keys()){
            sub.insert(key, variantToJsonObject(map[key]));
        }
        doc.insert(QStringLiteral("type"), QStringLiteral("object"));
        doc.insert(QStringLiteral("data"), sub);
        break;
    }
    case QMetaType::QVariantList: {
        QVariantList list = object.toList();
        QJsonArray ary;
        for(auto elem : list){
            ary.append(variantToJsonObject(elem));
        }
        doc.insert(QStringLiteral("type"), QStringLiteral("list"));
        doc.insert(QStringLiteral("data"), ary);
        break;
    }

    default: {
#ifndef QT_NO_DATASTREAM
        QByteArray a;
        {
            QDataStream s(&a, QIODevice::WriteOnly);
            s.setVersion(QDataStream::Qt_4_9);
            s << object;
        }
        doc.insert(QStringLiteral("type"), QStringLiteral("variant"));
        doc.insert(QStringLiteral("data"), QString::fromLatin1(a.constData(), a.size()));
#else
        Q_ASSERT(!"QSettings: Cannot save custom types without QDataStream support");
#endif
        break;
    }
    }

    return doc;
}

QVariant jsonToVariant(QString json)
{
    QJsonDocument document = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject object = document.object();
    return jsonToVariant(object);
}


QVariant jsonToVariant(QJsonObject object)
{
    if (object.isEmpty()){
        return QVariant();
    }
    if (object.value(QStringLiteral("type")) == QStringLiteral("primitive"))
        return object.value(QStringLiteral("data")).toVariant();
    else if (object.value(QStringLiteral("type")) == QStringLiteral("uuid")){
        QString data = object.value(QStringLiteral("data")).toString();
        return QVariant(QUuid(data));
    }
    else if (object.value(QStringLiteral("type")) == QStringLiteral("point")){
        QJsonObject data = object.value(QStringLiteral("data")).toObject();
        return QVariant(QPoint(data.value(QStringLiteral("x")).toInt(), data.value(QStringLiteral("y")).toInt()));
    }
    else if (object.value(QStringLiteral("type")) == QStringLiteral("pointf")){
        QJsonObject data = object.value(QStringLiteral("data")).toObject();
        return QVariant(QPoint(data.value(QStringLiteral("x")).toDouble(), data.value(QStringLiteral("y")).toDouble()));
    }
    else if (object.value(QStringLiteral("type")) == QStringLiteral("rect")){
        QJsonObject data = object.value(QStringLiteral("data")).toObject();
        return QVariant(QRect(data.value(QStringLiteral("x")).toInt(),
                              data.value(QStringLiteral("y")).toInt(),
                              data.value(QStringLiteral("width")).toInt(),
                              data.value(QStringLiteral("height")).toInt()));
    }
    else if (object.value(QStringLiteral("type")) == QStringLiteral("rectf")){
        QJsonObject data = object.value(QStringLiteral("data")).toObject();
        return QVariant(QRectF(data.value(QStringLiteral("x")).toDouble(),
                               data.value(QStringLiteral("y")).toDouble(),
                               data.value(QStringLiteral("width")).toDouble(),
                               data.value(QStringLiteral("height")).toDouble()));
    }
    else if (object.value(QStringLiteral("type")) == QStringLiteral("size")){
        QJsonObject data = object.value(QStringLiteral("data")).toObject();
        return QVariant(QSize(data.value(QStringLiteral("width")).toInt(), data.value(QStringLiteral("height")).toInt()));
    }
    else if (object.value(QStringLiteral("type")) == QStringLiteral("sizef")){
        QJsonObject data = object.value(QStringLiteral("data")).toObject();
        return QVariant(QSizeF(data.value(QStringLiteral("width")).toDouble(), data.value(QStringLiteral("height")).toDouble()));
    }
    else if (object.value(QStringLiteral("type")) == QStringLiteral("bytearray")){
        QVariant data = object.value(QStringLiteral("data")).toVariant();
        return QVariant(QByteArray(data.value<QByteArray>()));
    }
    else if (object.value(QStringLiteral("type")) == QStringLiteral("keysequence")){
        return object.value(QStringLiteral("data")).toString();
    }
    else if (object.value(QStringLiteral("type")) == QStringLiteral("date")){
        QJsonObject data = object.value(QStringLiteral("data")).toObject();
        return QVariant(QDate(data.value(QStringLiteral("year")).toInt(),
                              data.value(QStringLiteral("month")).toInt(),
                              data.value(QStringLiteral("day")).toInt()));
    }
    else if (object.value(QStringLiteral("type")) == QStringLiteral("time")){
        QJsonObject data = object.value(QStringLiteral("data")).toObject();
        return QVariant(QTime(data.value(QStringLiteral("hour")).toInt(),
                              data.value(QStringLiteral("minute")).toInt(),
                              data.value(QStringLiteral("second")).toInt(),
                              data.value(QStringLiteral("msec")).toInt()));
    }
    else if (object.value(QStringLiteral("type")) == QStringLiteral("datetime")){
        QString data = object.value(QStringLiteral("data")).toString();
        return QVariant(QDateTime::fromString(data, QStringLiteral("yyyy-MM-ddTHH:mm:ss.zzzTZD")));
    }
    else if (object.value(QStringLiteral("type")) == QStringLiteral("list")){
        QJsonArray ary = object.value(QStringLiteral("data")).toArray();
        QVariantList list;
        for(auto obj : ary){
            list.append(jsonToVariant(obj.toObject()));
        }
        return list;
    }
    else if (object.value(QStringLiteral("type")) == QStringLiteral("object")){
        QJsonObject data = object.value(QStringLiteral("data")).toObject();
        QVariantMap map;
        for(auto key : data.keys()){
            map.insert(key, jsonToVariant(data[key].toObject()));
        }
        return map;
    }
    else if (object.value(QStringLiteral("type")) == QStringLiteral("variant")){
#ifndef QT_NO_DATASTREAM
        QString data = object.value(QStringLiteral("data")).toString();
        QByteArray a(data.toLatin1().mid(9));
        QDataStream stream(&a, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_9);
        QVariant result;
        stream >> result;
        return result;
#else
        Q_ASSERT(!"QSettings: Cannot load custom types without QDataStream support");
#endif
    }
    return QVariant();
}

QT_END_NAMESPACE

