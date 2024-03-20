#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("model selection");
    this->setAttribute(Qt::WA_DeleteOnClose,false);

    //initalization
    createSessionWidget = nullptr;
    joinSessionWidget = nullptr;

    //connect signals and slots
    connect(ui->createButton,&QPushButton::clicked,this,&MainWidget::createButtonClicked);
    connect(ui->joinButton,&QPushButton::clicked,this,&MainWidget::joinButtonClicked);

}

void MainWidget::createButtonClicked(){
    this->hide();

    createSessionWidget = new CreateSession(0);

    //back to main widget
    connect(createSessionWidget,&CreateSession::backSignal,[=](){
       this->show();
       createSessionWidget->close();
    });

    createSessionWidget->show();
}

void MainWidget::joinButtonClicked(){
    this->hide();

    joinSessionWidget = new JoinSession(0);

     //back to main widget
    connect(joinSessionWidget,&JoinSession::backSignal,[=](){
       joinSessionWidget->close();
       this->show();
    });


    joinSessionWidget->show();
}

MainWidget::~MainWidget()
{
    delete ui;
}

