#include "threadedtcpsocket.h"

#include <QDebug>


ThreadedTcpSocket::ThreadedTcpSocket(qintptr ID, QObject *parent) : QThread(parent)
{
    socketDescriptor = ID;
}


ThreadedTcpSocket::ThreadedTcpSocket(QString IP, short port, QObject* parent) : QThread(parent)
{
    socketDescriptor = -1;
    socketIP = IP;
    socketPort = port;
}


ThreadedTcpSocket::~ThreadedTcpSocket()
{
    socket->abort();
    delete socket;
}


void ThreadedTcpSocket::run()
{
    socket = new QTcpSocket();


    if (socketDescriptor == -1)
    {
        socket->connectToHost(socketIP, socketPort);
        socket->waitForConnected();
    }
    else
        if(!socket->setSocketDescriptor(this->socketDescriptor))
            return;

    emit socketCreated(socket);
    exec();
}
