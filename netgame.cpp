#include "netgame.h"
#include <QDebug>
#include <QTcpServer>
#include <QMessageBox>

NetGame::NetGame(bool bServer)
{
    mServer = NULL;
    mSocket = NULL;

    if (bServer)
    {
        mServer = new QTcpServer(this);
        mServer->listen(QHostAddress::Any, 9999);
        connect(mServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    }
    else
    {
        mSocket = new QTcpSocket(this);
        mSocket->connectToHost(QHostAddress("127.0.0.1"), 9999);
    }


}

void NetGame::slotNewConnection()
{
    QMessageBox::information(this, "hehe", "connect");
    if (mSocket)
        return;
    mSocket = mServer->nextPendingConnection();
    qDebug() << "connect";
    QMessageBox::information(this, "hehe", "connect");



}
