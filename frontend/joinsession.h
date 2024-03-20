#ifndef JOINSESSION_H
#define JOINSESSION_H

#include <QWidget>

#include "whiteboard_new.h"
#include "receiveoption.h"
#include "client.hpp"

namespace Ui {
class JoinSession;
}

class JoinSession : public QWidget
{
    Q_OBJECT

public:
    explicit JoinSession(QWidget *parent);
    ~JoinSession();

signals:
    void backSignal();

private:
    Ui::JoinSession *ui;
//    QTcpSocket *myTcpSocket;
    std::string serverIP;
    unsigned short serverPort;
    string token;
    whiteboard_new *myWhiteBoard;
    ReceiveOption *myReceiveOption;
    WhiteboardClient *myclient;

private slots:
    void joinButtonClicked();
//    void displayError(QAbstractSocket::SocketError socketError);
};

#endif // JOINSESSION_H
