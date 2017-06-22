#include "collectionthread.h"
#include<QDebug>
CollectionThread* CollectionThread::single = new CollectionThread;
int CollectionThread::state = 0;
bool CollectionThread::working = false;
bool volatile CollectionThread::bFinish = false;
QWaitCondition CollectionThread::push;
QMutex CollectionThread::mutex;
CollectionThread::CollectionThread()
{

}

void CollectionThread::run() {
    int i=0;
    QString str;
    while(!bFinish) {
        if (working) {
            i++;
           sleep(1);
           if(state == 0) {
               //读取电桥电阻
               str = "读取电桥电阻" + QString::number(i);
           } else if (state == 1) {
               //读取绝缘电阻
                str = "读取绝缘电阻" + QString::number(i);
           } else if (state == 2) {
               //读取测点温度
               str = "读取测点温度" + QString::number(i);
           } else if (state == 3) {
               //读取电压值
               str = "读取电压值" + QString::number(i);
           } else if (state == 4) {
               //读取压力值
               str = "读取压力值" + QString::number(i);
           }
           emit modify(str);
        }
        else {
            push.wait(&mutex);
        }
    }
    qDebug()<<"Working thread finished.";
}

CollectionThread* CollectionThread::getInstance(){
    if(!single)
        single = new CollectionThread;
    return single;
}

CollectionThread::~CollectionThread(){
    if(single)
        delete single;
}

void CollectionThread::start2() {
    working = true;
    push.wakeAll();
}

void CollectionThread::stop2() {
    working = false;
}

void CollectionThread::testBridgeResistor() {state = 0;}
void CollectionThread::testInsulationResistor() {state = 1;}
void CollectionThread::testTemperature(){state = 2;}
void CollectionThread::testVoltage() {state = 3;}
void CollectionThread::testPressure() {state = 4;}
void CollectionThread::finish()
{
    qDebug()<<"Finish";
    bFinish = true;
    start2();
    CollectionThread::getInstance()->wait();
     qDebug()<<"Finish";
}
