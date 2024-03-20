#include "mainwidget.h"
#include <QApplication>
#include <QSysInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    qDebug()<<QSysInfo::productType();
    if (QSysInfo::productType() != "windows")
        qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QApplication a(argc, argv);
    MainWidget w(0);
    w.show();
    return a.exec();
}
