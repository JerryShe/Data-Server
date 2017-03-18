#include "tcpreceiver.h"
#include "threadedtcpsocket.h"

#include <QHostInfo>
#include <QDebug>
#include <QNetworkInterface>

#include <QAbstractSocket>



TcpReceiver::TcpReceiver(DeviceManager* manager, const short port)
{
    qRegisterMetaType<QAbstractSocket::SocketError>();
    qRegisterMetaType<QVector<int>>();

    deviceManager = manager;
    listen(QHostAddress::Any, port);
}


TcpReceiver::~TcpReceiver()
{}


void TcpReceiver::socketError(QAbstractSocket::SocketError err)
{
    QObject * object = QObject::sender();
    if (!object)
        return;

    QTcpSocket * socket = static_cast<QTcpSocket *>(object);
    qDebug()<<socket->errorString();

    emit connectionError(socket);
}


void TcpReceiver::connectTo(const QString &ip, const short &port)
{
    qDebug()<<"Connecting to " << ip << " : " << port << "...";

    ThreadedTcpSocket* socketThread = new ThreadedTcpSocket(ip, port, this);
    connect(socketThread, SIGNAL(socketCreated(QTcpSocket*)), this, SLOT(socketCreated(QTcpSocket*)));
    socketThread->start();
}


void TcpReceiver::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"New incomming connection";

    ThreadedTcpSocket* socketThread = new ThreadedTcpSocket(socketDescriptor, this);
    connect(socketThread, SIGNAL(socketCreated(QTcpSocket*)), this, SLOT(socketCreated(QTcpSocket*)));
    socketThread->start();
}


void TcpReceiver::socketCreated(QTcpSocket *socket)
{
    deviceManager->createDevice(socket);

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(disconnected()), deviceManager, SLOT(deleteDevice()));

    if (socket->bytesAvailable() > 0)
        emit socket->readyRead();

    qDebug()<<"Connection done";
}
