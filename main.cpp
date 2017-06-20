#include <QtGui/QApplication>
#include "mainwidget.h"
#include <QTextCodec>
#include "frminput.h"
#include "titlewidget.h"
#include<uiutils.h>

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
    app.setFont(QFont("simsun"));
    UiUtils::setScreen();
    mainWidget mainW;

    mainW.show();
    //frmInput::Instance()->Init("control", "black", 10, 10);

    return app.exec();
}
