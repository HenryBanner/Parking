#ifndef SUBWIDGET_H
#define SUBWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QTextEdit>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QTextToSpeech>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>
#include <QFile>
#include "pay.h"
#include "http.h"
#include "image.h"
#include "registwindow.h"

class SubWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SubWidget(QWidget *parent = 0);
    QLabel *lb;
    QLabel *lb1;
    QLabel *lb2;
    QLabel *lb3;
    QLabel *lb_icon;
    QLabel *lb_pix;
    QLabel *lb_gif;
    QLabel *lb_0;
    QTextEdit *text1;
    QLineEdit *le_path;
    //QLineEdit *le_path1;
    QNetworkAccessManager *httpmanager;
    QCamera *camera;
    QLabel *vfinder;
    QCameraImageCapture *imageCapture;
    QTextToSpeech *speeker;
    QTcpSocket *mysock;
    QTcpServer *recvSever;
    //QTcpServer *recvPicServer;
    //QTcpSocket *picSocket;
    QTcpSocket *tcpClient;
    QList<QTcpSocket*> listClient;
    int m_iClientCount;

//    qint64 totalBytes;     // 存放总大小信息
//    qint64 bytesReceived;  // 已收到数据的大小
//    qint64 fileNameSize;   // 文件名的大小信息
//    qint64 imageSize; //图片大小

    QString fileName;      // 存放文件名
    QFile *localFile;      // 本地文件
    QByteArray inBlock;    // 数据缓冲区
    QString imageContent;
    bool vipFlag;
    QImage image;//图片



public slots://申明下面的函数可被信号激发（槽函数）
        void timerUpdate(void);
        //QImage getImage(const QString &);
        void camCaptured();
        void timerCount();
        //void decode_pix();
        void regist_account();
        void read_ack(QNetworkReply*);
        void on_setVipSlot(bool vipFlag);
        //void NewPicConnect();
        //void recvPic();
        void addFolderImages(QString);
        void onNewClientConnect();
        void onRecvDataSlot();
        void changeSlot();
        void recv_data();
        void send_data();
        void send_1();
        void send_2();
        void send_3();
        void send_4();
        void send_7();
//protected:
    //void mouseDoubleClickEvent(QMouseEvent *event); //鼠标双击事件
signals:
        void vipSignal(bool vipFlag);

private:
    QPushButton * subButton1;
    QPushButton * subButton2;
    QPushButton * subButton3;
    QPushButton * subButton4;
    QPushButton * subButton5;
    QPushButton * subButton6;
    QPushButton * subButton7;
    QPushButton * btn_decode;
    QPushButton * btn_regist;
    registWindow *account;
    QStringList string_list;
    QString recvData;
    payWidget payWin;
    QString accessToken;
    QString rfID;
    bool rfidFlag;
    QStringList rfidIDList;
    QString timeUpDate;
private slots:
    void topaySlot();
   // void tosubSlot();

};

#endif // SUBWIDGET_H
