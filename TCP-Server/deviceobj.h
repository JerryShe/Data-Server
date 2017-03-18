#ifndef DEVICEOBJ_H
#define DEVICEOBJ_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QPair>
#include <QDataStream>


#include "streamobj.h"
#include "informationstructs.h"


class DeviceObj : public QObject
{
    Q_OBJECT

public:
    DeviceObj();
    DeviceObj(QTcpSocket* socket);
    ~DeviceObj();

    StreamObj* getStream(const unsigned char &index);

signals:
    void deviceWasConnected();
    void sendToDevice(QString);
    void sendToDevice(char*);

    void newData(packet pack);
    void newMessage(message);

public slots:
    void initialize();
    bool wasInitialized();
    void confirmInitialization();
    void confirmConnection();
    void requestNewName();


    void receiveNewData();

    QString getDeviceName();
    QString getDeviceDescription();

    QVector <QPair<bool, QString>> getDeviceMessages();
    void sendMessage(message mess);

    QTcpSocket* getDeviceSocket();

    deviceConnectionInfo getDeviceConnectionInfo();
    deviceInfo getDeviceInfo();

private slots:
    void getNewName();
    void abortInitialize();

    void writeToSocket(QString str);
    void writeToSocket(char* c);

    void newPacket(packet pack);
    void receiveMessage();

private:

    unsigned char initialized;                              //0 - не готово, 1 - готово, 2 - смена имени

    QString deviceName;
    QString deviceDescription;

    QTcpSocket* deviceSocket;
    QDataStream deviceDataStream;

    QMap <QString, StreamObj*> streamByName;
    QVector <StreamObj*> streamByIndex;

    QVector <QPair<bool, QString>> messages;       // 0 - полученное, 1 - отправленное
};


#endif // DEVICEOBJ_H
