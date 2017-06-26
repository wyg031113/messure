#include "contentwidget.h"
#include <QPalette>
#include <QVBoxLayout>
#include <QLabel>
#include<QLineEdit>
#include<QPushButton>
#include "statusinfo.h"
#include "recordinfo.h"
#include "collectionthread.h"
#include"fileexportwidget.h"
contentWidget::contentWidget(QWidget *parent) :
    QWidget(parent)
{
    QPalette palette;
    palette.setBrush(QPalette::Window,QBrush(QColor(250,255,240,150)));
    setPalette(palette);
    setAutoFillBackground(true);
    stack = new QStackedWidget(this);
    stack->setFrameStyle(QFrame::Panel|QFrame::Raised);
    StatusInfo *statusInfo = new StatusInfo;
    RecordInfo *recordInfo = new RecordInfo;
    fileExportWidget = new FileExportWidget;
    stack->addWidget(statusInfo);
    stack->addWidget(recordInfo);
    stack->addWidget(fileExportWidget);
    QVBoxLayout *qv = new QVBoxLayout;
    qv->addWidget(stack);
    setLayout(qv);
}

contentWidget::~contentWidget() {

}
