#include "statuswidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include<QMessageBox>
#include<QPushButton>
#include<uiutils.h>
#include "clickedlabel.h"
#include <QApplication>
#include <QFile>
#include <QProcess>
#include <QStringList>
#include <QDebug>
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
    if(!UiUtils::mount_usb()){
        return;
    }
   QString app_dir = QApplication::applicationDirPath();
   QString app_file = "/opt/measure/measure";  //QApplication::applicationFilePath();
   QString app_name  = "measure";     // QApplication::applicationName();
   QString src = QString(UiUtils::mount_point) + QString("/")+QString(app_name);
   qDebug()<<"Srcfile:"<<src;
   QFile  dst(app_file);
   QFile src_file(src);
   if(!src_file.exists()){
       QMessageBox::information(0,"更新失败", "没有发现新版本!");
   }else{
       dst.rename("measure.bak");
    if(QFile::copy(src, app_file)){
         QMessageBox::information(0,"版本更新", "版本更新成功!\n点击确定重新启动本程序!");
         qApp->quit();
         QStringList args;
         args.append("-qws");
         QProcess::startDetached(app_file, args);
         dst.remove();
    }else{
        dst.rename(app_name);
         QMessageBox::information(0,"更新失败", "没有发现新版本!");
         QFile bak_file("/opt/measure/measure.bak");
         bak_file.remove();
    }
   }

    UiUtils::umount_usb();
}
