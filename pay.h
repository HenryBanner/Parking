#ifndef PAY_H
#define PAY_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTcpSocket>
#include <QHostAddress>

class payWidget : public QWidget
{
    Q_OBJECT

public:
    payWidget(QWidget *parent = 0);


    QLabel *lb;
    QLabel *info;
    QString showInfo;
    QString isVip;
    QTcpSocket *mysock;
    bool vipFlag;


public:

    QPushButton * mainButton;

public slots:
    void changeSlot();
    void on_recvSlot(bool vipFlag);
    //void send();



};

#endif // PAY_H
