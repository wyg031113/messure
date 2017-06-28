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
    self_test_start,
};
CollectionThread::start_stop_fun_t CollectionThread::stop_funs[MeterType_NR] =
{
    in_out_resis_stop,
    resistance_stop,
    temperature_stop,
    pressure_stop,
    self_test_stop,
};
CollectionThread::CollectionThread()      //程序启动时调用初始化函数
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
                if(start_funs[state] != NULL){
                    int ret = start_funs[state]();
                    if(ret < 0){
                        qDebug()<<" NotStarted:state:"<<state;
                     }
                }else{
                    qDebug()<<"warning: start_funs is NULL!";
                }
                bStart = true;
            }
            i++;
            if(state == SELF_TEST){
                str = "设备自检中" + QString::number(i);
                double U0;
                int ret;
                 //读取自检电压
                ret = self_test_messure(&U0);
                str  = "";
                if(ret < 0){
                    str = "读取自检电压 失败! \n";
                }

               str += QString::number(i) + QString(".自检电压: ") +
                       UiUtils::double2string(U0) + QString(" V ");
               if(U0 >= 2.8 && U0 <= 3.2){
                   str += "合格!";
               }else{
                   str += "不合格";
               }
            }else  if(state == IN_OUT_RESIS) {
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
               str = "";
               if(ret < 0){
                   str = "读取绝缘电阻 失败!\n";
               }
                str +=  QString::number(i) + QString(".绝缘电阻:") +
                           UiUtils::double2string(ins_res) + QString(" 兆欧");
           } else if (state ==TEMPERATURE) {
               int ret;
               double temp;
               //读取测点温度
               str = "";
               ret = temperature_mesure(3.13,  get_temp_tab(), get_ntemp(), &temp);
               if(ret < 0){
                   str = "读取测点温度 失败!\n";
               }
               str += QString::number(i) + QString(".测点温度:")  +
                         UiUtils::double2string(temp) + QString(" 摄氏度");


           } else if (state == 5) {
                //读取电压值
               str = "读取电压值" + QString::number(i);
           } else if (state == PRESSURE) {
               int ret;
               double pressure;
               //读取压力值
               str = "";
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
                bStart = false;
                last_state = -1;
                if(state < 0 || state >= CollectionThread::MeterType_NR){
                    qDebug()<<"in start work:Eror state:"<<state;
                }else{
                    if(stop_funs[state])
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

void CollectionThread::start2() {    //开始按钮
    working = true;
    push.wakeAll();      //唤醒线程
}

void CollectionThread::stop2() {
    working = false;
}

void CollectionThread::testBridgeResistor() {state = IN_OUT_RESIS;}
void CollectionThread::testInsulationResistor() {state = INS_RES;}
void CollectionThread::testTemperature(){state = TEMPERATURE;}
void CollectionThread::testVoltage() {state = 5;}
void CollectionThread::testPressure() {state = PRESSURE;}
void CollectionThread::testSelf(){state = SELF_TEST;}
void CollectionThread::finish()
{
    qDebug()<<"Finish";
    bFinish = true;
    working = false;
    start2();
    CollectionThread::getInstance()->wait();
     qDebug()<<"Finish";
}
