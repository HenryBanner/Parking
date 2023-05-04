#include "registwindow.h"
#include "ui_registwindow.h"
#include <QGraphicsBlurEffect>
#include <QMovie>
#include <QFile>
#include <QLabel>
#include <QMouseEvent>
#include <QLine>
#include <QMessageBox>
#include "WaterDrop.h"
registWindow::registWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::registWindow)
{
    ui->setupUi(this);
    //initForm();
    setMouseTracking(true);
    ui->btn_qq->setToolTip("显示已注册信息");
    ui->btn_wb->setToolTip("返回到主窗口");
    ui->btn_wx->setToolTip("点击确认注册");
    ui->btn_1->setToolTip("切换主题为主题1");
    ui->btn_2->setToolTip("切换主题为主题2");
    ui->btn_3->setToolTip("切换主题为主题3");
    ui->btn_4->setToolTip("切换主题为主题4");

    //this->setWindowTitle("智能停车管理系统");
    //this->setFixedSize(540, 540);
    //this->setStyleSheet("background-color:#96e0f6");
    ui->label_user_name->setScaledContents(true);   //图片自适应label大小
    ui->label_pwd->setScaledContents(true);         //图片自适应label大小

    //ui->lineE_pwd->setEchoMode(QLineEdit::Password);//设置为小黑点

    connect(ui->btn_1,SIGNAL(clicked(bool)),this,SLOT(set_style()));
    connect(ui->btn_2,SIGNAL(clicked(bool)),this,SLOT(set_style()));
    connect(ui->btn_3,SIGNAL(clicked(bool)),this,SLOT(set_style()));
    connect(ui->btn_4,SIGNAL(clicked(bool)),this,SLOT(set_style()));
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("vip.db");
    if(!db.open())
    {
        qDebug() << "打开失败!" << db.lastError().text();
        return;
    }

    //QString sql = QString("CREATE TABLE if not exists info(name varchar(20),单价 float,数量 int,进货价 float);");
    QString sql = QString("create table if not exists Info(车牌号 varchar(20),手机号 int);");
    QSqlQuery query;
    query.prepare(sql);
    if(!query.exec(sql))
    {
        qDebug() << db.lastError().text();

    }

}

registWindow::~registWindow()
{
    delete ui;
}
// 新建一窗口类，重写mousePressEvent事件即可;
void registWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint cursorPos = event->pos();
    qDebug() << "mousePressEvent" << cursorPos;


}

void registWindow::mouseMoveEvent(QMouseEvent *event)
{
    (void)(event);
    setWindowState(Qt::WindowActive);
}



QPushButton* btn;
void registWindow::set_style()
{
    btn = qobject_cast<QPushButton*>(sender());//获取发射信号的对象
    QString filePath;
    if("btn_1" == btn->objectName())        //粉色
    {
        filePath = ":/res/qss/style-1.qss";
    }else if("btn_2" == btn->objectName())  //黄蓝
    {
        filePath = ":/res/qss/style-2.qss";
    }else if("btn_3" == btn->objectName())  //浅紫
    {
        filePath = ":/res/qss/style-3.qss";
    }else if("btn_4" == btn->objectName())  //青绿
    {
        filePath = ":/res/qss/style-4.qss";
    }

    /*皮肤设置*/
    QFile file(filePath);/*QSS文件所在的路径*/
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();
}

void registWindow::help()
{
    QString text = QString("帮助");
    QString msg = QString("帮助信息将显示到界面中");
    int ret = QMessageBox::warning(this,text,msg,QMessageBox::Ok);
    if(ret = QMessageBox::Ok)
    {
        ui->textBrowser->append("此界面需要提供车主的车牌号以及车主手机，为防止车主手机号码泄露，采用加密输入方式");
    }
}
//鼠标双击特效
void registWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    //判断是否为鼠标左键双击
    if(event->button() == Qt::LeftButton)
    {
        QLabel * label = new QLabel(this);
        QMovie * movie = new QMovie(":/res/pic/click.gif");//加载gif图片
        //设置label自动适应gif的大小
        label->setScaledContents(true);

        label->setMovie(movie);
        //这里为了调用move方便，进行resize，需要知道的是gif的大小本来也就是150*150
        label->resize(180,180);
        label->setStyleSheet("background-color:rgba(0,0,0,0);");
        //设置鼠标穿透
        label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        //让label的中心在当前鼠标双击位置
        label->move(event->pos().x()-label->width()/2,event->pos().y()-label->height()/2);
        //开始播放gif
        movie->start();
        movie->setSpeed(100);

        label->show();

        //绑定QMovie的信号，判断gif播放次数
        connect(movie, &QMovie::frameChanged, [=](int frameNumber) {
            if (frameNumber == movie->frameCount() - 1)//gif播放次数为1，关闭标签
                label->close();
        });
    }
    else if(event->button() == Qt::RightButton)
    {
        help();
    }
}

void registWindow::on_btn_wb_clicked()
{
    QWidget *parent = this->parentWidget();

    parent->show();
    this->hide();
}

void registWindow::on_btn_login_clicked()
{
    QString carId = ui->lineE_user_name->text();
    QString phone = ui->lineE_pwd->text();
    QString sql = QString("insert into Info(车牌号,手机号) values('%1','%2')").arg(carId)
            .arg(phone);
    QSqlQuery query;
    if(!query.exec(sql))
    {
        qDebug() << db.lastError().text();

    }
    ui->textBrowser->append("录入成功!");
    //ui->lineE_pwd->clear();
    //ui->lineE_user_name->clear();
    emit setvipSignal(true);
}

void registWindow::on_btn_qq_clicked()
{
    ui->textBrowser->clear();
    QString sqlSelect = QString("SELECT * FROM Info where 手机号='%1';").arg(ui->lineE_pwd->text());
    QSqlQuery query;
    QString list;
    if(!query.exec(sqlSelect)) {
    qDebug() << "Error Failed to slect record"<<db.lastError();
    }else {
    //开始读取表中的记录
        while (query.next())
        {
    //通过字段名来获取对应的值
            list.append("车牌号:"+query.value("车牌号").toString());
            list.append("手机号:"+query.value("手机号").toString());


            ui->textBrowser->append(list);

        }
    }
    //ui->textBrowser->append("\n");
}
void registWindow::initForm()
{
    //设置标题栏控件
    //this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    //ui->setText("智慧停车注册系统");
    //this->setWindowTitle(ui->labTitle->text());
    //this->setTitleBar(ui->labTitle);

    //关联信号
    connect(this, SIGNAL(titleDblClick()), this, SLOT(titleDblClick()));
    connect(this, SIGNAL(windowStateChange(bool)), this, SLOT(windowStateChange(bool)));

    //设置样式表
    QStringList list;
    list << "#titleBar{background:#BBBBBB;}";
    list << "#titleBar{border-top-left-radius:8px;border-top-right-radius:8px;}";
    list << "#widgetMain{border:2px solid #BBBBBB;background:#FFFFFF;}";
    //list << "#widgetMain{border-bottom-left-radius:8px;border-bottom-right-radius:8px;}";
    this->setStyleSheet(list.join(""));
}

void registWindow::setLabelStyle(QLabel *label)
{
//    //字体设置
//    int id = QFontDatabase::addApplicationFont(":/font/DS-DIGI.TTF");
//     qDebug()<<"family"<<QFontDatabase::applicationFontFamilies(id);

    //使用方式和普通字体一样
    QFont font;
    font.setFamily("DS-Digital");
    font.setPixelSize(38);
    label->setFont(font);

    //模糊、加强、阴影、透明特效
    //该类用应产生模糊效果,主要函数setBlurRadius(qreal blurRadius),用于控制图形元素的模糊度，数值越大越模糊
    QGraphicsBlurEffect *e0 = new QGraphicsBlurEffect(this);
    e0->setBlurRadius(2);

    //该类提供了使用另外一种颜色对当前图形的一种着色功能。主要函数是setColor(QColor)和setStrength (qreal strength)，指定了着色和着色强度。
    QGraphicsColorizeEffect *e1 = new QGraphicsColorizeEffect(this);
    e1->setColor(QColor(0,255,255));

    //该类提供了图形元素的阴影效果，用于增加立体感。
    QGraphicsDropShadowEffect *e2 = new QGraphicsDropShadowEffect(this);
    e2->setColor(QColor(180,220,232));     //用于设定阴影的颜色
    e2->setBlurRadius(10);               //用于设定阴影的模糊度，
    e2->setOffset(4,4);       //用于设定在哪个方向产生阴影效果，如果dx为负数，则阴影在图形元素的左边。

    //用于设置透明度，参数值在0和1.0之间。
    //也可以设置部分透明效果，需要调用的函数是setOpacityMask (QBrush mask)。
    QGraphicsOpacityEffect *e3 = new QGraphicsOpacityEffect(this);
    e3->setOpacity(0.4);

    label->setGraphicsEffect(e2);
}

void registWindow::setBackStyle(QLabel *label)
{
        //创建label
    label->setGeometry(0,0,800,600);//设置位置和大小，我的窗口是800*480,所以我设这么大
    label->setAlignment(Qt::AlignCenter);//设置字体上下左右BABA居中咯（这里很重要,是你的动图在lable的位置）
    label->setStyleSheet("background:transparent");//这个很重要，这个叫背景颜色透明（当然动图背景也要选透明的）
    label->setAlignment(Qt::AlignBottom);
    QPixmap pic = QPixmap(":/images/1.jpg");//写上你的图片路径，我这里是用了资源
    pic.scaled(label->size());
    label->setPixmap(pic);

}

void registWindow::titleDblClick()
{
    on_btnMenu_Max_clicked();
}

void registWindow::windowStateChange(bool max)
{
    //ui->btnMenu_Max->setText(max ? "还原" : "最大");
}

void registWindow::on_btnMenu_Min_clicked()
{
#ifdef Q_OS_MACOS
    this->setWindowFlags(this->windowFlags() & ~Qt::FramelessWindowHint);
#endif
    this->showMinimized();
}

void registWindow::on_btnMenu_Max_clicked()
{
    if (this->isMaximized()) {
        this->showNormal();
        //ui->btnMenu_Max->setText("最大");
    } else {
        this->showMaximized();
        //ui->btnMenu_Max->setText("还原");
    }
}

void registWindow::on_btnMenu_Close_clicked()
{
    this->close();
}

void registWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == false)
    {
        ui->lineE_pwd->setEchoMode(QLineEdit::Password);//设置为小黑点
    }
    if(arg1 == true)
    {
        ui->lineE_pwd->setEchoMode(QLineEdit::Normal);//设置为小黑点
    }
}
