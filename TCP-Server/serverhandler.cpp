#include "serverhandler.h"
#include <QCoreApplication>

#include <QTextStream>
#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>

ServerHandler::ServerHandler()
{
    QTextStream in(stdin);
    QTextStream out(stdout);

    if (qApp->arguments().size() == 4)
    {
        consoleMode = false;

        PortInput = qApp->arguments().at(1);
        PortOutput = qApp->arguments().at(2);
        masterKey = qApp->arguments().at(3);
    }
    else
    {
        consoleMode = true;

        out << "Choose reciver server port:" << endl;

        in >> PortInput;

        out << "Choose transmitter server port:" << endl;

        in >> PortOutput;

        out << "Enter master key:" <<endl;

        in >> masterKey;
    }

    deviceManager = new DeviceManager();
    connect(deviceManager, SIGNAL(deviceConnected(deviceConnectionInfo)), this, SLOT(confirmDeviceConnection(deviceConnectionInfo)));
    connect(deviceManager, SIGNAL(deviceDisconnected(deviceConnectionInfo)), this, SLOT(deviceDisconnected(deviceConnectionInfo)));

    listenerManager = new ListenerManager(deviceManager);
    connect(listenerManager, SIGNAL(listenerConnected(listenerInfo)), this, SLOT(confirmListenerConnection(listenerInfo)));
    connect(listenerManager, SIGNAL(listenerDisconnected(listenerInfo)), this, SLOT(listenerDisconnected(listenerInfo)));
    connect(listenerManager, SIGNAL(listenerRequestRights(listenerInfo,listenerRights)), this, SLOT(listenerRequestRights(listenerInfo,listenerRights)));
    connect(listenerManager, SIGNAL(listenerRequestControl(listenerInfo,QString)), this, SLOT(listenerRequestControl(listenerInfo,QString)));
    connect(listenerManager, SIGNAL(masterCommand(QString)), this, SLOT(masterCommand(QString)));

    if (consoleMode)
        out << "Receiver server starting at port: "<<PortInput << "..." << endl;

    receiver = new TcpReceiver(deviceManager, PortInput.toShort());

    if (consoleMode)
        out << "Transmitter server starting at port: "<<PortOutput << "..." << endl;

    transmitter = new TcpTransmitter(listenerManager, PortOutput.toShort());

    if (consoleMode)
        out << "Server started";
}


ServerHandler::~ServerHandler()
{
    delete transmitter;
    delete receiver;
    delete deviceManager;
    delete listenerManager;
}


void ServerHandler::confirmDeviceConnection(deviceConnectionInfo info)
{
    if (consoleMode && !masterType)
    {
        QTextStream out(stdout);
        QTextStream in(stdin);

        out << "New device wants to connect: " <<endl;
        out << "Device name: " << info.deviceName << endl;
        out << "Device IP" << info.peerIP << endl;

        while (true)
        {
            out << "Confirm connection?  Y/N" << endl;

            QString answ;
            in >> answ;

            if (answ == "Y" || answ == "y" || answ == "Yes" || answ == "yes")
            {
                deviceManager->confirmConnection(info.deviceSocket);
                out << endl;
                break;
            }
            else if (answ == "n" || answ == "N" || answ == "No" || answ == "no")
            {
                deviceManager->deleteDevice(info.deviceSocket);
                out << endl;
                break;
            }
        }
    }
    else if(consoleMode && masterType)
    {
        QTextStream out(stdout);
        QTextStream in(stdin);

        out << "device connection" << endl;
        out << info.deviceName << endl;
        out << info.deviceDescription << endl;
        out << info.peerIP << endl;
        out << info.peerPort << endl;

        QString answ;
        in >> answ;

        if (answ == "y")
            deviceManager->confirmConnection(info.deviceSocket);
        else
            deviceManager->deleteDevice(info.deviceSocket);
    }
    else if (!consoleMode)
    {
        ///
    }
}


void ServerHandler::confirmListenerConnection(listenerInfo info)
{
    if (consoleMode && !masterType)
    {
        QTextStream out(stdout);
        QTextStream in(stdin);

        out << "New listener wants to connect:" << endl;
        out << "Listener name: " << info.name <<endl;
        out << "Listener IP: " << info.IP;
        out << "Listener port: " << info.port;

        /// вывести список прав

        while (true)
        {
            out << "Confirm connection?  Y/N" << endl;

            QString answ;
            in >> answ;

            if (answ == "Y" || answ == "y" || answ == "Yes" || answ == "yes")
            {

                out << endl;
                break;
            }
            else if (answ == "n" || answ == "N" || answ == "No" || answ == "no")
            {

                out << endl;
                break;
            }
        }
    }
    else if(consoleMode && masterType)
    {
        QTextStream out(stdout);
        QTextStream in(stdin);
    }
    else if (!consoleMode)
    {

    }
}


void ServerHandler::deviceDisconnected(deviceConnectionInfo info)
{
    if (consoleMode && !masterType)
    {
        QTextStream out(stdout);
        QTextStream in(stdin);
    }
    else if(consoleMode && masterType)
    {

    }
    else if (!consoleMode)
    {

    }
}


void ServerHandler::listenerDisconnected(listenerInfo info)
{
    if (consoleMode && !masterType)
    {
        QTextStream out(stdout);
        QTextStream in(stdin);
    }
    else if(consoleMode && masterType)
    {

    }
    else if (!consoleMode)
    {

    }
}

void ServerHandler::listenerRequestRights(listenerInfo info, listenerRights rights)
{

}


void ServerHandler::listenerRequestControl(listenerInfo info ,QString key)
{

}


void ServerHandler::masterCommand(QString command)
{

}
