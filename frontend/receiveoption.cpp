#include "receiveoption.h"
#include "packet.hpp"
#include "element.hpp"
#include "element.pb.h"
#include "action.pb.h"
#include "packet.pb.h"
#include <queue>

ReceiveOption::ReceiveOption(QObject *parent, WhiteboardClient *_myclient, whiteboard_new *_myWhiteBoard) : QObject(parent)
{

    //initialization
    myclient = _myclient;
    myWhiteBoard = _myWhiteBoard;
    myTimer = new QTimer(this);
    interval = 100;
    //start timer
    myTimer->start(interval);

    connect(myTimer,&QTimer::timeout,this,&ReceiveOption::handlePacket);

}

//extract elements from packets
void ReceiveOption::handlePacket()
{
    myclient->handle_receive();
    std::queue<WhiteboardPacket> packets = myclient->received_queue;
    while(!packets.empty())
    {
        auto packet = packets.front();
        packets.pop();
        switch(packet.get_packet_type())
        {
        case(WhiteboardPacketType::broadcast):
            auto elements = myclient->handle_broadcast(packet.packet); //todo
            renderElement(elements);
            break;
        }
    }
}

//convert packets into QT format and render
void ReceiveOption::renderElement(vector<WhiteboardElements> elements){
    for(unsigned i=0;i<elements.size();i++)
    {
        //initialization
        std::vector<Point> path_points;
        QVector<QPoint> q_path_points;
        Point start;
        Point end;
        Point center;
        float radius;
        std::vector<Point> triangle_points;
        float square_side_length;
        Point square_topleft;
        Point text_center;
        std::string text_content;
        Point stickynote_center;
        float stickynote_side_length;
        std::string stickynote_content;
        QRect q_rect;

        switch(elements[i].get_type())
        {
        case(WhiteboardElementsType::Path):
            path_points = elements[i].get_path_points();
            for(const auto& point:path_points)
            {
                q_path_points.append(QPoint(static_cast<int>(point.x),static_cast<int>(point.y)));
            }
            myWhiteBoard->remotePathAppend(q_path_points,false);  //todo
            break;

        case(WhiteboardElementsType::Line):
            start = elements[i].get_line_start();
            end = elements[i].get_line_end();
            q_rect.setTopLeft(QPoint(static_cast<int>(start.x),static_cast<int>(start.y)));
            q_rect.setBottomRight(QPoint(static_cast<int>(end.x),static_cast<int>(end.y)));
            myWhiteBoard->remoteLineAppend(q_rect);
            break;

        case(WhiteboardElementsType::Circle):
            center = elements[i].get_circle_center();
            radius = elements[i].get_circle_radius();
            q_rect.setTopLeft(QPoint(static_cast<int>(center.x),static_cast<int>(center.y)));
            q_rect.setWidth(static_cast<int>(2*radius));
            q_rect.setHeight(static_cast<int>(2*radius));
            myWhiteBoard->remoteEllipseAppend(q_rect);
            break;

        case(WhiteboardElementsType::Triangle):
            triangle_points = elements[i].get_triangle_points();
            break;

        case(WhiteboardElementsType::Square):
            square_side_length = elements[i].get_square_side_length();
            square_topleft = elements[i].get_square_topleft();
            q_rect.setTopLeft(QPoint(static_cast<int>(square_topleft.x),static_cast<int>(square_topleft.y)));
            q_rect.setWidth(static_cast<int>(square_side_length));
            q_rect.setHeight(static_cast<int>(square_side_length));
            myWhiteBoard->remoteRectsAppend(q_rect);
            break;
        case(WhiteboardElementsType::Text):
            text_center = elements[i].get_text_center();
            text_content = elements[i].get_text_content();
            myWhiteBoard->remoteTextAppend(QString::fromStdString(text_content),QPoint(static_cast<int>(text_center.x),static_cast<int>(text_center.y)));
            break;
        case(WhiteboardElementsType::Picture):
            //todo
            break;
        case(WhiteboardElementsType::StickyNote):
            stickynote_center = elements[i].get_stickynote_center();
            stickynote_side_length = elements[i].get_stickynote_side_length();
            stickynote_content = elements[i].get_stickynote_content();
            myWhiteBoard->remoteNoteAppend(QString::fromStdString(stickynote_content),QPoint(static_cast<int>(stickynote_center.x),static_cast<int>(stickynote_center.y)),stickynote_side_length);
            break;
        case(WhiteboardElementsType::Null):
            qDebug()<<"Unrecoginzed WhiteboardElementsType";
            break;
        }
    }
}
