#include "fileexportwidget.h"
#include<QFont>
#include<QSize>
#include <QDebug>
#include<QFile>
#include<QDir>
#include<QMessageBox>
#include<QScrollBar>
#include "uiutils.h"

FileExportWidget::FileExportWidget(QWidget *parent) : QWidget(parent)
{
    flag = false;
    listWidget = new QListWidget(this);
    listWidget->verticalScrollBar()->setStyleSheet("QScrollBar{width:15px}");
    vb = new QVBoxLayout;
    exportFile = new QPushButton(tr("导出"));
    exportFile->setFixedSize(70, 35);
    selectAll = new QPushButton(tr("全选"));
    selectAll->setFixedSize(70,35);
    vb->addWidget(selectAll);
    vb->addWidget(exportFile);
    QFont f;
    f.setPointSize(18);
    for(int i=0;i<5;i++){
        QListWidgetItem *tmp =  new QListWidgetItem("data", listWidget);
        tmp->setFont(f);
        tmp->setCheckState(Qt::Unchecked);
        tmp->setSizeHint(QSize(0,30));
        listWidget->insertItem(i,tmp);
        list.append(tmp);
    }
    connect(listWidget,SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(setChecked(QListWidgetItem*)));
    connect(selectAll, SIGNAL(clicked()), this, SLOT(selectAll2()));
    connect(exportFile, SIGNAL(clicked()), this, SLOT(exportFile2()));
    hb = new QHBoxLayout;
    hb->addWidget(listWidget);
    hb->addLayout(vb);
    setLayout(hb);
}

FileExportWidget::~FileExportWidget(){
    delete listWidget;
    delete vb;
    delete hb;
    delete exportFile;
    delete selectAll;
}

void FileExportWidget::setChecked(QListWidgetItem* item){
    if(item->checkState() ==Qt::Unchecked)
        item->setCheckState(Qt::Checked);
    else
        item->setCheckState(Qt::Unchecked);
}

void FileExportWidget::selectAll2() {
   QList<QListWidgetItem*>::iterator item;
   if (flag)
       for(item = list.begin(); item != list.end();item++)
           (*item)->setCheckState(Qt::Unchecked);
   else
       for(item = list.begin(); item != list.end();item++)
           (*item)->setCheckState(Qt::Checked);
   flag = !flag;
}

void FileExportWidget::exportFile2() {
    if(!UiUtils::mount_usb())
        return;
    exportFile->setCheckable(false);
    if(QFile::copy("/root/qt.sh", QString(UiUtils::mount_point)+QString("/xxx")))
        QMessageBox::information(0, "文件拷贝","文件拷贝成功");
    else
          QMessageBox::information(0, "失败",QString("文件拷贝失败!\n目标文件已经存在!"));
   UiUtils::umount_usb();
    exportFile->setCheckable(true);
}
