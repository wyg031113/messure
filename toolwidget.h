#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include "toolbutton.h"
class toolWidget : public QWidget
{
    Q_OBJECT
public:
    explicit toolWidget(QWidget *parent = 0);
    toolButton *tbTiJian;
    toolButton *tbMuMa;
    toolButton *tbLouDong;
    QLabel *warn;
    QLabel *warn_text;
    QTimer *timer;
    void start_timer();
    void stop_timer();
signals:
    
public slots:
    void timer_done();
};

#endif // TOOLWIDGET_H
