#include "devicemanager.h"
#include "QThread"


DeviceManager::DeviceManager()
{
    qRegisterMetaType<deviceConnectionInfo>();
}


DeviceManager::~DeviceManager()
{
    foreach (DeviceObj* device, deviceByName) {
        QThread* deviceThread = device->thread();
        deviceThread->quit();
        delete device;
        deviceThread->deleteLater();
    }
}


void DeviceManager::createDevice(QTcpSocket *socket)
{
    DeviceObj* device = new DeviceObj(socket);

    connect(socket, SIGNAL(readyRead()), device, SLOT(receiveNewData()));
    connect(device, SIGNAL(deviceWasConnected()), this, SLOT(deviceWasConnected()));
    connect(device, SIGNAL(newMessage(message)), this, SIGNAL(newMessage(message)));

    deviceBySocket.insert(socket, device);
}


DeviceObj* DeviceManager::getDeviceByName(const QString &name)
{
    if (deviceByName.contains(name))
        return deviceByName[name];
    return 0;
}


StreamObj* DeviceManager::getDeviceStream(const QString &name, const unsigned char index)
{
    if (deviceByName.contains(name))
        return deviceByName[name]->getStream(index);
    return 0;
}


deviceList DeviceManager::getDeviceList()
{
    deviceList list;

    foreach (DeviceObj* device, deviceByName) {
        list.devices.append(device->getDeviceInfo());
    }

    return list;
}


void DeviceManager::sendMessage(message mess)
{
    if (mess.recipientName != "")
    {
        if (deviceByName.contains(mess.recipientName))
            deviceByName[mess.recipientName]->sendMessage(mess);
    }
    else
    {
        foreach (DeviceObj* device, deviceByName) {
            device->sendMessage(mess);
        }
    }
}


void DeviceManager::deleteDevice()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(QObject::sender());
    deleteDevice(socket);
}


void DeviceManager::deleteDevice(QTcpSocket* socket)
{
    qDebug()<< "delete device by socket";

    if (deviceBySocket.contains(socket) && socket != 0)
    {
        DeviceObj* device = deviceBySocket[socket];

        emit deviceDisconnected(device->getDeviceConnectionInfo());

        deviceByName.remove(device->getDeviceName());
        deviceBySocket.remove(socket);

        QThread* deviceThread = device->thread();
        deviceThread->quit();
        deviceThread->wait();
        delete device;
        delete deviceThread;

        emit deviceListChanged(this->getDeviceList());
    }
}


void DeviceManager::deleteDevice(const QString &DeviceName)
{
    qDebug()<< "delete device by name";
    if (deviceByName.contains(DeviceName))
    {
        DeviceObj* device = deviceByName[DeviceName];

        emit deviceDisconnected(device->getDeviceConnectionInfo());

        deviceByName.remove(DeviceName);
        deviceBySocket.remove(device->getDeviceSocket());

        QThread* deviceThread = device->thread();
        deviceThread->quit();
        deviceThread->wait();
        delete device;
        delete deviceThread;

        emit deviceListChanged(this->getDeviceList());
    }
}


void DeviceManager::confirmConnection(QTcpSocket *socket)
{
    if (deviceBySocket.contains(socket))
        deviceBySocket[socket]->confirmConnection();
}


void DeviceManager::deviceWasConnected()
{
    DeviceObj* device = static_cast<DeviceObj *>(QObject::sender());

    if (deviceByName.contains(device->getDeviceName()))
    {
        device->requestNewName();
        return;
    }
    else
        device->confirmInitialization();

    deviceByName.insert(device->getDeviceName(), device);

    qDebug()<<"Device '" << device->getDeviceName() << "' connected";
    emit deviceConnected(device->getDeviceConnectionInfo());
}
