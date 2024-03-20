#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "createsession.h"
#include "joinsession.h"
#include "clientwait.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent);
    ~MainWidget();

private slots:
    void createButtonClicked();
    void joinButtonClicked();


private:
    Ui::MainWidget *ui;
    //click create button to open new portal
    CreateSession *createSessionWidget;
    //click join button to open new portal
    JoinSession *joinSessionWidget;
};
#endif // MAINWIDGET_H
