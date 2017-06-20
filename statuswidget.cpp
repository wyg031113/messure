#include "statuswidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include<QMessageBox>
#include<QPushButton>
#include<uiutils.h>
#include "clickedlabel.h"
statusWidget::statusWidget(QWidget *parent) :
    QWidget(parent)
{
    QPalette palette;
    palette.setColor(QPalette::WindowText,Qt::white);
    this->setPalette(palette);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));//tr使用的编码
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));//QString使用的编码
    QLabel *labelVersion=new QLabel(tr("当前版本:2016.12.31"));
    ClickedLabel *labelUpdate=new ClickedLabel();
    labelUpdate->setText(tr("检查更新"));
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::blue);
    labelUpdate->setPalette(pa);
    QLabel *labelConn360=new QLabel(tr("已连接"));
    connect(labelUpdate, SIGNAL(clicked()), this, SLOT(showUpdateInfo()));
    QHBoxLayout *mainLayout=new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addWidget(labelVersion);
    labelVersion->setContentsMargins(5,0,0,0);
    mainLayout->addWidget(labelUpdate);
    mainLayout->addStretch();
    mainLayout->addWidget(labelConn360);
    labelConn360->setContentsMargins(0,0,40,0);
    setLayout(mainLayout);

    setFixedHeight((int)(UiUtils::screenHeight * 0.053));
}

void statusWidget::showUpdateInfo() {
    QMessageBox::information(0,"版本更新", "当前已是最新版本!");
}