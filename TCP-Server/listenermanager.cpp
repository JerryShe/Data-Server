#include "listenermanager.h"
#include <QThread>

ListenerManager::ListenerManager(DeviceManager* manager)
{
    deviceManager = manager;
    connect(deviceManager, SIGNAL(newMessage(message)), this, SLOT(sendDeviceMessage(message)));
    connect(this, SIGNAL(newMessage(message)), deviceManager, SLOT(sendMessage(message)));
}


ListenerManager::~ListenerManager()
{}


void ListenerManager::createListener(QTcpSocket* socket)
{
    ListenerObj* listener = new ListenerObj(socket);

    connect(socket, SIGNAL(readyRead()), listener, SLOT(receiveNewData()));
    connect(listener, SIGNAL(listenerWasConnected()), this, SLOT(listenerWasConnected()));

    connect(listener, SIGNAL(newMessage(message)), this, SLOT(sendListenerMessage(message)));
    connect(listener, SIGNAL(rightsRequest(listenerRights)), this, SLOT(requestRights(listenerRights)));
    connect(listener, SIGNAL(controlRequest(QString)), this, SLOT(requestControl(QString)));
    connect(listener, SIGNAL(commandRequest(QString)), this, SLOT(requestCommand(QString)));

    listenerBySocket.insert(socket, listener);
}


void ListenerManager::sendListenerMessage(message mess)
{
    ListenerObj* listener = static_cast<ListenerObj*>(QObject::sender());

    if (listener == 0 || !listenerPrivileges.contains(listener))
        return;

    if (mess.recipientName == "")
        if (listenerPrivileges[listener].broadcastMessaging == true)
            emit newMessage(mess);

    if (listenerPrivileges[listener].getByName(mess.recipientName).at(0) == true)
        emit newMessage(mess);
}


void ListenerManager::requestRights(listenerRights rights)
{
    ListenerObj* listener = static_cast<ListenerObj*>(QObject::sender());
    if (listener == 0)
        return;

    emit listenerRequestRights(listener->getListenerInfo(), rights);
}


void ListenerManager::requestControl(const QString key)
{
    ListenerObj* listener = static_cast<ListenerObj*>(QObject::sender());
    if (listener == 0)
        return;

    emit listenerRequestControl(listener->getListenerInfo(), key);
}


void ListenerManager::requestCommand(const QString command)
{
    ListenerObj* listener = static_cast<ListenerObj*>(QObject::sender());
    if (listener == 0 || listener != master)
        return;

    emit masterCommand(command);
}


void ListenerManager::requestDeviceList()
{
    ListenerObj* listener = static_cast<ListenerObj*>(QObject::sender());
    if (listener == 0 || listener != master)
        return;

    listener->sendJson(deviceManager->getDeviceList().toJson());
}


void ListenerManager::sendReplyTo(const QString &listenerName, const QString &answer, QString comment)
{
    if (listenerByName.contains(listenerName))
        listenerByName[listenerName]->sendReply(answer, comment);
}


void ListenerManager::sendDeviceMessage(message mess)
{
    if (!listenerByName.contains(mess.recipientName))
    {
        if (mess.recipientName == "")
        {
            foreach (ListenerObj* listener, listenerByName) {
                if (listenerPrivileges.contains(listener))
                {
                    if (listenerPrivileges[listener].getByName(mess.senderName).size() > 0)
                        if (listenerPrivileges[listener].getByName(mess.senderName)[0] == true)
                            listener->sendMessage(mess);
                }
            }
        }
        return;
    }

    ListenerObj* listener = listenerByName[mess.recipientName];
    if (listenerPrivileges.contains(listener))
        if (listenerPrivileges[listener].getByName(mess.senderName).size() > 0)
            if (listenerPrivileges[listener].getByName(mess.senderName)[0] == true)
                listener->sendMessage(mess);
}


void ListenerManager::confirmConnection(QTcpSocket* socket)
{
    if (listenerBySocket.contains(socket))
        listenerBySocket[socket]->confirmConnection();
}


void ListenerManager::confirmRights(const QString name, listenerRights rights)
{
    if (!listenerByName.contains(name))
        return;

    ListenerObj* listener = listenerByName[name];

    if (listenerPrivileges.contains(listener))
        listenerPrivileges[listener] = rights;
    else
        listenerPrivileges.insert(listener, rights);

    QList <QString> devices = rights.rightsMap.keys();
    for (int i = 0; i < devices.size(); i++)
    {
        DeviceObj* device = deviceManager->getDeviceByName(devices[i]);
        if (device != 0)
        {
            QVector <bool> arr = rights.rightsMap[devices[i]];
            for (int i = 1; i < arr.size(); i++)
                connect(device->getStream(i), SIGNAL(newData(packet)), listener, SLOT(sendData(packet)));
        }
    }
}


void ListenerManager::changeMaster(const QString &masterName)
{
    if (listenerByName.contains(masterName))
        master = listenerByName[masterName];
}


void ListenerManager::deleteListener()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(QObject::sender());
    deleteListener(socket);
}


void ListenerManager::deleteListener(QTcpSocket* socket)
{
    if (listenerBySocket.contains(socket) && socket != 0)
        deleteListener(listenerBySocket[socket]);
}


void ListenerManager::deleteListener(const QString &name)
{
    if (listenerByName.contains(name))
        deleteListener(listenerByName[name]);
}


void ListenerManager::deleteListener(ListenerObj* listener)
{
    emit listenerDisconnected(listener->getListenerInfo());

    listenerByName.remove(listener->getListenerName());
    listenerBySocket.remove(listener->getListenerSocket());

    QThread* listenerThread = listener->thread();
    listenerThread->quit();
    listenerThread->wait();
    delete listener;
    delete listenerThread;

    emit listenerListChanged(this->getListenerList());
}


void ListenerManager::listenerWasConnected()
{
    ListenerObj* listener = static_cast<ListenerObj*> (QObject::sender());

    if (listenerByName.contains(listener->getListenerName()))
    {
        listener->requestNewName();
        return;
    }
    else
        listener->confirmInitialization();

    listenerByName.insert(listener->getListenerName(), listener);

    qDebug()<<"Listener '" << listener->getListenerName() << "' connected";
    emit listenerConnected(listener->getListenerInfo());
}


listenerList ListenerManager::getListenerList()
{
    listenerList list;
    foreach (ListenerObj* listener, listenerByName) {
        listenerInfo info = listener->getListenerInfo();

        if (listenerPrivileges.contains(listener))
            info.rights = listenerPrivileges[listener];

        list.listeners.append(info);
    }

    return list;
}
