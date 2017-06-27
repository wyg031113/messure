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
    typedef int (*start_stop_fun_t)(void);
    enum MeterType
    {
        IN_OUT_RESIS,
        INS_RES,
        TEMPERATURE,
        PRESSURE,
        SELF_TEST,
        MeterType_NR
    };
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
    static volatile bool bFinish;
    static start_stop_fun_t start_funs[MeterType_NR];
    static start_stop_fun_t stop_funs[MeterType_NR];
public slots:
    static void start2();
    static void stop2();
    void testBridgeResistor();
    void testInsulationResistor();
    void testTemperature();
    void testVoltage();
    void testPressure();
    void testSelf();
    static void finish();

};

#endif // COLLECTIONTHREAD_H
