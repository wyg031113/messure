#ifndef UIUTILS_H
#define UIUTILS_H
#include<QApplication>
#include<QDesktopWidget>
#include <QRect>

class UiUtils
{
public:
    UiUtils();
    ~UiUtils();
    static int screenHeight; //单位px
    static int screenWidth;  //单位px
    static void setScreen();
};

#endif // UIUTILS_H
