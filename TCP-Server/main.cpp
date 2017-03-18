#include <QCoreApplication>
#include <serverhandler.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ServerHandler server;

    return a.exec();
}
