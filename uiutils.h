#ifndef UIUTILS_H
#define UIUTILS_H
#include<QApplication>
#include<QDesktopWidget>
#include <QRect>
#include <QString>

class UiUtils
{
public:
    UiUtils();
    ~UiUtils();
    static int screenHeight; //单位px
    static int screenWidth;  //单位px
    static void setScreen();
    static QString double2string(double d);
    static bool mount_usb();
    static bool umount_usb();
    static const char *dev;
    static  const char *mount_point;
    static const char *filsys_type;
};

#endif // UIUTILS_H
