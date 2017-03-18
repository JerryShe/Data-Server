#include "informationstructs.h"
#include <QJsonDocument>
#include <QJsonArray>

packet::packet()
{

}


void packet::addVar(double var)
{
    data.append(var);
}


QByteArray packet::toJson()
{
    QJsonObject pack;
    pack["type"] = "data";
    pack["name"] = deviceName;
    pack["stream"] = streamIndex;
    pack["time"] = QString::number(time);

    QJsonArray vars;
    for (int i = 0; i < data.size(); i++)
        vars.append(data[i]);

    pack["vars"] = vars;

    QJsonDocument doc(pack);
    return doc.toJson();
}


QJsonObject streamInfo::toJson()
{
    QJsonObject streamObj;
    streamObj["name"] = streamName;
    streamObj["description"] = streamDescription;

    QJsonArray arr;
    for (int i = 0; i < streamVariables.size(); i++)
    {
        QJsonObject var;
        var["name"] = streamVariables[i];
        var["description"] = streamVariablesDescription[i];

        arr.append(var);
    }

    streamObj["variables"] = arr;

    return streamObj;
}


QJsonObject deviceInfo::toJson()
{
    QJsonObject deviceObj;
    deviceObj["name"] = deviceName;
    deviceObj["description"] = deviceDescription;

    QJsonArray arr;
    for (int i = 0; i < streams.size(); i++)
        arr.append(streams[i].toJson());

    deviceObj["streams"] = arr;

    return deviceObj;
}


QByteArray deviceList::toJson()
{
    QJsonObject list;
    list["type"] = "device list";

    QJsonArray arr;
    for (int i = 0; i < devices.size(); i++)
        arr.append(devices[i].toJson());

    list["devices"] = arr;

    QJsonDocument doc (list);
    return doc.toJson();
}


listenerRights::listenerRights()
{
    broadcastMessaging = false;
}


listenerRights::listenerRights(QJsonObject json)
{
    QJsonArray arr = json["list"].toArray();
    broadcastMessaging = json["broadcast"].toBool();

    foreach (auto item, arr) {
        QString name = item.toObject()["name"].toString();

        QVector <bool> accessList;
        QJsonArray access = item.toObject()["access"].toArray();

        foreach (auto t, access) {
            accessList.append(t.toBool());
        }
        if (accessList.size() > 0)
            append(name, accessList);
    }
}


void listenerRights::append(const QString &name, QVector<bool> &list)
{
    if (!rightsMap.contains(name))
        rightsMap.insert(name, list);
}


QVector<bool> listenerRights::getByName(const QString name)
{
    if (rightsMap.contains(name))
        return rightsMap[name];
    return QVector<bool>(0);
}


listenerInfo::listenerInfo()
{

}


QByteArray listenerList::toJson()
{

}


message::message()
{

}


message::message(QJsonObject json, const QString &name)
{
    senderName = name;
    recipientName = json["recipient"].toString();
    mess = json["text"].toString();
}


QByteArray message::toJson()
{
    QJsonObject messObj;
    messObj["type"] = "message";
    messObj["sender"] = senderName;
    messObj["recipient"] = recipientName;
    messObj["text"] = mess;

    QJsonDocument doc(messObj);
    return doc.toJson();
}
