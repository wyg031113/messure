#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>
#include "toolbutton.h"
class toolWidget : public QWidget
{
    Q_OBJECT
public:
    explicit toolWidget(QWidget *parent = 0);
    toolButton *tbTiJian;
    toolButton *tbMuMa;
    toolButton *tbLouDong;
signals:
    
public slots:
};

#endif // TOOLWIDGET_H
