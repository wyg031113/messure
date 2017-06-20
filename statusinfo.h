#ifndef STATUSINFO
#define STATUSINFO
#include<QTextBrowser>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include <QPushButton>
#include<collectionthread.h>
class StatusInfo : public QWidget {
    Q_OBJECT
public:
    explicit StatusInfo(QWidget *parent=0);
    ~StatusInfo();
private:
    QPushButton *bridgeResistorBtn;
    QPushButton *insulationResistorBtn;
    QPushButton *temperatureBtn;
    QPushButton *voltageBtn;
    QPushButton *pressureBtn;
    QPushButton *start;
    QPushButton *stop;
    QTextBrowser *contentText;
    QHBoxLayout *firstLayout;
    QHBoxLayout *secondLayout;
    QVBoxLayout *mainLayout;
    static CollectionThread *collectionThread;
public slots:
    void chageStatus(QString);
    void startDisable();
    void stopDisable();
};

#endif // STATUSINFO

