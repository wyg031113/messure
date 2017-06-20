#ifndef BRIDGERESISTOR
#define BRIDGERESISTOR
#include<QString>
class BridgeResistor {
public:
      BridgeResistor();
     ~BridgeResistor();
      void start();
      //QString get();
     // void stop();

private:
      int stauts; //0表示停止状态，1表示就绪状态，2表示工作状态
};
#endif // BRIDGERESISTOR

