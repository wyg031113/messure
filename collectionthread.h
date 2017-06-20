#ifndef COLLECTIONTHREAD_H
#define COLLECTIONTHREAD_H

#include<QThread>
#include "infobean.h"
#include<QWaitCondition>
#include<QMutex>
/*
  QThread类先term
*/
class CollectionThread : public QThread {
    Q_OBJECT

public:
    ~CollectionThread();
     static CollectionThread* getInstance();
protected:
    void run();
signals:
    void modify(QString);
private:
    static CollectionThread *single;  //单例
    static int state; //测试，0表示电桥电阻，1表示绝缘电阻，2表示测点温度,3表示测点电压，4表示测点压力
    static QWaitCondition push;
    static bool working;
    CollectionThread();
    static QMutex mutex;
public slots:
    static void start2();
    static void stop2();
    void testBridgeResistor();
    void testInsulationResistor();
    void testTemperature();
    void testVoltage();
    void testPressure();
};

#endif // COLLECTIONTHREAD_H
