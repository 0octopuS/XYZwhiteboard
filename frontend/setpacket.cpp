#include "setpacket.h"
#include "whiteboard.h"

setPacket::setPacket(WhiteboardClient *_myClient)
{
    Client = _myClient;

}
std::vector<Point> convertVector(const QVector<QPoint>& vector){
    std::vector<Point> result;
    for(const QPoint& point : vector){
        Point p;
        p.x = static_cast<uint32_t>(point.x());
        p.y = static_cast<uint32_t>(point.y());
        result.push_back(p);
    }
    return result;

}



void setPacket::setPath(const QVector<QPoint> &line){
    WhiteboardElements _path;
    std::vector<Point> _line;
    for(const QPoint& point : line){
        Point p;
        p.x = static_cast<uint32_t>(point.x());
        p.y = static_cast<uint32_t>(point.y());
        _line.push_back(p);
    }
    _path.new_path(_line);
    Client->send_add_element_request(_path);
}

void setPacket::setEraser(const QVector<QPoint> &eraser){
    WhiteboardElements _eraser;
    std::vector<Point> _line;
    for(const QPoint& point : eraser){
        Point p;
        p.x = static_cast<uint32_t>(point.x());
        p.y = static_cast<uint32_t>(point.y());
        _line.push_back(p);
    }
    _eraser.new_path(_line);
    Client->send_add_element_request(_eraser);

}
void setPacket::setLine(const QRect &line){
    WhiteboardElements _line;
    Point start,end;
    start.x = static_cast<uint32_t>(line.topLeft().x());
    start.y = static_cast<uint32_t>(line.topLeft().y());
    end.x = static_cast<uint32_t>(line.bottomRight().x());
    end.y = static_cast<uint32_t>(line.bottomRight().y());

    _line.new_line(start,end);
    Client->send_add_element_request(_line);

}

void setPacket::setCircle(const QRect &ellipse) {
    // Construct and send a message containing circle data
    WhiteboardElements circle;
    Point start;
    start.x= static_cast<uint32_t>(ellipse.center().x());
    start.y= static_cast<uint32_t>(ellipse.center().y());
    int diameter = static_cast<uint32_t>(ellipse.width());//get radius
    circle.new_circle(start,diameter);
    Client->send_add_element_request(circle);
}

void setPacket::setSquare(const QRect &square){
    WhiteboardElements _square;
    Point start;
    start.x=static_cast<uint32_t>(square.topLeft().x());
    start.y=static_cast<uint32_t>(square.topLeft().y());
    int width = static_cast<uint32_t>(square.width());

    _square.new_square(start,width);
    Client->send_add_element_request(_square);

}

void setPacket::setText(const QString &text, QPoint &last){
    WhiteboardElements _text;
    Point start;
    start.x = static_cast<uint32_t>(last.x());
    start.y = static_cast<uint32_t>(last.y());
    _text.new_text(start,text.toStdString());
    Client->send_add_element_request(_text);
}

void setPacket::setPic(QString &pathname,QString &pic,QPoint &topLeft,QSize &size){
    WhiteboardElements _image;
    std::string name = pathname.toStdString();
    Point start;
    start.x = static_cast<uint32_t>(topLeft.x());
    start.y = static_cast<uint32_t>(topLeft.y());
    float top = size.width();
    _image.new_picture(start,pic.toStdString(),name);
    Client->send_add_element_request(_image);

}
void setPacket::setStickynote(const QString &text, QPoint &last, float side){
    WhiteboardElements _note;
    Point start;
    start.x = static_cast<uint32_t>(last.x());
    start.y = static_cast<uint32_t>(last.y());
    std::string content = text.toStdString();
    _note.new_stickynote(start,side,content);
    Client->send_add_element_request(_note);
}

