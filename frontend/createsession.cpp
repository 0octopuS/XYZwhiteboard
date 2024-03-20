#include "createsession.h"
#include "ui_createsession.h"
#include "packet.pb.h"
#include <QDebug>
#include <QString>
#include <thread>


CreateSession::CreateSession(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CreateSession)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("waiting for creating!");

    //initialization
//    myTcpSocket = new QTcpSocket(0);
    myWhiteBoard = nullptr;
    myclient = nullptr;

    //connect signals & slots
    //back button connection
    connect(ui->backButton,&QPushButton::clicked,[=](){
       emit backSignal();
    });
    //create button connection
    connect(ui->createButton,&QPushButton::clicked,this,&CreateSession::createButtonClicked);
}

void CreateSession::createButtonClicked(){
    serverIP = ui->ipInput->text().toStdString();
    serverPort = ui->portInput->text().toUShort();
    qDebug()<<"create is clicked";
    qDebug()<<"serverIP:"<<QString::fromStdString(serverIP)<<"serverPort:"<<serverPort;

    myclient = new WhiteboardClient(serverIP,serverPort);
    myclient->send_create_whiteboard_request();
    myWhiteBoard = new whiteboard_new(0,myclient);
    myReceiveOption = new ReceiveOption(0,myclient,myWhiteBoard);
    myWhiteBoard->setWindowIcon(QIcon(":/png/images/1.png"));
    myWhiteBoard->show();
    this->hide();

}

//void CreateSession::displayError(QAbstractSocket::SocketError socketError)
//{
//    Q_UNUSED(socketError)
//    qDebug()<<myTcpSocket->errorString();
//    myTcpSocket->close();
//    myWhiteBoard->close();
//    this->close();
//}

CreateSession::~CreateSession()
{
    delete ui;
}
