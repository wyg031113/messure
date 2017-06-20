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
    tbTiJian=new toolButton(":/img/toolWidget/status.png",tr("参数设置"), 0);
    tbMuMa=new toolButton(":/img/toolWidget/config.png",tr("结果录入"), 1);
    tbLouDong=new toolButton(":/img/toolWidget/export.png",tr("文件导出"), 2);

    QLabel *logoLabel=new QLabel;
    QPixmap pixmap(":/img/logo.png");
    logoLabel->setPixmap(pixmap);
    logoLabel->setFixedSize(pixmap.size());
    logoLabel->setCursor(Qt::PointingHandCursor);

    QHBoxLayout *mainLayout=new QHBoxLayout;
    mainLayout->setContentsMargins(5,0,5,0);
    mainLayout->addWidget(tbTiJian);
    mainLayout->addWidget(tbMuMa);
    mainLayout->addWidget(tbLouDong);
    mainLayout->addStretch();
    mainLayout->addWidget(logoLabel);
    setLayout(mainLayout);

    setFixedHeight((int)(UiUtils::screenHeight * 0.167));
}
