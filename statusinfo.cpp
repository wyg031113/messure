#include "statusinfo.h"
#include <QApplication>
#include<QString>
#include<QDesktopWidget>
#include <QRadioButton>

//获取一个后台线程用来采集数据
CollectionThread* StatusInfo::collectionThread = CollectionThread::getInstance();

StatusInfo::StatusInfo(QWidget *parent) : QWidget(parent) {
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::black);
    this->setPalette(pe);

    //生成按键
    selfTestBtn = new QRadioButton(tr("自检"));
    selfTestBtn->setFixedSize(50, 35);
    selfTestBtn->setChecked(true);
    CollectionThread::getInstance()->testSelf();

    bridgeResistorBtn = new QRadioButton(tr("电桥电阻"));
    bridgeResistorBtn->setFixedSize(90, 35);

    insulationResistorBtn = new QRadioButton(tr("绝缘电阻"));
    insulationResistorBtn->setFixedSize(80, 35);
    temperatureBtn = new QRadioButton(tr("测点温度"));
    temperatureBtn->setFixedSize(90, 35);
    voltageBtn = new QRadioButton(tr("零点电压"));
    voltageBtn->setFixedSize(100, 35);
    pressureBtn = new QRadioButton(tr("压力值"));
    pressureBtn->setFixedSize(90, 35);
    start = new QPushButton(tr("开始"));

    start->setFixedSize(90, 35);
    start->setEnabled(true);
    stop = new QPushButton(tr("停止"));

    stop->setFixedSize(90, 35);
    stop->setEnabled(false);

    //将功能按键以水平方式布局
    firstLayout = new QHBoxLayout;
    firstLayout->addWidget(selfTestBtn);
    firstLayout->addWidget(bridgeResistorBtn);
    firstLayout->addWidget(insulationResistorBtn);
    firstLayout->addWidget(temperatureBtn);
    firstLayout->addWidget(voltageBtn);
    firstLayout->addWidget(pressureBtn);

    //工作按键垂直方式布局
    QVBoxLayout *tmp = new QVBoxLayout;
    tmp->addWidget(start);
    tmp->addWidget(stop);

    //第二个布局
    secondLayout = new  QHBoxLayout;
    contentText = new QTextBrowser;
    contentText->setReadOnly(true);
    secondLayout->addLayout(tmp);
    secondLayout->addWidget(contentText);
    contentText->setText("欢迎使用测量仪!\n请选择上方的测量项目, 并点击开始.");
    contentText->setProperty("noinput", true);
    //获取监测设备状态
    CollectionThread* ctd = CollectionThread::getInstance();
    connect(start, SIGNAL(clicked()),ctd, SLOT(start2()));
    connect(start, SIGNAL(clicked()), this, SLOT(startDisable()));

    connect(stop, SIGNAL(clicked()), ctd, SLOT(stop2()));
    connect(stop, SIGNAL(clicked()), this, SLOT(stopDisable()));
    connect(ctd, SIGNAL(modify(QString)), this, SLOT(chageStatus(QString)));

    connect(selfTestBtn, SIGNAL(clicked()), ctd, SLOT(testSelf()));
    connect(bridgeResistorBtn, SIGNAL(clicked()), ctd, SLOT(testBridgeResistor()));
    connect(insulationResistorBtn, SIGNAL(clicked()), ctd, SLOT(testInsulationResistor()));
    connect(temperatureBtn, SIGNAL(clicked()), ctd, SLOT(testTemperature()));
    connect(pressureBtn, SIGNAL(clicked()), ctd, SLOT(testPressure()));
    connect(voltageBtn, SIGNAL(clicked()), ctd, SLOT(testVoltage()));
    ctd->start();

    mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(15);
    mainLayout->setSpacing(10);
    mainLayout->addLayout(firstLayout);
    mainLayout->addLayout(secondLayout);
    setLayout(mainLayout);

}

void StatusInfo::chageStatus(QString str) {
   contentText->setText(str);
}
void StatusInfo::MesureTypeDisable()
{
    bridgeResistorBtn->setDisabled(true);
    insulationResistorBtn->setDisabled(true);
    temperatureBtn->setDisabled(true);
    voltageBtn->setDisabled(true);
    pressureBtn->setDisabled(true);
   selfTestBtn->setDisabled(true);
}
void StatusInfo::MesureTypeEnable()
{
    bridgeResistorBtn->setEnabled(true);
    insulationResistorBtn->setEnabled(true);
    temperatureBtn->setEnabled(true);
    voltageBtn->setEnabled(true);
    pressureBtn->setEnabled(true);
    selfTestBtn->setEnabled(true);
}
void StatusInfo::startDisable()
{
    start->setEnabled(false);
    stop->setEnabled(true);
    MesureTypeDisable();
}
void StatusInfo::stopDisable()
{
    stop->setEnabled(false);
    start->setEnabled(true);
    MesureTypeEnable();
}
StatusInfo::~StatusInfo(){
    delete bridgeResistorBtn;
    delete insulationResistorBtn;
    delete temperatureBtn;
    delete voltageBtn;
    delete pressureBtn;
    delete contentText;
    delete start;
    delete stop;
    delete firstLayout;
    delete secondLayout;
    delete mainLayout;

}
