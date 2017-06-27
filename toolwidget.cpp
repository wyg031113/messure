#include "toolwidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include<uiutils.h>
toolWidget::toolWidget(QWidget *parent) :
    QWidget(parent)
{
    QPalette palette;
    palette.setColor(QPalette::WindowText,Qt::white);
    this->setPalette(palette);
    tbTiJian=new toolButton(":/img/toolWidget/status.png",tr("开始测量"), 0);
    tbMuMa=new toolButton(":/img/toolWidget/config.png",tr("参数设置"), 1);
    tbLouDong=new toolButton(":/img/toolWidget/export.png",tr("文件导出"), 2);

    QLabel *logoLabel=new QLabel;
    QPixmap pixmap(":/img/logo.png");
    logoLabel->setPixmap(pixmap);
    logoLabel->setFixedSize(pixmap.size());
    logoLabel->setCursor(Qt::PointingHandCursor);

    warn=new QLabel;
    QPixmap warn_map(":/img/warning.png");
    warn->setPixmap(warn_map);
    warn->setFixedSize(warn_map.size());
    warn->setCursor(Qt::PointingHandCursor);

    QHBoxLayout *mainLayout=new QHBoxLayout;
    mainLayout->setContentsMargins(5,0,5,0);
    mainLayout->addWidget(tbTiJian);
    mainLayout->addWidget(tbMuMa);
    mainLayout->addWidget(tbLouDong);
    mainLayout->addStretch(1);
    mainLayout->addWidget(warn);
    mainLayout->addStretch(1);
    mainLayout->addWidget(logoLabel);
    setLayout(mainLayout);
    setFixedHeight((int)(UiUtils::screenHeight * 0.167));
    start_timer();
}
 void toolWidget::timer_done()
 {
     if(warn->isVisible()){
         warn->setVisible(false);
     }else{
         warn->setVisible(true);
     }
 }
 void  toolWidget::start_timer()
 {
     timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(timer_done()));
     timer->start(1000);
 }

 void  toolWidget::stop_timer()
 {
     timer->stop();;
     warn->setVisible(false);
 }
