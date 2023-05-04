#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include <QDebug>
#include <QBuffer>
#include <QImageReader>
#include <QImage>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //ui->setupUi(this);

}

 MainWindow::~MainWindow()
 {

 }

 void MainWindow::init()
 {
     totalBytes = 0;
     bytesReceived = 0;
     fileNameSize = 0;
     totalBytes = 0;
     bytesReceived = 0;
     fileNameSize = 0;
     tcpServer = new QTcpServer(this);
     if(!tcpServer->listen(QHostAddress::Any,7777))
     {  //**本地主机的6666端口，如果出错就输出错误信息，并关闭
         qDebug() << tcpServer->errorString();
         close();
     }
     //连接信号和相应槽函数,有新的连接进入是需处理
     connect(tcpServer,SIGNAL(newConnection()),this,SLOT(NewConnection()));
 }
void MainWindow::NewConnection()
{

    //新连接进入的显示处理
    currentClient = tcpServer->nextPendingConnection();
    //ui->label_2->setText
    qDebug() << (tr("%1:%2").arg(currentClient->peerAddress().toString().split("::ffff:")[1])\
            .arg(currentClient->peerPort()));
    connect(currentClient, SIGNAL(readyRead()), this, SLOT(recMessage()));
    connect(currentClient, SIGNAL(disconnected()), this, SLOT(disconnect()));

}

void MainWindow::recMessage()
{
    QDataStream in(currentClient);
    in.setVersion(QDataStream::Qt_5_8);
    if(bytesReceived <= sizeof(qint64)*2)
    { //如果接收到的数据小于16个字节，那么是刚开始接收数据，我们保存到//来的头文件信息

        if((currentClient->bytesAvailable() >= sizeof(qint64)*2)

                && (fileNameSize == 0))

        { //接收数据总大小信息和文件名大小信息

            in >> totalBytes >> fileNameSize;

            bytesReceived += sizeof(qint64) * 2;

        }

        if((currentClient->bytesAvailable() >= fileNameSize)

                && (fileNameSize != 0))

        {  //接收文件名，并建立文件

            in >> fileName;

            qDebug() <<(tr("接收文件 %1 ...").arg(fileName));

            bytesReceived += fileNameSize;
            qDebug() <<fileName;
            localFile= new QFile(fileName);
            if(!localFile->open(QFile::WriteOnly))
            {
                qDebug() << "open file error!";
                return;
            }
        }
        else return;
    }
    if(bytesReceived < totalBytes)
    {  //如果接收的数据小于总数据，那么写入文件
        bytesReceived += currentClient->bytesAvailable();
        inBlock+= currentClient->readAll();
    }
    //更新进度条
    //ui->progressBar->setMaximum(totalBytes);
    //ui->progressBar->setValue(bytesReceived);
    if(bytesReceived == totalBytes)
    { //接收数据完成时
        //接收显示
        QBuffer buffer(&inBlock);
        buffer.open(QIODevice::ReadOnly);
        QImageReader reader(&buffer,"png");
        QImage image = reader.read();
        if(!image.isNull())
        {
            image.save("../park/carCapture/1.png");
        }
        localFile->write(inBlock);
        localFile->close();
        inBlock.resize(0);
        //重新置0 准备下次接收
        totalBytes = 0;
        bytesReceived = 0;
        fileNameSize = 0;
        qDebug() <<(tr("接收文件 %1 成功！").arg(fileName));
    }
}

void MainWindow::disconnect()
{
    qDebug()<<"disconnect";
}

