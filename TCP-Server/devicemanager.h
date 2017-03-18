#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QPair>

#include "deviceobj.h"
#include "streamobj.h"
#include "informationstructs.h"


class DeviceManager : public QObject
{
    Q_OBJECT

public:
    DeviceManager();
    ~DeviceManager();

    DeviceObj* getDeviceByName(const QString &name);
    StreamObj* getDeviceStream(const QString &name, const unsigned char index);

public slots:    
    void createDevice(QTcpSocket* socket);

    void sendMessage(message mess);

    void deleteDevice();
    void deleteDevice(QTcpSocket* socket);
    void deleteDevice(const QString &DeviceName);

    deviceList getDeviceList();

    void confirmConnection(QTcpSocket* socket);

signals:
    void deviceConnected(deviceConnectionInfo);
    void deviceDisconnected(deviceConnectionInfo);

    void newMessage(message);
    void deviceListChanged(deviceList);

private slots:
    void deviceWasConnected();

private:
    QMap <QTcpSocket*, DeviceObj*> deviceBySocket;
    QMap <QString, DeviceObj*> deviceByName;
};


#endif // DATAMANAGER_H
