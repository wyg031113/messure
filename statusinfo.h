#ifndef STATUSINFO
#define STATUSINFO
#include<QTextBrowser>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include <QPushButton>
#include<collectionthread.h>
#include <QRadioButton>
class StatusInfo : public QWidget {
    Q_OBJECT
public:
    explicit StatusInfo(QWidget *parent=0);
    ~StatusInfo();
private:
    QRadioButton *selfTestBtn;
    QRadioButton *bridgeResistorBtn;
    QRadioButton *insulationResistorBtn;
    QRadioButton *temperatureBtn;
    QRadioButton *voltageBtn;
    QRadioButton *pressureBtn;
    QPushButton *start;
    QPushButton *stop;
    QTextBrowser *contentText;
    QHBoxLayout *firstLayout;
    QHBoxLayout *secondLayout;
    QVBoxLayout *mainLayout;
    static CollectionThread *collectionThread;
    void MesureTypeDisable();
    void MesureTypeEnable();
public slots:
    void chageStatus(QString);
    void startDisable();
    void stopDisable();
};

#endif // STATUSINFO

