#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include "fileexportwidget.h"
class contentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit contentWidget(QWidget *parent = 0);
    ~contentWidget();
    QStackedWidget *stack;
     FileExportWidget *fileExportWidget;
signals:
    
public slots:
    
};

#endif // CONTENTWIDGET_H

/*
  主界面上的内容区域
*/
