#ifndef THREADEDTCPSOCKET_H
#define THREADEDTCPSOCKET_H

#include <QThread>
#include <QTcpSocket>

class ThreadedTcpSocket : public QThread
{
    Q_OBJECT

public:
    ThreadedTcpSocket(qintptr ID, QObject *parent = 0);
    ThreadedTcpSocket(QString IP, short port, QObject* parent = 0);
    ~ThreadedTcpSocket();
    void run();

signals:
    void socketCreated(QTcpSocket*);

private:
    QTcpSocket *socket;

    qintptr socketDescriptor;
    QString socketIP;
    short socketPort;
};



#endif // THREADEDTCPSOCKET_H
