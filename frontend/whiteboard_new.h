#ifndef WHITEBOARD_NEW_H
#define WHITEBOARD_NEW_H

#include <QMainWindow>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVector>
#include <QPoint>
#include <QToolBar>
#include <QAction>
#include <QPalette>
#include <QColor>
#include <QMenu>
#include <QFileDialog>
#include <QTextEdit>
#include <QTextBrowser>
#include <QDateTime>
#include <QPushButton>

#include "setpacket.h"
class whiteboard_new : public QMainWindow
{
    Q_OBJECT
public:
    explicit whiteboard_new(QWidget *parent, WhiteboardClient *_Client);
    ~whiteboard_new();

private:
    WhiteboardClient *Client;
    setPacket *setPacket;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void contextMenuEvent(QContextMenuEvent *);
    void keyPressEvent(QKeyEvent *e);
public:
private:
    int _lpress;//left mouse button press flag
    int _drag;//dragging flag
    int _drawType;//describe drawing type
    QMenu *_Rmenu;//right mouse button menu
    int _openflag;//open image

    QPixmap _pixmap;

    QRect  _imageRect; //position and size of image
    bool _isDragging; //image dragging flag
    QPoint _dragStartPos; //position of mouse when start dragging


    QTextEdit *_tEdit;//text input field
    QRect _drag_start_geometry;//strat dragging
    int _color;//color of drawing
    int _size;//size of drawing
    bool _selectMode;//select drawing mode
public:
    QVector<QVector<QPoint> > _eraser;//set of eraser
    QVector<QVector<QPoint> > _remote_eraser;//set of remote eraser
    QVector<QVector<QPoint> > _lines;//set of path
    QVector<QVector<QPoint> > _lines_remote;//set of remote path
    QVector<QRect> _rects;//set of rectangle
    QVector<QRect> _rects_remote;//set of remote rectangle
    QVector<QRect> _ellipse;//set of circle
    QVector<QRect> _ellipse_remote;//set of remote circle
    QVector<QRect>  _line;//set of line
    QVector<QRect>  _line_remote;//set of remote line
    QVector<QString>  _text;//set of text
    QVector<QString>  _text_remote;//set of remote text
    QVector<QPoint>  _tpoint;//position of text
    QVector<QPoint>  _tpoint_remote;//position of remote text
    QVector<QString>  _note;//set of text
    QVector<QString>  _note_remote;//set of remote text
    QVector<QPoint>  _npoint;//position of text
    QVector<QPoint>  _npoint_remote;//position of remote text
    QVector<int>  _shape;//set of shape
    QVector<int>  _shape_remote;//set of remote shape
    QPoint _begin;//mouse presses flag, used to undo
    QPoint _begin_remote;//remote mouse presses flag
    QVector<int> _shape_colors;//set of shape color
    QVector<int> _shape_colors_remote;//set of remote shape color
    QVector<int> _shape_size;//set of shape size
    QVector<int> _shape_size_remote;//set of remote shape size

    const QVector<QColor> _colors={QColor(0,0,0),QColor(255,0,0),QColor(0,255,0),QColor(0,0,255),\
    QColor(255,255,0),QColor(255,0,255),QColor(0,255,255),QColor(255,255,255)};//set of colors
    const QVector<int> _sizes={1,2,3,4,5,60};//set of sizes

signals:

public:
    void remotePathAppend(const QVector<QPoint> &path,bool isEraser);
    void remoteRectsAppend(const QRect &rect);
    void remoteEllipseAppend(const QRect &circle);
    void remoteLineAppend(const QRect &line);
    void remoteTextAppend(const QString &text, const QPoint &position);
    void remoteNoteAppend(const QString &note, const QPoint &position,float side);
    void remoteUndo();
    void remoteMove(int type, double dx, double dy);
    void remoteOpenpic(QString pic);
    QString pixmapTostring( const QPixmap& pixmap );
    QPixmap stringTopixmap( const QString& pic );


public slots:
    void Lines();
    void SavePic();
    void Rects();
    void Ellipses();
    void Line();
    void OpenPic();
    void Texts();
    void AddTexts();
    void Eraser();
    void StickyNote();


    void Color0();//black
    void Color1();//red
    void Color2();//green
    void Color3();//blue
    void Color4();//yellow
    void Color5();//pink
    void Color6();//cyan
    void Color7();//white

    void Size0();
    void Size1();
    void Size2();
    void Size3();
    void Size4();

};

#endif // WHITEBOARD_NEW_H
