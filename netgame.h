#ifndef NETGAME_H
#define NETGAME_H
#include "board.h"
#include <QTcpServer>
#include <QTcpSocket>

class NetGame : public Board
{
public:
    NetGame(bool bServer);


    QTcpServer* mServer;
    QTcpSocket* mSocket;

public slots:
    void slotNewConnection();


};

#endif // NETGAME_H
