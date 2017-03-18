#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QObject>

#include <tcpreceiver.h>
#include <tcptransmitter.h>
#include <devicemanager.h>

#include "informationstructs.h"
#include "listenermanager.h"

class ServerHandler : public QObject
{
    Q_OBJECT

public:
    ServerHandler();
    ~ServerHandler();

private slots:
    void confirmDeviceConnection(deviceConnectionInfo deviceInfo);
    void confirmListenerConnection(listenerInfo deviceInfo);

    void deviceDisconnected(deviceConnectionInfo deviceInfo);
    void listenerDisconnected(listenerInfo deviceInfo);

    void listenerRequestRights(listenerInfo info, listenerRights rights);
    void listenerRequestControl(listenerInfo info ,QString key);
    void masterCommand(QString command);

private:
    DeviceManager* deviceManager;
    ListenerManager* listenerManager;

    TcpReceiver* receiver;
    TcpTransmitter* transmitter;

    QString PortInput, PortOutput;
    QString masterKey;

    bool consoleMode;       // 0 - tcp, 1 - console
    bool masterType;        // 0 - human, 1 - soft
};

#endif // SERVERHANDLER_H
