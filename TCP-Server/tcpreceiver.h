#ifndef TCPWORKER_H
#define TCPWORKER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMetaType>

#include "devicemanager.h"


class TcpReceiver : public QTcpServer
{
    Q_OBJECT

public:
    TcpReceiver(DeviceManager* manager, const short port);
    ~TcpReceiver();

signals:
    void connectionError(QTcpSocket*);
    void disconnected(QStringList);
    void connectionDone(QTcpSocket*);

public slots:
    void connectTo(const QString &ip, const short &port);

private slots:
    void socketError(QAbstractSocket::SocketError);
    void socketCreated(QTcpSocket* socket);

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    DeviceManager* deviceManager;
};


#endif // TCPWORKER_H
