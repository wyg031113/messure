#include "recordinfo.h"
#include<QFont>
#include<QCalendarWidget>
#include<QMessageBox>
#include <QTime>
#include <QDate>
#include <fstream>
#include "uiutils.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <iostream>
#include <QDateTime>
#include <QTextCodec>
#include <QDir>
using namespace std;
 void RecordInfo::refresh_time()
 {
    QTime now = QTime::currentTime();
    QDate date = QDate::currentDate();
    year->setText(QString::number(date.year()));
    month->setText(QString::number(date.month()));
    day->setText(QString::number(date.day()));
    hour->setText(QString::number(now.hour()));
    minute->setText(QString::number(now.minute()));
 }

RecordInfo::RecordInfo(QWidget *parent):
    QWidget(parent) {
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::black);
    this->setPalette(pe);

    //设置字体大小
    QFont f;
    f.setPointSize(18);
    EINLabel = new QLabel(tr("发动机编号:"));
    EINLabel->setFont(f);
    EINEdit = new QLineEdit;
    EINEdit->setFont(f);
    TesterLabel = new QLabel(tr("测试人员:"));
    TesterLabel->setFont(f);
    TesterEdit = new QLineEdit;
    TesterEdit->setFont(f);

    CircumstanceLabel = new QLabel(tr("环境温度:"));
    CircumstanceLabel->setFont(f);
    CircumstanceEdit = new QLineEdit;
    CircumstanceEdit->setFixedSize(80, 35);
    CircumstanceEdit->setFont(f);
    HumidityLabel = new QLabel(tr("相对湿度:"));
    HumidityLabel->setFont(f);
    HumidityEdit = new QLineEdit;
    HumidityEdit->setFont(f);
    HumidityEdit->setFixedSize(80, 35);

    QHBoxLayout *temp_hum_layout1 = new QHBoxLayout;
    QHBoxLayout *temp_hum_layout2 = new QHBoxLayout;
    temp_hum_layout1->addWidget(CircumstanceLabel);
    temp_hum_layout2->addWidget(CircumstanceEdit);
    temp_hum_layout2->addWidget(HumidityLabel);
    temp_hum_layout2->addWidget(HumidityEdit);

    pkLabel = new QLabel(tr("压力参数pk:"));
    pkLabel->setFont(f);
    pkEdit = new QLineEdit;
    pkEdit->setFixedSize(80, 35);
    pkEdit->setFont(f);
    pbLabel = new QLabel(tr("压力参数pb:"));
    pbLabel->setFont(f);
    pbEdit = new QLineEdit;
    pbEdit->setFont(f);
    pbEdit->setFixedSize(80, 35);

    QHBoxLayout *pkb_layout1 = new QHBoxLayout;
    QHBoxLayout *pkb_layout2 = new QHBoxLayout;
    pkb_layout1->addWidget(pkLabel);
    pkb_layout2->addWidget(pkEdit);
    pkb_layout2->addWidget(pbLabel);
    pkb_layout2->addWidget(pbEdit);


    DateLabel = new QLabel(tr("测试日期:"));
    DateLabel->setFont(f);
    DateInput = new QHBoxLayout;
    year = new QLineEdit;
    year->setFont(f);
    year->setFixedWidth(80);
    QLabel *year1 = new QLabel(tr("年"));
    year1->setFont(f);

    month = new QLineEdit;
    month->setFont(f);
    month->setFixedWidth(40);
    QLabel *month1 = new QLabel(tr("月"));
    month1->setFont(f);
    day = new QLineEdit;
    day->setFont(f);
    day->setFixedWidth(40);
    QLabel *day1 = new QLabel(tr("日"));
    day1->setFont(f);

    DateInput->addWidget(year);
    DateInput->addWidget(year1);
    DateInput->addWidget(month);
    DateInput->addWidget(month1);
    DateInput->addWidget(day);
    DateInput->addWidget(day1);

    TimeLabel = new QLabel(tr("测试时间:"));
    TimeLabel->setFont(f);
    TimeInput = new QHBoxLayout;
    hour = new QLineEdit;
    hour->setFont(f);

    hour->setFixedWidth(80);
    QLabel *hour1 = new QLabel(tr("时"));
    hour1->setFont(f);
    minute = new QLineEdit;
    minute->setFont(f);

    minute->setFixedWidth(80);
    QLabel *minute1 = new QLabel(tr("分"));
    minute1->setFont(f);
    TimeInput->addWidget(hour);
    TimeInput->addWidget(hour1);
    TimeInput->addWidget(minute);
    TimeInput->addWidget(minute1);
    SINLabel = new QLabel(tr("SIN:"));
    SINLabel->setFont(f);
    SINEdit = new QLineEdit;
    SINEdit->setFont(f);
    save = new QPushButton(tr("保存"));
    save->setFixedSize(80, 35);

    refresh_btn = new QPushButton(tr("刷新时间"));
    refresh_btn->setFixedSize(70, 35);

    apply_btn = new QPushButton(tr("应用"));;
    load_btn = new QPushButton(tr("读取"));;

    mainLayout = new QGridLayout(this);
    mainLayout->setMargin(15);
    mainLayout->setSpacing(12);
    mainLayout->addWidget(SINLabel, 0, 0);
    mainLayout->addWidget(SINEdit, 0, 1);
    mainLayout->addWidget(EINLabel, 1, 0);
    mainLayout->addWidget(EINEdit, 1, 1);
    mainLayout->addWidget(TesterLabel, 2, 0);
    mainLayout->addWidget(TesterEdit, 2, 1);
    mainLayout->addLayout(temp_hum_layout1, 3, 0);
    mainLayout->addLayout(temp_hum_layout2, 3, 1);
    mainLayout->addLayout(pkb_layout1, 4, 0);
    mainLayout->addLayout(pkb_layout2, 4, 1);
    mainLayout->addWidget(apply_btn, 4, 2);
    mainLayout->addWidget(load_btn, 4, 3);
    mainLayout->addWidget(DateLabel, 5, 0);
    mainLayout->addLayout(DateInput, 5, 1);
    mainLayout->addWidget(TimeLabel, 6, 0);
    mainLayout->addLayout(TimeInput, 6, 1);
    mainLayout->addWidget(save, 6, 2);
    mainLayout->addWidget(refresh_btn, 6, 3);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    refresh_time();
    connect(save, SIGNAL(clicked()), this, SLOT(save2()));
    connect(refresh_btn, SIGNAL(clicked()), this, SLOT(refresh_time()));
    connect(apply_btn, SIGNAL(clicked()), this, SLOT(apply()));
    connect(load_btn, SIGNAL(clicked()), this, SLOT(load()));
}

void RecordInfo::apply()
{
    qDebug()<<"Apply pk-pb"<<pkEdit->text()<<" "<<pbEdit->text();
    UiUtils::messure_data.pk = pkEdit->text().toDouble();
    UiUtils::messure_data.pb = pbEdit->text().toDouble();
}

void RecordInfo::load()
{
    qDebug()<<"Load pk pb";
}

void RecordInfo::save2() {

    QDateTime qdt = QDateTime::currentDateTime();
    QDir qdata(UiUtils::data_dir);
    qdata.mkdir(UiUtils::data_dir);
    QFile f(UiUtils::data_dir+QString("data")+qdt.toString("yyyyMMdd_hhmmss")+QString(".txt"));
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))
    {
        qDebug() << "Open failed." << endl;
        QMessageBox::critical(0, "失败", "文件保存失败!");
        return;
    }
    QTextStream txtOutput(&f);
    txtOutput.setCodec(QTextCodec::codecForName("utf8"));
    txtOutput<<year->text()<<" "<<month->text()<<" "<<day->text()<<" "
                 <<hour->text()<<" "<<minute->text()<<endl;
    txtOutput<<SINLabel->text()<<" " << SINEdit->text()<<endl;
    txtOutput<<EINLabel->text()<<" " << EINEdit->text()<<endl;
    txtOutput<<TesterLabel->text()<<" "<<TesterEdit->text()<<endl;
    txtOutput<<CircumstanceLabel->text()<<" "<<CircumstanceEdit->text()<<endl;
    txtOutput<<HumidityLabel->text()<<" "<<HumidityEdit->text()<<endl;
    txtOutput<< QString("自检电压: ") << UiUtils::double2string(UiUtils::messure_data.self_test_voltage) << endl
             << QString("输入阻抗: ") << UiUtils::double2string(UiUtils::messure_data.in_resis) << endl
             << QString("输出阻抗: ") << UiUtils::double2string(UiUtils::messure_data.out_resis) << endl
             << QString("零点电压: ") << UiUtils::double2string(UiUtils::messure_data.zero_voltage) << endl
             << QString("绝缘电阻: ") << UiUtils::double2string(UiUtils::messure_data.ins_resis) << endl
             << QString("压力: ") << UiUtils::double2string(UiUtils::messure_data.pressure) << endl
             << QString("负10摄氏度下的压力: ") << UiUtils::double2string(UiUtils::messure_data.pressure_tm10) << endl
             << QString("压力参数pk: ") << UiUtils::double2string(UiUtils::messure_data.pk) << endl
             << QString("压力参数pb: ") << UiUtils::double2string(UiUtils::messure_data.pb)  << endl;

    txtOutput<<endl;

    f.close();

    QMessageBox::information(0, "文件保存", "文件保存成功!");
}

RecordInfo::~RecordInfo() {
    delete EINLabel;
    delete EINEdit;
    delete TesterLabel;
    delete TesterEdit;
    delete CircumstanceLabel;
    delete CircumstanceEdit;
    delete HumidityLabel;
    delete HumidityEdit;
    delete TimeLabel;
    delete TimeInput;
    delete DateInput;
    delete DateLabel;
    delete save;
    delete mainLayout;
    delete pkLabel;
    delete pbLabel;
    delete pkEdit;
    delete pbEdit;
    delete year;
    delete month;
    delete day;
    delete hour;
    delete minute;
    delete apply_btn;
    delete load_btn;
    qDebug()<<"delete delete delete .....";
}


