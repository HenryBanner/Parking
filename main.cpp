#include "mainwidget.h"
#include <QApplication>
#include <QIcon>
#include <QSplashScreen>
#include <QElapsedTimer>
#include <QMovie>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pic(":/res/pic/open.gif");
    QMovie mv(":/res/pic/open.gif");
    QSplashScreen screen(pic);
    QLabel label(&screen);
    label.setMovie(&mv);
    //label.setText("程序正在初始化中！！！！");
    mv.start();
    screen.show();
    int delayTime = 5;
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < (delayTime * 1000))
    {
        a.processEvents();
    }
    mainWidget w;
    screen.finish(&w);
    w.setWindowState(Qt::WindowMaximized);
    w.setWindowFlags(Qt::FramelessWindowHint);//设置窗体无边框
   // w.setWindowOpacity(0.7);

    QPalette palette;
    QPixmap pixmap(":/images/0.png");
    w.setAutoFillBackground(true);
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    w.setPalette(palette);
    w.show();

    QIcon icon("src/1.png");
    w.setWindowIcon(icon);

    return a.exec();
}
