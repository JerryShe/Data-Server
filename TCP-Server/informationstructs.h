#ifndef INFORMATIONSTRUCTS_H
#define INFORMATIONSTRUCTS_H


#include <QString>
#include <QVector>
#include <QTcpSocket>
#include <QMetaType>
#include <QJsonObject>

struct streamInfo
{
    QString streamName;
    QString streamDescription;
    QStringList streamVariables;
    QStringList streamVariablesDescription;

    QJsonObject toJson();
};


struct deviceInfo
{
    QString deviceName;
    QString deviceDescription;
    QVector <streamInfo> streams;

    QJsonObject toJson();
};


struct deviceConnectionInfo
{
    QString peerIP;
    QString peerPort;
    QString deviceName;
    QString deviceDescription;
    QTcpSocket* deviceSocket;
};


struct deviceList
{
    QVector <deviceInfo> devices;

    QByteArray toJson();
};


struct listenerRights
{
    listenerRights();
    listenerRights(QJsonObject json);

    QMap <QString, QVector<bool>> rightsMap;
    bool broadcastMessaging;

    QVector <bool> getByName(const QString name);
    void append(const QString &name, QVector <bool> &list);
};


struct listenerInfo
{
    listenerInfo();

    QString name;
    QString IP;
    QString port;
    bool listenerType;             //0 - listener, 1 - master
    listenerRights rights;
};


struct listenerList
{
    QVector <listenerInfo> listeners;

    QByteArray toJson();
};


struct packet
{
    packet();

    QString deviceName;
    unsigned char streamIndex;
    unsigned int time;
    QVector <double> data;

    void addVar(double var);

    QByteArray toJson();
};


struct message
{
    message();
    message(QJsonObject json, const QString &name);

    QString senderName;
    QString recipientName;      //empty if broadcast
    QString mess;

    QByteArray toJson();
};


Q_DECLARE_METATYPE(deviceConnectionInfo)
Q_DECLARE_METATYPE(deviceList)
Q_DECLARE_METATYPE(deviceInfo)
Q_DECLARE_METATYPE(streamInfo)
Q_DECLARE_METATYPE(packet)
Q_DECLARE_METATYPE(listenerInfo)
Q_DECLARE_METATYPE(listenerRights)
Q_DECLARE_METATYPE(message)

#endif // INFORMATIONSTRUCTS_H
