#include "tcptransmitter.h"
#include "threadedtcpsocket.h"
#include "listenermanager.h"

#include <QDebug>
#include <QAbstractSocket>


TcpTransmitter::TcpTransmitter(ListenerManager *manager, const short port)
{
    qRegisterMetaType<QAbstractSocket::SocketError>();
    qRegisterMetaType<QVector<int>>();

    listenerManager = manager;
    listen(QHostAddress::Any, port);
}


TcpTransmitter::~TcpTransmitter()
{

}


void TcpTransmitter::socketError(QAbstractSocket::SocketError err)
{
    QObject * object = QObject::sender();
    if (!object)
        return;

    QTcpSocket * socket = static_cast<QTcpSocket *>(object);
    qDebug()<<socket->errorString();

    //emit connectionError(socket);
}


void TcpTransmitter::socketDisconnected()
{
    QObject * object = QObject::sender();
    if (!object)
        return;

    QTcpSocket * socket = static_cast<QTcpSocket *>(object);

    socket->thread()->quit();
    socket->thread()->wait();
    delete socket->thread();
}


void TcpTransmitter::connectTo(const QString &ip, const short &port)
{
    qDebug()<<"Connecting to " << ip << " : " << port << "...";

    ThreadedTcpSocket* socketThread = new ThreadedTcpSocket(ip, port, this);
    connect(socketThread, SIGNAL(socketCreated(QTcpSocket*)), this, SLOT(socketCreated(QTcpSocket*)));

    socketThread->start();
}


void TcpTransmitter::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"New incomming connection";

    ThreadedTcpSocket* socketThread = new ThreadedTcpSocket(socketDescriptor, this);
    connect(socketThread, SIGNAL(socketCreated(QTcpSocket*)), this, SLOT(socketCreated(QTcpSocket*)));

    socketThread->start();
}


void TcpTransmitter::socketCreated(QTcpSocket *socket)
{
    listenerManager->createListener(socket);

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    if (socket->bytesAvailable() > 0)
        emit socket->readyRead();

    qDebug()<<"Connection done";
}
