#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include <QLabel>
#include <QWidget>

class ClickedLabel : public QLabel {
    Q_OBJECT

signals:
    void clicked();
public:
    ClickedLabel(QWidget *parent=0);
protected:
     void mouseReleaseEvent(QMouseEvent*);
};

#endif //CLICKEDLABEL_H
