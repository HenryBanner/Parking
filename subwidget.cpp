#include "subwidget.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTime>
#include <QDateTime>
#include <QCameraInfo>
#include <QBuffer>
#include <QFileDialog>
#include <QNetworkRequest>
#include <QFile>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHostAddress>
#include <QGridLayout>
#include <QCameraInfo>
#include <QBuffer>
#include <QMovie>
#include <QFont>
#include "http.h"
#include <mainwindow.h>

const QString baiduTokenUrl = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2&";

const char *client_id = "6HcQSEvPSlYnoGglZz2AtcLB";
const char *secret_id = "VIzzxFfwFNfDGNbKj7C3VHpIpGhkdsuV";
const QString baiduImageUrl = "https://aip.baidubce.com/rest/2.0/ocr/v1/license_plate?access_token=%1";
SubWidget::SubWidget(QWidget *parent) : QWidget(parent)
{
     //1. 构造所有需要的控件
     this->setWindowTitle("智能停车管理系统");
     this->setWindowState(Qt::WindowMaximized);//窗口大小
     this->setWindowFlags(Qt::FramelessWindowHint);//设置窗体无边框
     //this->setWindowOpacity(0.7);

     //设置子窗体背景
     QPalette palette;
     QPixmap pixmap(":/images/0.png");
     this->setAutoFillBackground(true);
     palette.setBrush(QPalette::Window, QBrush(pixmap));
     this->setPalette(palette);
     //设置按钮
     subButton1 = new QPushButton("停车");
     subButton2 = new QPushButton("取车");
     subButton3 = new QPushButton("查询");
     subButton4 = new QPushButton("返回");
     subButton5 = new QPushButton("退出");
     subButton6 = new QPushButton("发送");
     subButton7 = new QPushButton("缴费");
     subButton1->setFixedSize(175,70);
     subButton2->setFixedSize(175,70);
     subButton3->setFixedSize(175,70);
     subButton4->setFixedSize(175,70);
     subButton5->setFixedSize(175,70);
     subButton6->setFixedSize(175,70);
     subButton7->setFixedSize(175,70);
     subButton1->setIcon(QIcon(":/images/2.png"));
     subButton1->setIconSize(QSize(60,60));
     subButton2->setIcon(QIcon(":/images/3.png"));
     subButton2->setIconSize(QSize(60,60));
     subButton3->setIcon(QIcon(":/images/4.png"));
     subButton3->setIconSize(QSize(60,60));
     subButton4->setIcon(QIcon(":/images/5.png"));
     subButton4->setIconSize(QSize(60,60));
     subButton5->setIcon(QIcon(":/images/6.png"));
     subButton5->setIconSize(QSize(60,60));
     subButton6->setIcon(QIcon(":/images/8.png"));
     subButton6->setIconSize(QSize(60,60));
     subButton7->setIcon(QIcon(":/images/8.png"));
     subButton7->setIconSize(QSize(60,60));

     btn_regist = new QPushButton("注册");
     btn_regist->setFixedSize(175,70);
     btn_regist->setIcon(QIcon(":/images/3.png"));
     btn_regist->setIconSize(QSize(60,60));

     btn_decode = new QPushButton("识别");
     btn_decode->setFixedSize(175,70);
     btn_decode->setIcon(QIcon(":/images/7.png"));
     btn_decode->setIconSize(QSize(60,60));
     //设置label
     lb = new QLabel("智能停车管理系统");
     lb->setScaledContents(true);
     QFont font = lb->font();
     font.setFamily("KaiTi");//楷体
     lb->setFont(font);

     lb1 = new QLabel("第一版");
     lb1->setScaledContents(true);

     account = new registWindow(this);//注册窗口对象

     //二级窗口中的各个显示label组件
     QLabel *lb_gif = new QLabel();  //主页动图
     lb_gif->setScaledContents(true);

     lb_gif->setFixedSize(160,160);
     QMovie *m = new QMovie(":/images/3.gif");
     m->start();
     lb_gif->setMovie(m);

     lb2 = new QLabel;//时间
     lb2->setScaledContents(true);

     lb_pix = new QLabel;//公司图标
     lb_pix->setFixedSize(600,200);
     lb_pix->setScaledContents(true);
     lb_pix->setPixmap(QPixmap(":/images/1.png"));

     lb_0 = new QLabel;//空lb
     lb_0->setScaledContents(true);
     //显示的主窗口
     text1 = new QTextEdit;//显示屏
     text1->setFixedSize(800,630);
     text1->setReadOnly(true);


     //服务器套接字初始化以及连接
     mysock = new QTcpSocket;//套接字
     mysock->connectToHost(QHostAddress("192.168.6.152"), 8888);

     recvSever = new QTcpServer;
     recvSever->listen(QHostAddress::Any,6666);
     MainWindow *recv = new MainWindow;
     recv->init();
     connect(recvSever,&QTcpServer::newConnection,this,&SubWidget::onNewClientConnect);
     //connect(recvPicServer,&QTcpServer::newConnection,this,&SubWidget::NewPicConnect);
     //recvSever->listen(QHostAddress("192.168.6.153"), 6666);

     rfidIDList.push_back("a6ab6e87");
     rfidFlag = false;
     //设置输入框
     le_path = new QLineEdit;
     //le_path1 = new QLineEdit;
     le_path->setPlaceholderText("请输入车牌:");
     //le_path1->setPlaceholderText(":");
     le_path->setFixedSize(800,70);
     //le_path1->setFixedSize(700,70);

     //定时器（更新时间）
     QTimer *timer = new QTimer(this);
     connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
     timer->start(1000);



     //设置将会员模式置为非会员
     vipFlag = false;

     //显示车牌照片
     addFolderImages("D:/Qt/ParkingManagementSystem-main/QT/park/park/carCapture");
     vfinder = new QLabel(this);
     vfinder->setFixedSize(960, 540);
     vfinder->setMaximumSize(960, 540);
     vfinder->setGeometry(10, 10, 320, 240);
     //车牌识别http对象实例化
     httpmanager = new QNetworkAccessManager;
     connect(httpmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_ack(QNetworkReply*)));

     speeker = new QTextToSpeech;

     //2. 排版
     QHBoxLayout *box1 = new QHBoxLayout;
     box1->addWidget(lb_pix,0,Qt::AlignLeft | Qt::AlignTop);
     box1->addWidget(lb,0,Qt::AlignCenter | Qt::AlignCenter);
     box1->addWidget(lb2,0,Qt::AlignRight | Qt::AlignTop);

     QHBoxLayout *box2 = new QHBoxLayout;
     box2->addWidget(vfinder,0,Qt::AlignCenter | Qt::AlignTop);
     box2->addWidget(text1,0,Qt::AlignCenter | Qt::AlignCenter);

     QHBoxLayout *box3 = new QHBoxLayout;
     box3->addWidget(lb1,0,Qt::AlignLeft | Qt::AlignBottom);

     box3->addStretch();
     box3->setSpacing(30);
     box3->addWidget(lb_gif);
     box3->addWidget(btn_regist);
     box3->addWidget(btn_decode);
     box3->addWidget(subButton1);
     box3->addWidget(subButton2);
     box3->addWidget(subButton3);
     box3->addWidget(subButton7);
     box3->addWidget(subButton4);
     box3->addWidget(subButton5);

     QHBoxLayout *box4 = new QHBoxLayout;
     box4->addWidget(le_path,0,Qt::AlignCenter | Qt::AlignTop);
     box4->addWidget(subButton6,0,Qt::AlignLeft | Qt::AlignTop);
     box4->addWidget(lb_0,0,Qt::AlignCenter | Qt::AlignTop);

     QVBoxLayout *mainbox = new QVBoxLayout;
     mainbox->addLayout(box1);
     mainbox->addLayout(box2);
     mainbox->addLayout(box4);
     mainbox->addLayout(box3);
     setLayout(mainbox);

     //3.前后台挂接
     connect(btn_decode, SIGNAL(clicked(bool)), this, SLOT(camCaptured()));
     connect(btn_regist, SIGNAL(clicked(bool)), this, SLOT(regist_account()));

     connect(subButton1, SIGNAL(clicked(bool)), this, SLOT(send_1()));
     connect(subButton2, SIGNAL(clicked(bool)), this, SLOT(send_2()));
     connect(subButton3, SIGNAL(clicked(bool)), this, SLOT(send_3()));
     connect(subButton4, SIGNAL(clicked(bool)), this, SLOT(send_4()));
     connect(subButton7, SIGNAL(clicked(bool)), this, SLOT(send_7()));
     connect(subButton5,SIGNAL(clicked()),this,SLOT(changeSlot()));
     //connect(&payWin,SIGNAL(mySignal()),this,SLOT(tosubSlot()));
     connect(subButton7,SIGNAL(clicked()),this,SLOT(topaySlot()));
     connect(subButton6, SIGNAL(clicked(bool)), this, SLOT(send_data()));
     connect(mysock, SIGNAL(readyRead()), this, SLOT(recv_data()));
     connect(this,&SubWidget::vipSignal,&payWin,&payWidget::on_recvSlot);
     connect(account,&registWindow::setvipSignal,this,&SubWidget::on_setVipSlot);
     //connect(recvSever,&QTcpServer::newConnection,this,&SubWidget::onNewClientConnect);
     //4. 美化
     lb->setStyleSheet("color:white;font-weight:bold;font-size:50px;");
     lb1->setStyleSheet("color:white;font-weight:bold;font-size:40px;");
     lb2->setStyleSheet("color:white;font-weight:bold;font-size:40px;");
     text1->setStyleSheet("background:rgba(46, 134, 222,0.6);color:white;font-weight:bold;font-size:30px;border-radius: 50px;");
     le_path->setStyleSheet("background:rgba(45, 152, 218,0.8);color:white;font-weight:bold;font-size:30px;border-radius: 20px;");
     //le_path1->setStyleSheet("background:rgba(45, 152, 218,0.8);color:white;font-weight:bold;font-size:30px;border-radius: 20px;");

     subButton1->setStyleSheet("QPushButton{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:30px;}"
                               "QPushButton:hover{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;}"
                               "QPushButton:pressed{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;"
                               "padding-left:4px;"
                               "padding-top:4px;}");
     subButton2->setStyleSheet("QPushButton{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:30px;}"
                               "QPushButton:hover{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;}"
                               "QPushButton:pressed{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;"
                               "padding-left:4px;"
                               "padding-top:4px;}");
     subButton3->setStyleSheet("QPushButton{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:30px;}"
                               "QPushButton:hover{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;}"
                               "QPushButton:pressed{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;"
                               "padding-left:4px;"
                               "padding-top:4px;}");
     subButton4->setStyleSheet("QPushButton{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:30px;}"
                               "QPushButton:hover{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;}"
                               "QPushButton:pressed{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;"
                               "padding-left:4px;"
                               "padding-top:4px;}");
     subButton5->setStyleSheet("QPushButton{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:30px;}"
                               "QPushButton:hover{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;}"
                               "QPushButton:pressed{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;"
                               "padding-left:4px;"
                               "padding-top:4px;}");
     subButton6->setStyleSheet("QPushButton{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:30px;}"
                               "QPushButton:hover{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;}"
                               "QPushButton:pressed{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;"
                               "padding-left:4px;"
                               "padding-top:4px;}");
     subButton7->setStyleSheet("QPushButton{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:30px;}"
                               "QPushButton:hover{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;}"
                               "QPushButton:pressed{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;"
                               "padding-left:4px;"
                               "padding-top:4px;}");
     btn_decode->setStyleSheet("QPushButton{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:30px;}"
                               "QPushButton:hover{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;}"
                               "QPushButton:pressed{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;"
                               "padding-left:4px;"
                               "padding-top:4px;}");
     btn_regist->setStyleSheet("QPushButton{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:30px;}"
                               "QPushButton:hover{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;}"
                               "QPushButton:pressed{"
                               "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop: 0.3 #96e0f6, stop:1 #02154f);"
                               "border-radius: 35px;"
                               "color: white;"
                               "font-weight:bold;"
                               "font-size:33px;"
                               "padding-left:4px;"
                               "padding-top:4px;}");


 }
void SubWidget::addFolderImages(QString path)
{
    //判断路径是否存在
    QDir dir(path);
    if(!dir.exists())
    {
        return;
    }
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    QStringList removeList;
    int file_count = list.count();

    if(file_count <= 0)
    {
        return;
    }
    for(int i=0; i<file_count; i++)

    {

        QFileInfo file_info = list.at(i);

        QString suffix = file_info.suffix();

        if(QString::compare(suffix, QString("png"), Qt::CaseInsensitive) == 0)

        {

            QString absolute_file_path = file_info.absoluteFilePath();

            string_list.append(absolute_file_path);

            for(int i = 0; i < string_list.length(); i++){
               if(!removeList.contains(string_list[i])){
                  removeList.append(string_list[i]);
               }
            }
            string_list = removeList;

        }

    }

}

void SubWidget::onNewClientConnect()
{

    tcpClient = recvSever->nextPendingConnection();

    //后面跟客户端通信 都是使用 这个连接上来的 tcpClient对象
    listClient.append(tcpClient);//将生成的socket添加到容器里
    m_iClientCount++;
    qDebug() << __LINE__;
    //当有客户端发送数据的时候，tcpClient会触发readyRead信号，关联槽函数，接收数据
    connect(tcpClient,&QTcpSocket::readyRead,this,&SubWidget::onRecvDataSlot);
}

void SubWidget::onRecvDataSlot()
{
    //qDebug() << __LINE__;

    recvData = tcpClient->readAll();
    qDebug() << recvData;
    //rfidIDList.push_back(recvData);
    this->rfID = recvData;
    //qDebug() << rfidIDList.at(0) << this->rfID;
    if( rfidIDList.contains(this->rfID)&& rfidFlag == false)
    {
        rfidIDList.push_back(recvData);
        rfID = recvData;
        mysock->write("in");
        rfidFlag = true;
    }



}
void SubWidget::camCaptured()
{
//    QByteArray buf;
//    QFile file("C:/Users/11590/Desktop/savePic1.png");
//    if(file.open(QIODevice::ReadOnly))
//    {
//        buf = file.readAll();
//    }
//    QBuffer buffer(&buf);
//    buffer.open(QIODevice::WriteOnly);
//    pix.save(&buffer,"png");
    //qDebug() << string_list.at(0);
    fileName = string_list.at(0);
    //2.转成base64及urlcode
    QByteArray buf64 = Image::imageToBase64(fileName);
    QByteArray body = "image=" + buf64; //body
//    QByteArray buf64 = buf.toBase64().toPercentEncoding();
//    QByteArray body = "image="+buf64;
    //发送完整的一次识别请求
    QByteArray replyData; //保存回复信息
    QString url = QString(baiduTokenUrl).arg(client_id).arg(secret_id);

    QMap<QString,QString> header; //封装头部信息
    header.insert(QString("Content-Type"),QString("application/x-www-form-urlencoded"));


    bool result = Http::post_syns(url,header,body,replyData);
    if(result)
    {
        QJsonObject obj = QJsonDocument::fromJson(replyData).object();

        accessToken = obj.value("access_token").toString();
    }
    replyData.clear();
    QString imgurl = baiduImageUrl.arg(accessToken);
    result = Http::post_syns(imgurl,header,body,replyData);
    if(result)
    {
        QJsonObject obj = QJsonDocument::fromJson(replyData).object();
        QJsonValue val = obj.value("words_result");
        QString carId = val.toObject().value("number").toString();
        le_path->setText(carId);
        speeker->say("您好"+carId);
    }
}



void SubWidget::regist_account()
{

    QFile file(":/res/qss/style-1.qss");/*QSS文件所在的路径*/

    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    account->setStyleSheet(stylesheet);
    file.close();
    account->setAttribute(Qt::WA_DeleteOnClose);
    account->show();
    this->hide();
}
//结果解析
void SubWidget::read_ack(QNetworkReply* r)
{
    //提取json格式的返回
    QByteArray buf = r->readAll();
    qDebug() << buf;

    //提取json中的感兴趣的内容
    QJsonDocument myjson = QJsonDocument::fromJson(buf);
    QJsonObject node = myjson.object();
    QJsonObject arr = node.value("words_result").toObject();
    QString text = arr.value("number").toString();
     qDebug() << arr;
    qDebug() << text;
    le_path->setText(text);
    speeker->say("您好"+text);
}

void SubWidget::on_setVipSlot(bool vipFlag)
{
    this->vipFlag = vipFlag;
}



void SubWidget::recv_data() //接受到显示屏
{
//    QMessageBox message(QMessageBox::Information,"Tip","<font size='5'>Send success!</font>");
//    message.exec();
    QByteArray buf = mysock->readAll();
    text1->append(buf);
}

void SubWidget::send_data()  //发送
{
     qDebug()<<"send: "<<le_path->text();
     mysock->write(le_path->text().toStdString().c_str());
     le_path->clear();
}

void SubWidget::timerUpdate(void)  //时间
{
    QDateTime time = QDateTime::currentDateTime();

    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    lb2->setText(str);
    timeUpDate = time.toString("hh:mm:ss");
    timerCount();
    //qDebug() << string_list.at(string_list.size()-1);
    fileName = string_list.at(string_list.size()-1);
    QPixmap pic(fileName);
    pic.scaled(vfinder->size());
    vfinder->setPixmap(pic);
}
void SubWidget::timerCount()
{
    QString speakText;
    QStringList tt = timeUpDate.split(":");
    if(tt.at(1) == "59" && tt.at(2) == "59")
    {
        int countTime = tt.at(1).toInt();
        speakText = QString("现在是北京时间%1点整").arg(countTime+1);
        speeker->say(speakText);
    }
    else if(tt.at(1) == "29" && tt.at(2) == "59")
    {
        int countHalfTime = tt.at(1).toInt();
        speakText = QString("现在是北京时间%1点30分").arg(countHalfTime+1);
        speeker->say(speakText);
    }
}

void SubWidget::changeSlot() //关闭窗口
{

        QPushButton *okbtn = new QPushButton(QString::fromLocal8Bit("Ok"));
        QPushButton *cancelbtn = new QPushButton(QString::fromLocal8Bit("Cancel"));
        QMessageBox *mymsgbox = new QMessageBox;

        mymsgbox->setIcon(QMessageBox::Warning);
        mymsgbox->setWindowTitle(QString::fromLocal8Bit("Tip"));
        mymsgbox->setText(QString("确定退出停车场吗?"));
        mymsgbox->addButton(okbtn, QMessageBox::AcceptRole);
        mymsgbox->addButton(cancelbtn, QMessageBox::RejectRole);
        mymsgbox->show();

        mymsgbox->exec();//阻塞等待用户输入
        if (mymsgbox->clickedButton()==okbtn)//点击了OK按钮
        {
            speeker->say("欢迎下次使用本系统，祝您生活愉快");
            account->close();
            this->close();QWidget:close();
        }
        else{

        }

}

void SubWidget::send_1()  //停车按钮发送命令
{

     qDebug()<<"send: 停车";
     mysock->write("1");
     //subButton1->setEnabled(false);
}

void SubWidget::send_2()  //取车按钮发送命令
{

     qDebug()<<"send: 取车";

     if(rfidIDList.contains(this->rfID) && rfidFlag == true)
     {
         mysock->write("out");
         rfidIDList.pop_back();
     }
     else
     {
         mysock->write("2");
     }
     //subButton2->setEnabled(false);
}

void SubWidget::send_3()  //查询按钮发送命令
{

     qDebug()<<"send: 查询";
     mysock->write("show");
     //subButton3->setEnabled(false);
}

void SubWidget::send_4()  //返回按钮发送命令
{
     subButton1->setEnabled(true);
     subButton2->setEnabled(true);
     subButton3->setEnabled(true);


     qDebug()<<"send: 返回";
     mysock->write("0");

}

void SubWidget::send_7()  //缴费按钮发送命令
{
    if(vipFlag == true)
    {
        emit vipSignal(true);
        qDebug()<<"send: 会员缴费";
        mysock->write("6");
    }
    else
    {
        qDebug()<<"send: 非会员缴费";
        mysock->write("6");
    }

}



void SubWidget::topaySlot()
{
    //this->hide();
    if(vipFlag == true)
    {
        emit vipSignal(true);
        payWin.show();
    }else{
        payWin.show();
    }
}

