#include "uiutils.h"
#include <sys/mount.h>
#include <errno.h>
#include <string.h>
#include <QFile>
#include <QDebug>
#include <QDial>
#include <QDir>
#include<QCalendarWidget>
#include<QMessageBox>

int UiUtils::screenHeight; //单位px
int UiUtils::screenWidth;  //单位px
UiUtils::UiUtils()
{

}

void UiUtils::setScreen() {
     QRect ret = QApplication::desktop()->availableGeometry();
     UiUtils::screenHeight = ret.height();
     UiUtils::screenWidth = ret.width();
}
QString UiUtils::double2string(double d) {
    return QString::number(d, 'f', 2);
}

const char * UiUtils::dev = "/dev/sda1";
const char * UiUtils::mount_point = "/usb";
const char * UiUtils::filsys_type = "vfat";
const char * UiUtils::data_dir = "/data/";
MessureData UiUtils::messure_data;
bool UiUtils::mount_usb()
{
    QFile dir(dev);
    QDir usb(mount_point);
    QDir ddir(data_dir);
    ddir.mkdir(data_dir);
    if (!dir.exists()){
        QMessageBox::warning(0, "文件拷贝", "未发现u盘!");
        return false;
    }
    if (!usb.exists()){
        qDebug()<<mount_point<<" note exists, creating...";
        if(!usb.mkdir(mount_point)){
            QMessageBox::warning(0, "失败", QString( "创建/usb失败!\n")+QString(strerror(errno)));
             return false;
        }
    }
    if(mount(dev, mount_point, filsys_type, 0, NULL) < 0){
         qDebug()<<strerror(errno);
        QMessageBox::warning(0,  "失败", QString("挂载U盘失败!\n") + QString(strerror(errno)));

         return false;
    }
    return true;
}
bool UiUtils::umount_usb()
{
    return umount(mount_point) == 0;
}

UiUtils::~UiUtils(){

}
