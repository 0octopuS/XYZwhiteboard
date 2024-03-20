#ifndef SETPACKET_H
#define SETPACKET_H
#include <QObject>
#include <QTimer>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QByteArray>
#include <QDataStream>
#include <QPoint>
#include "element.hpp"
#include "client.hpp"

class setPacket
{
    //Q_OBJECT
public:
    setPacket(WhiteboardClient *Client);

    void setPath(const QVector<QPoint>& line);
    void setEraser(const QVector<QPoint>& eraser);
    void setLine(const QRect& line);
    void setCircle(const QRect& ellipse);
    void setSquare(const QRect& square);
    void setText(const QString& text, QPoint& last);
    void setNote(const QString& text, QPoint& last);
    void setUndo();
    //void setPic()
    void setPic(QString& pathname,QString& pic,QPoint& topLeft,QSize& size);
    void setStickynote(const QString& text, QPoint& last, float side);
    std::vector<Point> convertVector(const QVector<QPoint>& vector);
private:
    WhiteboardClient *Client;
};

#endif // SETPACKET_H
