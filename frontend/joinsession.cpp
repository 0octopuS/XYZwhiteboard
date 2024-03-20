#include "joinsession.h"
#include "ui_joinsession.h"
#include <QDebug>
#include <QString>

JoinSession::JoinSession(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JoinSession)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("waiting for joining!");

    //initialization
    myWhiteBoard = nullptr;
    myclient = nullptr;

    //信号与槽
    //back button connection
    connect(ui->backButton,&QPushButton::clicked,[=](){
       emit backSignal();
    });
    //join button connection
    connect(ui->joinButton,&QPushButton::clicked,this,&JoinSession::joinButtonClicked);
}

void JoinSession::joinButtonClicked(){
    serverIP = ui->ipInput->text().toStdString();
    serverPort = ui->portInput->text().toUShort();
    token = ui->roomInput->text().toStdString();
    qDebug()<<"join is clicked";
    qDebug()<<"serverIP:"<<QString::fromStdString(serverIP)<<"serverPort:"<<serverPort;

    myclient = new WhiteboardClient(serverIP,serverPort);
    myclient->send_join_session_request(token);
    myWhiteBoard = new whiteboard_new(0,myclient);
    myReceiveOption = new ReceiveOption(0,myclient,myWhiteBoard);
    myWhiteBoard->setWindowIcon(QIcon(":/png/images/1.png"));
    myWhiteBoard->show();
    this->hide();

}

//void JoinSession::displayError(QAbstractSocket::SocketError socketError)
//{
//    Q_UNUSED(socketError)
//    qDebug()<<myTcpSocket->errorString();
//    myTcpSocket->close();
//    myWhiteBoard->close();
//    this->close();
//}

JoinSession::~JoinSession()
{
    delete ui;
}
