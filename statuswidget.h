#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QWidget>
#include <QTextCodec>
class statusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit statusWidget(QWidget *parent = 0);
    
signals:
    
public slots:
    void showUpdateInfo();
};

#endif // STATUSWIDGET_H

/*
  状态栏
*/
