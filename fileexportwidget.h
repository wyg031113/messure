#ifndef FILEEXPORTWIDGET_H
#define FILEEXPORTWIDGET_H

#include <QWidget>
#include<QListWidget>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QPushButton>
#include<QListWidgetItem>
#include<QList>
class FileExportWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileExportWidget(QWidget *parent = 0);
    ~FileExportWidget();
    QListWidget *listWidget;
    QVBoxLayout *vb;
    QHBoxLayout *hb;
    QPushButton *exportFile;
    QPushButton *selectAll;
    QList<QListWidgetItem*> list;
    bool flag;
signals:

public slots:
    void setChecked(QListWidgetItem*);
    void selectAll2();
    void exportFile2();
    void refresh_list();
};

#endif // FILEEXPORTWIDGET_H
