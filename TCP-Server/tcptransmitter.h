#ifndef TCPTRANSMITTER_H
#define TCPTRANSMITTER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMetaType>

#include "listenermanager.h"

class TcpTransmitter : public QTcpServer
{
    Q_OBJECT

public:
    TcpTransmitter(ListenerManager* dataManager, const short port);
    ~TcpTransmitter();

signals:
    //void connectionError(QTcpSocket*);
    void disconnected(QStringList);
    void connectionDone(QTcpSocket*);

public slots:
    void connectTo(const QString &ip, const short &port);

private slots:
    void socketError(QAbstractSocket::SocketError);
    void socketCreated(QTcpSocket* socket);
    void socketDisconnected();

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    ListenerManager* listenerManager;
};

#endif // TCPTRANSMITTER_H
