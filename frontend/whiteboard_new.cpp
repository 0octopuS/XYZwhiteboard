#include "whiteboard_new.h"

#include <QScreen>
#include <QGuiApplication>
#include <QDebug>

whiteboard_new::whiteboard_new(QWidget *parent, WhiteboardClient *_Client) :
    QMainWindow(parent)
{

    Client = _Client;

    _lpress = false;//mouse unpress
    _drawType = 0;//default drawing type
    _drag = 0;//default undrag
    _begin = pos();//drag start position
    _openflag = 0;//default not open picture
    _tEdit = new QTextEdit(this);//initializes the text box
    _tEdit->hide();//hide text box
    _color=0;//default color is black
    _size=1;//default size
    this->setFocusPolicy(Qt::StrongFocus);

    _selectMode = false;


    _isDragging = false;

    QScreen *screen=QGuiApplication::primaryScreen ();
    QRect deskRect=screen->availableGeometry() ;
    this->setGeometry(deskRect);
    this->setFixedSize(this->width(),this->height());
    setMouseTracking(true);//Enable real-time mouse tracking to monitor mouse movement events

    //set the background color
    this->setStyleSheet("background-color:white;");

    //create Toolbar
    QToolBar *tbar = addToolBar(tr("Toolbar"));
    tbar->setMovable(false);
    tbar->setIconSize(QSize(15, 15));//set the size of the action icons
    tbar->setStyleSheet("background-color:rgb(199,237,204)");//background color

    _Rmenu = new QMenu(this);//create menu
    _Rmenu->addAction(tr("Save  \tCtrl+S"), this, SLOT(SavePic()));//save action
    _Rmenu->addAction(tr("Edix  \tALT+F4"), this, SLOT(close()));//edit action
    _Rmenu->setStyleSheet("background-color:rgb(199,237,204)");//set background

    QAction *openAction = new QAction(tr("&Open"), this);//open action
    openAction->setIcon(QIcon(":/png/images/open.png"));//set icon
    openAction->setShortcut(QKeySequence(tr("Ctrl+O")));//set shortcut
    tbar->addAction(openAction);

    QAction *saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":/png/images/save.png"));
    saveAction->setShortcut(QKeySequence(tr("Ctrl+S")));
    tbar->addAction(saveAction);

    QAction *saveasAction = new QAction(tr("&Save as"), this);
    saveasAction->setIcon(QIcon(":/png/images/saveas.png"));
    saveasAction->setShortcut(QKeySequence(tr("Ctrl+ALT+S")));
    tbar->addAction(saveasAction);

    QAction *lineAction = new QAction(tr("&Line"), this);
    lineAction->setIcon(QIcon(":/png/images/line.png"));
    lineAction->setShortcut(QKeySequence(tr("Ctrl+L")));
    tbar->addAction(lineAction);

    QAction *linesAction = new QAction(tr("&Pencil"), this);
    linesAction->setIcon(QIcon(":/png/images/lines.png"));
    linesAction->setShortcut(QKeySequence(tr("Ctrl+P")));
    tbar->addAction(linesAction);

    QAction *rectAction = new QAction(tr("&Square"), this);
    rectAction->setIcon(QIcon(":/png/images/rect.png"));
    rectAction->setShortcut(QKeySequence(tr("Ctrl+R")));
    tbar->addAction(rectAction);

    QAction *ellipseAction = new QAction(tr("&Circle"), this);
    ellipseAction->setIcon(QIcon(":/png/images/ellipse.png"));
    ellipseAction->setShortcut(QKeySequence(tr("Ctrl+E")));
    tbar->addAction(ellipseAction);

    QAction *textAction = new QAction(tr("&Text"), this);
    textAction->setIcon(QIcon(":/png/images/texts.png"));
    textAction->setShortcut(QKeySequence(tr("Ctrl+T")));
    tbar->addAction(textAction);

    QAction *noteAction = new QAction(tr("&stickyNote"), this);
    noteAction->setIcon(QIcon(":/png/images/texts.png"));
    noteAction->setShortcut(QKeySequence(tr("Ctrl+N")));
    tbar->addAction(noteAction);

    QAction *eraseAction = new QAction(tr("&Eraser"), this);
    eraseAction->setIcon(QIcon(":/png/images/eraser.png"));
    eraseAction->setShortcut(QKeySequence(tr("Ctrl+Y")));
    tbar->addAction(eraseAction);


    QToolBar *tbar_color = addToolBar(tr("Color Bar"));
    tbar_color->setMovable(false);
    tbar_color->setIconSize(QSize(15, 15));
    tbar_color->setStyleSheet("background-color:rgb(199,237,204)");

    QAction *color0Action = new QAction(tr("&Black"), this);//Black color
    color0Action->setIcon(QIcon(":/png/images/color0.png"));
    tbar_color->addAction(color0Action);

    QAction *color1Action = new QAction(tr("&Red"), this);
    color1Action->setIcon(QIcon(":/png/images/color1.png"));
    tbar_color->addAction(color1Action);

    QAction *color2Action = new QAction(tr("&Green"), this);
    color2Action->setIcon(QIcon(":/png/images/color2.png"));
    tbar_color->addAction(color2Action);

    QAction *color3Action = new QAction(tr("&Blue"), this);
    color3Action->setIcon(QIcon(":/png/images/color3.png"));
    tbar_color->addAction(color3Action);

    QAction *color4Action = new QAction(tr("&Yellow"), this);
    color4Action->setIcon(QIcon(":/png/images/color4.png"));
    tbar_color->addAction(color4Action);

    QAction *color5Action = new QAction(tr("&Pink"), this);
    color5Action->setIcon(QIcon(":/png/images/color5.png"));
    tbar_color->addAction(color5Action);

    QAction *color6Action = new QAction(tr("&Cyan"), this);
    color6Action->setIcon(QIcon(":/png/images/color6.png"));
    tbar_color->addAction(color6Action);

    QAction *color7Action = new QAction(tr("&White"), this);
    color7Action->setIcon(QIcon(":/png/images/color7.png"));
    tbar_color->addAction(color7Action);


    QToolBar *tbar_size = addToolBar(tr("line size ToolBar"));
    tbar_size->setMovable(false);
    tbar_size->setIconSize(QSize(15, 15));
    tbar_size->setStyleSheet("background-color:rgb(199,237,204)");

    QAction *size0Action = new QAction(tr("&Size0"), this);//Size0
    size0Action->setIcon(QIcon(":/png/images/size0.png"));//set icon
    tbar_size->addAction(size0Action);

    QAction *size1Action = new QAction(tr("&Size1"), this);//Size1
    size1Action->setIcon(QIcon(":/png/images/size1.png"));//set icon
    tbar_size->addAction(size1Action);

    QAction *size2Action = new QAction(tr("&Size2"), this);//Size2
    size2Action->setIcon(QIcon(":/png/images/size2.png"));//set icon
    tbar_size->addAction(size2Action);

    QAction *size3Action = new QAction(tr("&Size3"), this);//Size3
    size3Action->setIcon(QIcon(":/png/images/size3.png"));//set icon
    tbar_size->addAction(size3Action);

    QAction *size4Action = new QAction(tr("&Size4"), this);//Size4
    size4Action->setIcon(QIcon(":/png/images/size4.png"));//set icon
    tbar_size->addAction(size4Action);

    //connect signal and slot
    connect(linesAction, SIGNAL(triggered()), this, SLOT(Lines()));
    connect(rectAction, SIGNAL(triggered()), this, SLOT(Rects()));
    connect(ellipseAction, SIGNAL(triggered()), this, SLOT(Ellipses()));
    connect(lineAction, SIGNAL(triggered()), this, SLOT(Line()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(SavePic()));
    connect(saveasAction, SIGNAL(triggered()), this, SLOT(SavePic()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(OpenPic()));
    connect(textAction, SIGNAL(triggered()), this, SLOT(Texts()));
    connect(_tEdit, SIGNAL(textChanged()), this, SLOT(AddTexts()));
    connect(eraseAction, SIGNAL(triggered()), this, SLOT(Eraser()));
    connect(noteAction, SIGNAL(triggered()), this, SLOT(StickyNote()));


    connect(color0Action, SIGNAL(triggered()), this, SLOT(Color0()));
    connect(color1Action, SIGNAL(triggered()), this, SLOT(Color1()));
    connect(color2Action, SIGNAL(triggered()), this, SLOT(Color2()));
    connect(color3Action, SIGNAL(triggered()), this, SLOT(Color3()));
    connect(color4Action, SIGNAL(triggered()), this, SLOT(Color4()));
    connect(color5Action, SIGNAL(triggered()), this, SLOT(Color5()));
    connect(color6Action, SIGNAL(triggered()), this, SLOT(Color6()));
    connect(color7Action, SIGNAL(triggered()), this, SLOT(Color7()));

    connect(size0Action, SIGNAL(triggered()), this, SLOT(Size0()));
    connect(size1Action, SIGNAL(triggered()), this, SLOT(Size1()));
    connect(size2Action, SIGNAL(triggered()), this, SLOT(Size2()));
    connect(size3Action, SIGNAL(triggered()), this, SLOT(Size3()));
    connect(size4Action, SIGNAL(triggered()), this, SLOT(Size4()));

}





void whiteboard_new::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(rect(),Qt::white);
    unsigned int i1=0,i2=0,i3=0,i4=0,i5=0,i6=0,i7=0;//index of various graphics
    for(int c = 0;c<_shape.size();++c)//controls the total number of graphics currently drawn by the user
    {
        if(_shape.at(c) == 6){
            p.setPen(QPen(Qt::white,_sizes.at(_shape_size.at(c))));
        }
        else p.setPen(QPen(_colors.at(_shape_colors.at(c)),_sizes.at(_shape_size.at(c))));
        if(_shape.at(c) == 1 || _shape.at(c) == 6 )//line or eraser
        {
              const QVector<QPoint>& line = _lines.at(i1++);//take out a line
              for(int j=0; j<line.size()-1; ++j)
              {
                  p.drawLine(line.at(j), line.at(j+1));
              }
        }
        else if(_shape.at(c) == 2)//square
        {
             p.drawRect(_rects.at(i2++));
        }
        else if(_shape.at(c) == 3)//circle
        {
            p.drawEllipse(_ellipse.at(i3++));
        }
        else if(_shape.at(c) == 4)//line
        {
            p.drawLine(_line.at(i4).topLeft(),_line.at(i4).bottomRight());
            i4++;
        }
        else if(_shape.at(c) == 5)//text
        {
            p.drawText(_tpoint.at(i5),_text.at(i5));
            i5++;
        }
        else if(_shape.at(c) == 7){//stickynote
            p.setBrush(QColor(231,205,226));
            QRect noteRect(_npoint.at(i5), QFontMetrics(p.font()).boundingRect(_text.at(i5)).size());
            p.drawRect(noteRect);
            p.drawText(noteRect, Qt::AlignCenter,_note.at(i7));
            i7++;

        }

    }
    unsigned int i1_rt=0,i2_rt=0,i3_rt=0,i4_rt=0,i5_rt=0;//Index of various remote graphics
    for(int c_rt = 0;c_rt<_shape_remote.size();++c_rt)
    {
        if(_shape_remote.at(c_rt) == 6){
            p.setPen(QPen(Qt::white,_sizes.at(_shape_size_remote.at(c_rt))));
        }
        else p.setPen(QPen(_colors.at(_shape_colors_remote.at(c_rt)),_sizes.at(_shape_size_remote.at(c_rt))));
        if(_shape_remote.at(c_rt) == 1 || _shape_remote.at(c_rt) == 6)//line or path
        {
              const QVector<QPoint>& line = _lines_remote.at(i1_rt++);
              for(int j=0; j<line.size()-1; ++j)//draw all the segments of the line
              {
                  p.drawLine(line.at(j), line.at(j+1));
              }
        }
        else if(_shape_remote.at(c_rt) == 2)//square
        {
             p.drawRect(_rects_remote.at(i2_rt++));
        }
        else if(_shape_remote.at(c_rt) == 3)//circle
        {
            p.drawEllipse(_ellipse_remote.at(i3_rt++));
        }
        else if(_shape_remote.at(c_rt) == 4)//line
        {
            p.drawLine(_line_remote.at(i4_rt).topLeft(),_line_remote.at(i4_rt).bottomRight());
            i4_rt++;
        }
        else if(_shape_remote.at(c_rt) == 5)//text
        {
            p.drawText(_tpoint_remote.at(i5_rt),_text_remote.at(i5_rt));
            i5_rt++;
        }
        else if(_shape_remote.at(c_rt) == 7){
            p.setBrush(QColor(231,205,226));
            float sideLength =  QFontMetrics(p.font()).boundingRect(_note.at(i7)).width();
            QRect noteRect(_npoint.at(i7), QSize(sideLength,sideLength)); //calculate rectangle size based on text size
            p.drawRect(noteRect);
            p.drawText(noteRect, Qt::AlignCenter,_note_remote.at(i7));
            i7++;

        }
    }
    //_drawType = 0;
    p.end();
    p.begin(this);//use the current form as a canvas
    p.drawPixmap(_imageRect, _pixmap);//draw pixmap to form
}

void whiteboard_new::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)//when left mouse button is pressed
    {
        if(_imageRect.contains(e->pos())){
            qDebug()<<"Inside the picture";
            _isDragging = true;
            _begin = e->pos();

    }
        if(_drawType == 1 || _drawType == 6)//path
        {
            _lpress = true;
            QVector<QPoint> line;//press the mouse and the new line begins
            _lines.append(line);//add a new line to the line collection
            QVector<QPoint>& lastLine = _lines.last();//get new lines
            lastLine.append(e->pos());//Record the mouse coordinates (start coordinates of the new line)
            if(_drawType ==1){
                _shape.append(1);
                _shape_size.append(_size);
            }
            else{
                _shape.append(6);
                _shape_size.append(5);
            }
               //_shape_colors.append(_color);

        }
        else if(_drawType == 2)//square
        {
            _lpress = true;//left button press flag
            if(!_drag)//not dragging
            {
                QRect rect;//create rectangle
                _rects.append(rect);//Add a new rectangle to the rectangle collection
                QRect& lastRect = _rects.last();
                //lastRect.topLeft()
                lastRect.setTopLeft(e->pos());//record the topleft position
                //setPacket ->setSquare(lastRect);
                _shape.append(2);
                _shape_colors.append(_color);
                _shape_size.append(_size);
            }
            else if(_rects.last().contains(e->pos()))//drag mode, if press inside the rectangle
            {
                qDebug()<<"Inside the rectangle";
                _begin = e->pos();//Record the position of the start of the drag

            }
        }
        else if(_drawType == 3)//circle
        {
            _lpress = true;//left button press flag
            if(!_drag)//not dragging
            {
                QRect rect;
                _ellipse.append(rect);//add a new circle to the circle collection
                QRect& lastEllipse = _ellipse.last();//get the new circle
                lastEllipse.setTopLeft(e->pos());//record the position of the mouse
                //setPacket->setCircle(lastEllipse);
                _shape.append(3);
                _shape_colors.append(_color);
                _shape_size.append(_size);
                //_drawType =0;
            }
            else if(_ellipse.last().contains(e->pos()))//
            {
                _begin = e->pos();//Record the position of the start of the drag

            }
        }
        else if(_drawType == 4)//line
        {
            _lpress = true;//left button press flag
            QRect rect;
            _line.append(rect);//add a new line to the line collection
            QRect& lastLine = _line.last();//get the new line
            lastLine.setTopLeft(e->pos());//record the position of the mouse
            setPacket ->setLine(lastLine);
            _shape.append(4);
            _shape_colors.append(_color);
            _shape_size.append(_size);
        }
        else if(_drawType == 5)//Text
        {
            if(!_drag){
            update();//update windows drawing
            QPoint p;
            _tpoint.append(p);//add text coordinates to the text position collection
            QPoint& lastp = _tpoint.last();//get new text
            lastp = e->pos();//Record the coordinates of the mouse
            _tEdit->setGeometry(lastp.x()-6,lastp.y()-17,90,90);//Set the position and size of the text box
            _tEdit->show();//show text box
            _text.append("");//add text to a text collection
            _tEdit->clear();//clear text box
            _shape.append(5);
            _shape_colors.append(_color);
            _shape_size.append(_size);
            }
            else if(_tEdit->geometry().contains(e->pos())){
                _begin = e->pos();
                _drag_start_geometry = _tEdit->geometry();
            }
        }
        else if(_drawType == 7){
            if(!_drag){
            update();//update the window
            QPoint p;
            _npoint.append(p);//add sticky note coordinates to the note position collection
            QPoint& lastp = _npoint.last();//get new sticky note
            lastp = e->pos();//record the coordinates of the mouse
            _tEdit->setGeometry(lastp.x()-6,lastp.y()-17,90,90);//Set the position and size of the text box
            _tEdit->show();//show text box
            _note.append("");//add note to a note collection
            _tEdit->clear();//clear text box
            _shape.append(7);
            _shape_colors.append(_color);
            _shape_size.append(_size);
            }
            else if(_tEdit->geometry().contains(e->pos())){
                _begin = e->pos();
                _drag_start_geometry = _tEdit->geometry();
            }

        }
        else if(_drawType == 6)
        {
            _lpress = true;
            QVector<QPoint> erasers;
            _eraser.append(erasers);//add a new eraser to the eraser collection
            QVector<QPoint>& lastErasers = _eraser.last();//get the new line
            lastErasers.append(e->pos());//record the coordinates of the mouse
            _shape.append(6);
            _shape_size.append(5);

        }

    }
}

void whiteboard_new::AddTexts()//Called when the input box text changes
{
    QString& last = _text.last();//get the last text
    last = _tEdit->toPlainText();
    QPoint lastPos = _tEdit->pos();
    int side = _tEdit->width();
    if(_drawType == 5){
        setPacket ->setText(last,lastPos);
    }
    else{
        setPacket ->setStickynote(last,lastPos,side);
    }
}

void whiteboard_new::mouseMoveEvent(QMouseEvent *e)
{
    if(_drag && ((_drawType == 2 && _rects.last().contains(e->pos()))
            || (_drawType == 3 && _ellipse.last().contains(e->pos()) )))
    {
        setCursor(Qt::SizeAllCursor);//Drag mode, and in the drag graphic, change the cursor shape to a cross
    }
    else
    {
        setCursor(Qt::ArrowCursor);//restore original cursor shape
        _drag = 0;
    }
    if(_isDragging){
        setCursor(Qt::SizeAllCursor);
        int dx = e->pos().x()-_begin.x();//Distance of horizontal movement x
        int dy = e->pos().y()-_begin.y();//Distance of vertical movement y
        _imageRect.translate(dx,dy);
        _begin = e->pos();
        update();
        QPoint topLeft = _imageRect.topLeft();
        QSize size = _imageRect.size();
        QString picData = pixmapTostring(_pixmap);
        QString name = "name";
        setPacket ->setPic(name,picData,topLeft,size);
        //setCursor(Qt::ArrowCursor);
    }

    if(_lpress)
    {
        if(_drawType == 1)
        {
            if(_lines.size()<=0) return;
            QVector<QPoint>& lastLine = _lines.last();//The last line added is the latest drawing
            lastLine.append(e->pos());//Record the coordinates of the mouse
            update();//update windows
        }
        else if(_drawType == 2)
        {
            if(_drag == 0)//not dragging
            {
                QRect& lastRect = _rects.last();//get the last square
                lastRect.setBottomRight(e->pos());//Update the bottom right corner coordinates of the square
            }
            else//is dragging
            {
                QRect& lastRect = _rects.last();//get the last square
                if(lastRect.contains(e->pos()))//inside the square
                {
                    int dx = e->pos().x()-_begin.x();//Distance of horizontal movement x
                    int dy = e->pos().y()-_begin.y();//Distance of vertical movement y
                    lastRect = lastRect.adjusted(dx,dy,dx,dy);//update the position of square
                    setPacket ->setSquare(lastRect);
                    _begin = e->pos();//Refresh the starting coordinates of drag point
                }

            }
            update();//update the windows

        }
        else if(_drawType == 3)
        {
            if(_drag == 0)//not dragging
            {
                QRect& lastEllipse = _ellipse.last();//get the new circle
                lastEllipse.setBottomRight(e->pos());//Update the bottom right corner coordinates of the circle

            }
            else//if dragging
            {
                QRect& lastEllipse = _ellipse.last();//get the last circle
                if(lastEllipse.contains(e->pos()))//inside the circle
                {
                    int dx = e->pos().x()-_begin.x();//Distance of horizontal movement x
                    int dy = e->pos().y()-_begin.y();//Distance of vertical movement y
                    lastEllipse = lastEllipse.adjusted(dx,dy,dx,dy);
                    setPacket ->setCircle(lastEllipse);
                    _begin = e->pos();//Refresh the starting coordinates of drag point
                }

            }
            update();//update windows
        }
        else if(_drawType == 4)
        {
            QRect& lastLine = _line.last();//get the new line
            lastLine.setBottomRight(e->pos());
            update();//update windows
        }
        else if(_drawType == 5)
        {
            int dx = e->pos().x()-_begin.x();//Distance of horizontal movement x
            int dy = e->pos().y()-_begin.y();//Distance of vertical movement y
            _tEdit->setGeometry(_drag_start_geometry.adjusted(dx, dy, dx, dy));
            QPoint lastPos = _tEdit->pos();
            setPacket->setText(_text.last(),lastPos);
            _begin = e->pos();
            update();
        }
        else if(_drawType == 6){
            if(_eraser.size()<=0) return;
            QVector<QPoint>& lastErasers = _eraser.last();//add last eraser
            lastErasers.append(e->pos());//Record the coordinates of the mouse
            setPacket ->setEraser(lastErasers);
            update();//update window
        }
        else if(_drawType ==7){
            int dx = e->pos().x()-_begin.x();//Distance of horizontal movement x
            int dy = e->pos().y()-_begin.y();//Distance of vertical movement y
            _tEdit->setGeometry(_drag_start_geometry.adjusted(dx, dy, dx, dy));
            QPoint lastPos = _tEdit->pos();
            int side = _tEdit->width();
            setPacket -> setStickynote(_text.last(),lastPos,side);
            //setPacket->setText(_text.last(),lastPos);
            _begin = e->pos();
            update();
        }
    }


}

void whiteboard_new::mouseReleaseEvent(QMouseEvent *e)
{
    if(_isDragging){
        _isDragging = false;
    }
    if(_lpress)
    {
        if(_drawType == 1)
        {
            QVector<QPoint>& lastLine = _lines.last();//Last added line
            lastLine.append(e->pos());//Record the end coordinates of the line
            setPacket ->setPath(lastLine);
            _lpress = false;
        }
        else if(_drawType == 2)
        {
            QRect& lastRect = _rects.last();//get new square
            if(!_drag)
            {
                lastRect.setBottomRight(e->pos());//Update the bottom right corner coordinates of the rectangle when not dragging
                setPacket ->setSquare(lastRect);
                //After drawing the rectangle, set the cursor to the center of the new rectangle and enter drag mode
                this->cursor().setPos(this->cursor().pos().x()-lastRect.width()/2,this->cursor().pos().y()-lastRect.height()/2);
                _drag = 1;

            }
            _lpress = false;
            //_drawType = 0;

        }
        else if(_drawType == 3)
        {
            QRect& lastEllipse = _ellipse.last();//get new circle
            if(!_drag)
            {
                lastEllipse.setBottomRight(e->pos());//Update the bottom right corner coordinates of the circle when not dragging
                setPacket ->setCircle(lastEllipse);
                //After drawing the circle, set the cursor to the center of the new ellipse and enter drag mode
                this->cursor().setPos(this->cursor().pos().x()-lastEllipse.width()/2,this->cursor().pos().y()-lastEllipse.height()/2);
                _drag = 1;

            }
            _lpress = false;
            //_drawType = 0;
        }
        else if(_drawType == 4)
        {
            QRect& lastLine = _line.last();//get the new line
            lastLine.setBottomRight(e->pos());//Update the bottom right corner coordinates of the line
            setPacket ->setLine(lastLine);
            _lpress = false;

        }
        else if(_drawType ==5){
            QString text = _tEdit->toPlainText();
            QPoint pos = _tEdit->pos();
            setPacket->setText(text,pos);

        }
        else if(_drawType == 6){
            QVector<QPoint>& lastErasers = _eraser.last();
            lastErasers.append(e->pos());//Record the end coordinates of the line
            setPacket ->setEraser(lastErasers);
            _lpress = false;//Mark left button release
        }
        else if(_drawType == 7){
            QString note = _tEdit->toPlainText();
            QPoint pos = _tEdit->pos();
            int side = _tEdit->width();
            setPacket->setStickynote(note,pos,side);
        }
    }

}

void whiteboard_new::Lines()
{
    _drawType = 1;
    _tEdit->hide();
}

void whiteboard_new::Line()
{
    _drawType = 4;
    _tEdit->hide();
}

void whiteboard_new::Rects()
{
    _drawType = 2;
    _tEdit->hide();

}

void whiteboard_new::Ellipses()
{
    _drawType = 3;
    _tEdit->hide();
}

void whiteboard_new::Texts()
{
    _drawType = 5;
}

void whiteboard_new::Eraser()
{
    _drawType = 6;
    _tEdit->hide();
}

void whiteboard_new::StickyNote(){
    _drawType = 7;
}
//color setting
void whiteboard_new::Color0()
{
    _color=0;//black
}

void whiteboard_new::Color1()
{
    _color=1;//red
}

void whiteboard_new::Color2()
{
    _color=2;//green
}

void whiteboard_new::Color3()
{
    _color=3;//blue
}

void whiteboard_new::Color4()
{
    _color=4;//yellow
}

void whiteboard_new::Color5()
{
    _color=5;//pink
}

void whiteboard_new::Color6()
{
    _color=6;//cyan
}

void whiteboard_new::Color7()
{
    _color=7;//white
}

//line size setting
void whiteboard_new::Size0()
{
    _size=0;
}

void whiteboard_new::Size1()
{
    _size=1;
}

void whiteboard_new::Size2()
{
    _size=2;
}

void whiteboard_new::Size3()
{
    _size=3;
}

void whiteboard_new::Size4()
{
    _size=4;
}

void whiteboard_new::SavePic()
{
    //Pop-up file save dialog
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save"), "new.jpg", "Image (*.jpg *.png)");

    if (fileName.length() > 0)
    {
        _tEdit->hide();
        QPixmap pixmap(size());//Create a new pixmap of the form size
        QPainter painter(&pixmap);//Use pixmap as a canvas
        painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);//Set the painting area and canvas color
        this->render(&painter);//Render the form to the painter, then draw the painter to the canvas
        pixmap.copy(QRect(0,30,size().width(),size().height()-30)).save(fileName);
    }
}

void whiteboard_new::OpenPic()
{
    //popup file open dialog
    QString picPath = QFileDialog::getOpenFileName(this,tr("open"),"","Image Files(*.jpg *.png)");
    if(!picPath.isEmpty())//user select file
    {
        QPixmap pix;
        pix.load(picPath);//load image

        //calculate 30% of the window
        int targetWidth = this->width()*0.3;
        int targetHeight = this->height()*0.3;
        //resize the image to 30% of the window and keep the scale
        QPixmap scaledPix = pix.scaled(targetWidth, targetHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        _pixmap = scaledPix;

        //_imageRect.setSize(scaledPix.size());

        QPainter p(&_pixmap);
        //draw in the middle of window
        int x = (this->width() - scaledPix.width()) / 2;
        int y = (this->height() - scaledPix.height()) / 2;
        _imageRect = QRect(x,y,scaledPix.width(),scaledPix.height());
        QPoint topLeft = _imageRect.topLeft();
        QSize size = _imageRect.size();
        //p.drawPixmap(x, y, scaledPix);
        //_isDragging = true;
        qDebug()<<"Can drag and drop";
        _openflag = 1;
        QString pic = pixmapTostring(_pixmap);
        //send pic information
        setPacket->setPic(picPath,pic,topLeft,size);
        update();
    }
}

void whiteboard_new::remoteOpenpic(QString pic){
    qDebug()<<"Receive remote picture";
    //QByteArray imageData = QByteArray::fromStdString(pic.data());
    QPixmap pix = stringTopixmap(pic);
    int targetWidth = this->width()*0.3;
    int targetHeight = this->height()*0.3;
    QPixmap scaledPix = pix.scaled(targetWidth, targetHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _pixmap = scaledPix;
    int x = (this->width() - scaledPix.width()) / 2;
    int y = (this->height() - scaledPix.height()) / 2;
    _imageRect = QRect(x,y,scaledPix.width(),scaledPix.height());
    _openflag = 1;//set pic open flag
    update();
}

QString whiteboard_new::pixmapTostring( const QPixmap& pixmap )
{
  QByteArray byte_array;
  QDataStream data_stream(&byte_array,QIODevice::WriteOnly);
  data_stream<<pixmap;
  QString res = QString::fromLocal8Bit(byte_array.toBase64());
  return res;
}

QPixmap  whiteboard_new::stringTopixmap( const QString& pic )
{
  QByteArray byte_array = QByteArray::fromBase64(pic.toLocal8Bit());
  QPixmap pix;
  QDataStream data_stream(&byte_array,QIODevice::ReadOnly);
  data_stream>>pix;
  return pix;
}

void whiteboard_new::contextMenuEvent(QContextMenuEvent *)  //Right-click menu event
{
    _Rmenu->exec(cursor().pos());//Pop-up menu at cursor position
}

void whiteboard_new::keyPressEvent(QKeyEvent *e) //key press event
{
     //Ctrl+Z Undo
     if (e->key() == Qt::Key_Z && e->modifiers() == Qt::ControlModifier)
     {
         qDebug()<<"Undo";
         if(_shape.size()>0)
         {
             switch(_shape.last())
             {
                 case 1: _lines.pop_back();
                         break;
                 case 6: _lines.pop_back();
                         break;
                 case 2: _rects.pop_back();
                         break;
                 case 3: _ellipse.pop_back();
                         break;
                 case 4: _line.pop_back();
                         break;
                 case 5:{
                    _text.pop_back();
                    _tpoint.pop_back();
                    break;
             }
             default:
                 break;
             }
             _shape.pop_back();
             _drag = 0;//set as not dragging
             update();
         }
     }
     else if (e->key() == Qt::Key_S && e->modifiers() == Qt::ControlModifier)//save
     {
        SavePic();//Ctrl+S Save
     }
}

void whiteboard_new::remoteUndo(){
    qDebug()<<"Remote undo";
    if(_shape_remote.size()>0)
    {
        switch(_shape_remote.last())
        {
            case 1: _lines_remote.pop_back();
                    break;
            case 6: _lines.pop_back();
                    break;
            case 2: _rects_remote.pop_back();
                    break;
            case 3: _ellipse_remote.pop_back();
                    break;
            case 4: _line_remote.pop_back();
                    break;
            case 5:{
                    _text_remote.pop_back();
                    _tpoint_remote.pop_back();
                    break;
                }
        default:
            break;
        }
        _shape_remote.pop_back();
        update();
    }
}
void whiteboard_new::remotePathAppend(const QVector<QPoint> &path, bool isEraser){
        _lines_remote.append(path);
        if(!isEraser){
            _shape_remote.append(1);
            _shape_colors_remote.append(0);
            //_shape_size_remote.append(size);

    }else{
            _shape_remote.append(6);
            _shape_colors_remote.append(7);
        }
}
void whiteboard_new::remoteRectsAppend(const QRect &rect){
    _rects_remote.append(rect);
    _shape_remote.append(2);
    //_shape_colors_remote.append(color);
    //_shape_size_remote.append(size);
    update();
}
void whiteboard_new::remoteEllipseAppend(const QRect &circle){
    _ellipse_remote.append(circle);
    _shape_remote.append(3);
    //_shape_colors_remote.append(color);
    //_shape_size_remote.append(size);
    update();
}

void whiteboard_new::remoteLineAppend(const QRect &line){
    _line_remote.append(line);
    _shape_remote.append(4);
    update();

}
void whiteboard_new::remoteTextAppend(const QString &text, const QPoint &position){
    _tpoint_remote.append(position);
    _text_remote.append(text);
    _shape_remote.append(5);
    update();
}

void whiteboard_new::remoteNoteAppend(const QString &note, const QPoint &position,float side){
    _npoint_remote.append(position);
    _note_remote.append(note);
    _shape_remote.append(7);
    update();
}
whiteboard_new::~whiteboard_new()
{
}
