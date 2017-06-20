#ifndef INFOBEAN_H
#define INFOBEAN_H
#include<QString>

class InfoBean
{
public:
    InfoBean();
    void setBridgeResistor(QString _br);
    QString getBridgeResistor();
    void setInsulationResistor(QString _in);
    QString getInsulationResistor();
    void setTemperature(QString _te);
    QString getTemperature();
    void setVoltage(QString _vo);
    QString getVoltage();
    void setPressure(QString _pr);
    QString getPressure();

private:
    QString bridgeResistor;
    QString insulationResistor;
    QString temperature;
    QString voltage;
    QString pressure;
};

#endif // INFOBEAN_H
