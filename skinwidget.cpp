#include "skinwidget.h"
#include <QSignalMapper>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>

skinWidget::skinWidget(QString picName,QWidget *parent) :bkPicName(picName),
    QWidget(parent)
{
    QSignalMapper *signalMapper=new QSignalMapper(this);
    QStringList bkPicName;
    bkPicName<<":/img/skin/blackPoint.jpg"<<":/img/skin/blueSky.jpg"
               <<":/img/skin/greenWorld.jpg"<<":/img/skin/oldWood.jpg"<<
               ":/img/skin/pinkLove.jpg"<<":/img/skin/redThunder.jpg"<<":/img/skin/sixYears.jpg";
    QStringList tip;
    tip<<tr("优雅爵士")<<tr("天空蓝")<<
             tr("青青世界")<<tr("古典木纹")<<tr("粉色之恋")<<tr("红色惊雷");

    QGridLayout *gridLayout=new QGridLayout;
    gridLayout->setSpacing(0);
    int r=0,c=0;
    for(int i=0;i<6;i++)
    {
        QPushButton *btn=new QPushButton;
       // btn->setFlat(true);
        QIcon icon(bkPicName[i].left(bkPicName[i].indexOf("."))+"Small.jpg");
        btn->setIcon(icon);
        btn->setIconSize(QSize(97,62));
        btn->setToolTip(tip[i]);
        connect(btn,SIGNAL(clicked()),signalMapper,SLOT(map()));
        signalMapper->setMapping(btn,bkPicName[i]);
        if(i%3==0)
        {
            r++;
            c=0;
        }
        gridLayout->addWidget(btn,r,c++);
    }
    connect(signalMapper,SIGNAL(mapped(QString)),this,SIGNAL(changeSkin(QString)));
    connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(setSkin(QString)));

    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addWidget(new QLabel(tr("更换皮肤")),0,Qt::AlignHCenter);
    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);

    setWindowFlags(Qt::Popup);
}

void skinWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(QBrush(QPixmap(bkPicName)));
    painter.setRenderHints(QPainter::Antialiasing,true);
    painter.setPen(Qt::black);
    painter.drawRect(rect());
}
void skinWidget::setSkin(QString picName)
{
    bkPicName=picName;
    update();
}
