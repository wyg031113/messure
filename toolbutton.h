#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QWidget>

class toolButton : public QWidget
{
    Q_OBJECT
public:
    explicit toolButton(QString picName,QString text,int _id, QWidget *parent = 0);
    
signals:
    void myToolClicked(int);

public slots:
    
private:
    void setBkPalette(int);
    int id;
protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void leaveEvent(QEvent *);
};

#endif // TOOLBUTTON_H


