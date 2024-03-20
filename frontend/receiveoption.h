#ifndef RECEIVEOPTION_H
#define RECEIVEOPTION_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QDebug>
#include <QTimer>
#include <QVector>
#include "whiteboard_new.h"
#include "client.hpp"
#include "packet.hpp"
#include "element.hpp"

class ReceiveOption : public QObject
{
    Q_OBJECT
public:
    explicit ReceiveOption(QObject *parent, WhiteboardClient *myclient, whiteboard_new *myWhiteBoard);

signals:

private:
    WhiteboardClient *myclient;
    whiteboard_new *myWhiteBoard;
//    qint64 totalBytes;
//    qint64 bytesReceived;
    QTimer *myTimer;
    quint16 interval;


    //json对象
    QJsonDocument myJsonDoc;
    QJsonObject myJsonObject;
    QByteArray receiveJsonByteArray;

private slots:
    void handlePacket();
    void renderElement(vector<WhiteboardElements> elements);
};

#endif // RECEIVEOPTION_H
