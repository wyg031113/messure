#include "recordinfo.h"
#include<QFont>
#include<QCalendarWidget>
#include<QMessageBox>
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
    CircumstanceEdit->setFont(f);
    HumidityLable = new QLabel(tr("相对湿度:"));
    HumidityLable->setFont(f);
    HumidityEdit = new QLineEdit;
    HumidityEdit->setFont(f);

    DateLabel = new QLabel(tr("测试日期:"));
    DateLabel->setFont(f);
    DateInput = new QHBoxLayout;
    QLineEdit *year = new QLineEdit;
    year->setFont(f);
    year->setText("2017");
    year->setFixedWidth(70);
    QLabel *year1 = new QLabel(tr("年"));
    year1->setFont(f);
    QLineEdit *month = new QLineEdit;
    month->setFont(f);
    month->setText("12");
    month->setFixedWidth(40);
    QLabel *month1 = new QLabel(tr("月"));
    month1->setFont(f);
    QLineEdit *day = new QLineEdit;
    day->setFont(f);
    day->setText("30");
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
    QLineEdit *hour = new QLineEdit;
    hour->setFont(f);
    hour->setText("12");
    hour->setFixedWidth(90);
    QLabel *hour1 = new QLabel(tr("时"));
    hour1->setFont(f);
    QLineEdit *minute = new QLineEdit;
    minute->setFont(f);
    minute->setText("13");
    minute->setFixedWidth(90);
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
    save->setFixedSize(70, 35);
    mainLayout = new QGridLayout(this);
    mainLayout->setMargin(15);
    mainLayout->setSpacing(12);
    mainLayout->addWidget(SINLabel, 0, 0);
    mainLayout->addWidget(SINEdit, 0, 1);
    mainLayout->addWidget(EINLabel, 1, 0);
    mainLayout->addWidget(EINEdit, 1, 1);
    mainLayout->addWidget(TesterLabel, 2, 0);
    mainLayout->addWidget(TesterEdit, 2, 1);
    mainLayout->addWidget(CircumstanceLabel, 3, 0);
    mainLayout->addWidget(CircumstanceEdit, 3, 1);
    mainLayout->addWidget(HumidityLable, 4, 0);
    mainLayout->addWidget(HumidityEdit, 4, 1);
    mainLayout->addWidget(DateLabel, 5, 0);
    mainLayout->addLayout(DateInput, 5, 1);
    mainLayout->addWidget(TimeLabel, 6, 0);
    mainLayout->addLayout(TimeInput, 6, 1);
    mainLayout->addWidget(save, 6, 2);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    connect(save, SIGNAL(clicked()), this, SLOT(save2()));
}

void RecordInfo::save2() {
    QMessageBox::information(0, "文件保存", "文件保存成功!");
}

RecordInfo::~RecordInfo() {
    delete EINLabel;
    delete EINEdit;
    delete TesterLabel;
    delete TesterEdit;
    delete CircumstanceLabel;
    delete CircumstanceEdit;
    delete HumidityLable;
    delete HumidityEdit;
    delete TimeLabel;
    delete TimeInput;
    delete DateInput;
    delete DateLabel;
    delete save;
    delete mainLayout;
}


