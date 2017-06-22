#include "uiutils.h"
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
UiUtils::~UiUtils(){

}
