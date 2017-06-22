#include "collectionthread.h"
#include "uiutils.h"
#include "meter.h"
#include<QDebug>
CollectionThread* CollectionThread::single = new CollectionThread;
int CollectionThread::state = 0;
bool CollectionThread::working = false;
bool volatile CollectionThread::bFinish = false;
QWaitCondition CollectionThread::push;
QMutex CollectionThread::mutex;
CollectionThread::start_stop_fun_t CollectionThread::start_funs[MeterType_NR] =
{
    in_out_resis_start,
    resistance_start,
    temperature_start,
    pressure_start,
};
CollectionThread::start_stop_fun_t CollectionThread::stop_funs[MeterType_NR] =
{
    in_out_resis_stop,
    resistance_stop,
    temperature_stop,
    pressure_stop,
};
CollectionThread::CollectionThread()
{
    int ret;
    ret = gpio_init();
    if(ret < 0){
        qDebug()<<"GPIO INIT failed!\n";
    }

    ret = setup_ad7124("/dev/spidev3.0");
    if(ret < 0){
        qDebug("Setup AD7124 failed!");
    }

   ret =  load_temp("/temp.txt");
   if(ret < 0){
       qDebug("load resistance to temperature table failed!");
   }
}


void CollectionThread::run() {
    int i=0;
    QString str;
    int last_state = -1;
    bool bStart = false;
    while(!bFinish) {
        if (working) {
            if(bFinish)
                break;
            qDebug()<<"workthread running:state:"<<state;
            if(state < 0 || state >= CollectionThread::MeterType_NR){
                qDebug()<<"in start work:Eror state:"<<state;
            }
            if(state != last_state){
                last_state = state;
                i = 0;
                int ret = start_funs[state]();
                if(ret < 0){
                    qDebug()<<" NotStarted:state:"<<state;
                 }
                bStart = true;
            }
            i++;

           if(state == IN_OUT_RESIS) {
               double din, dout;
               int ret;
                ret = in_out_resis_mesure(3.31, &din, &dout);
                str = "";
                if(ret < 0){
                    str = "读取电桥电阻 失败! \n";
                }
               //读取电桥电阻
               str += QString::number(i) + QString(".读取电桥电阻:\n") + QString("输入电阻:") + UiUtils::double2string(din) +
                        QString("千欧\n输出电阻: ") + UiUtils::double2string(dout) + QString("千欧 ");
           } else if (state == INS_RES) {
               double ins_res;
               //读取绝缘电阻
               int ret = resistance_mesure(&ins_res);
               if(ret < 0){
                   str = "读取绝缘电阻 失败!\n";
               }
                str +=  QString::number(i) + QString(".绝缘电阻:") +
                           UiUtils::double2string(ins_res) + QString(" 兆欧");
           } else if (state ==TEMPERATURE) {
               int ret;
               double temp;
               //读取测点温度
               ret = temperature_mesure(3.13,  get_temp_tab(), get_ntemp(), &temp);
               if(ret < 0){
                   str = "读取测点温度 失败!\n";
               }
               str += QString::number(i) + QString(".测点温度:")  +
                         UiUtils::double2string(temp) + QString(" 摄氏度");


           } else if (state == 4) {
                //读取电压值
               str = "读取电压值" + QString::number(i);
           } else if (state == PRESSURE) {
               int ret;
               double pressure;
               //读取压力值
               ret = pressure_mesure(&pressure);
               if(ret < 0){
                   str = "读取压力值 失败!\n";
               }
               str += QString::number(i) + QString(".压力值:")  +
                         UiUtils::double2string(pressure) + QString(" 帕");
           }
           emit modify(str);
            sleep(1);
        }
        else {
            if(!bFinish && bStart){
                last_state = -1;
                if(state < 0 || state >= CollectionThread::MeterType_NR){
                    qDebug()<<"in start work:Eror state:"<<state;
                }else{
                    stop_funs[state]();
                }
            }
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

void CollectionThread::testBridgeResistor() {state = IN_OUT_RESIS;}
void CollectionThread::testInsulationResistor() {state = INS_RES;}
void CollectionThread::testTemperature(){state = TEMPERATURE;}
void CollectionThread::testVoltage() {state = 4;}
void CollectionThread::testPressure() {state = PRESSURE;}
void CollectionThread::finish()
{
    qDebug()<<"Finish";
    bFinish = true;
    working = false;
    start2();
    CollectionThread::getInstance()->wait();
     qDebug()<<"Finish";
}
