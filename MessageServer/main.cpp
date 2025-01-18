#include "message_server.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MessageServer* serverObj = new MessageServer();

    return a.exec();
}
