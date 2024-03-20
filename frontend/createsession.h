#ifndef CREATESESSION_H
#define CREATESESSION_H

#include <QWidget>

#include "whiteboard_new.h"
#include "receiveoption.h"
#include "client.hpp"


namespace Ui {
class CreateSession;
}

class CreateSession : public QWidget
{
    Q_OBJECT

public:
    CreateSession(QWidget *parent);
    ~CreateSession();

signals:
    void backSignal();

private:
    Ui::CreateSession *ui;
//    QTcpSocket *myTcpSocket;
    std::string serverIP;
    unsigned short serverPort;
    whiteboard_new *myWhiteBoard;
    ReceiveOption *myReceiveOption;
    uint32_t user_id = 0;
    WhiteboardClient *myclient;

private slots:
    void createButtonClicked();
//    void displayError(QAbstractSocket::SocketError socketError);

};

#endif // CREATESESSION_H
