#include "infobean.h"

InfoBean::InfoBean()
{

}

void InfoBean::setBridgeResistor(QString _br) {
    bridgeResistor = _br;
}

QString InfoBean::getBridgeResistor() {
    return bridgeResistor;
}

void InfoBean::setInsulationResistor(QString _in) {
    insulationResistor = _in;
}

QString InfoBean::getInsulationResistor() {
    return insulationResistor;
}

void InfoBean::setTemperature(QString _te) {
    temperature = _te;
}

QString InfoBean::getTemperature() {
    return temperature;
}

void InfoBean::setVoltage(QString _vo) {
    voltage = _vo;
}

QString InfoBean::getVoltage() {
    return voltage;
}

void InfoBean::setPressure(QString _pr) {
    pressure = _pr;
}

QString InfoBean::getPressure() {
    return pressure;
}
