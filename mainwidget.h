#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui/QWidget>
#include <QTextCodec>
#include "toolwidget.h"
class mainWidget : public QWidget
{
    Q_OBJECT
    
public:
    mainWidget(QWidget *parent = 0);
    ~mainWidget();

protected:
    void paintEvent(QPaintEvent *);
private:
    QString bkPicName;//主窗口背景图片的名称
    QPoint normalTopLeft;//正常显示的窗口左上角坐标
     toolWidget *toolW;

private slots:
    void showMax();
    void showMin();
    void showSkinWidget();
    void setPicName(QString);
    void MuMaClicked();
};

#endif // MAINWIDGET_H
