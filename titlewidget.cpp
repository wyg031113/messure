#include "titlewidget.h"
#include "sysbutton.h"
#include "mypushbutton.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QApplication>
#include <QMessageBox>
#include <QMouseEvent>
#include<uiutils.h>
#include "collectionthread.h"
#include <QMessageBox>
#include "gpio_ctl.h"
titleWidget::titleWidget(QWidget *parent) :
    QWidget(parent)
{
    QPalette palette;
    palette.setColor(QPalette::WindowText,Qt::white);
    this->setPalette(palette);
    QLabel *versionText=new QLabel(tr("设备检测仪(已连接)"));
    btnSkin=new myPushButton("SkinButton.png",tr("换肤"));
    connect(btnSkin,SIGNAL(clicked()),this,SIGNAL(showSkin()));
    btnMenuBar=new sysButton("title_bar_menu.png",tr("主菜单"));
    btnMin=new sysButton("sys_button_min.png",tr("最小化"));
    connect(btnMin,SIGNAL(clicked()),this,SIGNAL(showMin()));
    btnMax=new sysButton("sys_button_max.png",tr("最大化"));
    connect(btnMax,SIGNAL(clicked()),this,SIGNAL(showMax()));
    btnClose=new sysButton("sys_button_close.png",tr("关闭"));
   // connect(btnClose,SIGNAL(clicked()),qApp,SLOT(quit()));
    connect(btnClose,SIGNAL(clicked()),this,SLOT(quit_main()));
    btnClose->setFixedHeight(30);
    btnSkin->setFixedHeight(30);


    QHBoxLayout *mainLayout=new QHBoxLayout;
    mainLayout->addWidget(versionText,0,Qt::AlignVCenter);
    versionText->setContentsMargins(5,0,0,0);
    mainLayout->addStretch();
    mainLayout->addWidget(btnSkin,0,Qt::AlignVCenter);
    btnSkin->setContentsMargins(0,0,5,0);
   // mainLayout->addWidget(btnMenuBar);
    //mainLayout->addWidget(btnMin);
    //mainLayout->addWidget(btnMax);
    mainLayout->addWidget(btnClose);

    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
    setFixedHeight((int)(UiUtils::screenHeight *0.083));

    isMove=false;
}
void titleWidget::quit_main()
{
    int ret = QMessageBox::question(NULL, "关机","是否关机?", QMessageBox::Yes|QMessageBox::No);
    if(ret == QMessageBox::Yes){
        qApp->quit();
        set_gpio_value(GPIO16, 1);
    }
}

void titleWidget::mousePressEvent(QMouseEvent *e)
{
    //如果点在按钮区域  不会移动 立即返回
    if(e->x()+170>=this->width())
        return;
    pressedPoint=e->pos();
    isMove=true;
}
void titleWidget::mouseMoveEvent(QMouseEvent *e)
{
    if( (e->buttons()&Qt::LeftButton) && isMove)
    {
        static QWidget* parWidget=this->parentWidget();
        QPoint nowParPoint=parWidget->pos();
        nowParPoint.setX(nowParPoint.x()+e->x()-pressedPoint.x());
        nowParPoint.setY(nowParPoint.y()+e->y()-pressedPoint.y());
        parWidget->move(nowParPoint);
    }
}
void titleWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(isMove)
        isMove=false;
}
void titleWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->x()+170>=this->width())
        return;
    emit showMax();
}
