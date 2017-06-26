#ifndef RECORDINFO_H
#define RECORDINFO_H

#include<QLabel>
#include<QLineEdit>
#include<QGridLayout>
//#include<QDateTimeEdit>
#include<QTimeEdit>
#include <QHBoxLayout>
#include<QPushButton>
#include <QLineEdit>
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
    QPushButton *refresh_btn;
    QGridLayout *mainLayout;

      QLineEdit *year;
      QLineEdit *month;
      QLineEdit *day;
      QLineEdit *hour;
      QLineEdit *minute;
public slots:
    void save2();
    void refresh_time();
};

#endif // RECORDINFO_H
