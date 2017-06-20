#ifndef RECORDINFO_H
#define RECORDINFO_H

#include<QLabel>
#include<QLineEdit>
#include<QGridLayout>
//#include<QDateTimeEdit>
#include<QTimeEdit>
#include <QHBoxLayout>
#include<QPushButton>
class RecordInfo : public QWidget{

    Q_OBJECT

public:
    explicit RecordInfo(QWidget *parent=0);
    ~RecordInfo();
private:
    QLabel *EINLabel;
    QLineEdit *EINEdit;
    QLabel *TesterLabel;
    QLineEdit *TesterEdit;
    QLabel *CircumstanceLabel;
    QLineEdit *CircumstanceEdit;
    QLabel *HumidityLable;
    QLineEdit *HumidityEdit;
    QLabel *DateLabel;
    QLabel *TimeLabel;
    QHBoxLayout *DateInput;
    QHBoxLayout *TimeInput;
    QLabel *SINLabel;
    QLineEdit *SINEdit;
    QPushButton *save;
    QGridLayout *mainLayout;

public slots:
    void save2();
};

#endif // RECORDINFO_H
