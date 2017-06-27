#include <QtGui/QApplication>
#include "mainwidget.h"
#include <QTextCodec>
#include "frminput.h"
#include "titlewidget.h"
#include<uiutils.h>
#include <iostream>
#include <QDebug>
#include "collectionthread.h"
using namespace std;
int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));//tr使用的编码
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));//QString使用的编码

    //设置默认颜色
    QPalette palette;
    palette.setColor(QPalette::WindowText,Qt::black);
    app.setPalette(palette);
    //设置字体为宋体
   // QFont qf(QString("WenQuanYi Zen Hei"));
     QFont qf(QString("SimSun"));
    cout<<"QTFonstSize:"<<qf.pointSize()<<endl;
    qf.setPointSize(12);
    app.setFont(qf);
    QFont font;
    qDebug()<<font.family();


    UiUtils::setScreen();
    mainWidget mainW;

    mainW.show();
   frmInput::Instance()->Init("control", "black", 10, 10);
   //frmInput::Instance()->hide();
    int ret =  app.exec();

    qDebug()<<"Main finished.";
        CollectionThread::finish();
    return ret;
}
