#ifndef REGISTWINDOW_H
#define REGISTWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QPushButton>
#include <QLabel>
#include <QContextMenuEvent>
#include <QProcess>
#include <QTableView>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QMenu>
namespace Ui {
class registWindow;
}

class registWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit registWindow(QWidget *parent = nullptr);
    ~registWindow();

private slots:
    void on_btn_wb_clicked();//返回按钮

    void on_btn_login_clicked();//注册按钮

    void on_btn_qq_clicked();

    void on_btnMenu_Min_clicked();

    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
    void titleDblClick();
    void windowStateChange(bool max);
    void initForm();
    void setLabelStyle(QLabel *label);
    void setBackStyle(QLabel *label);
    void set_style();
    void help();
    void on_checkBox_stateChanged(int arg1);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event); //鼠标双击事件
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    //virtual void contextMenuEvent(QContextMenuEvent *event);

signals:
    void setvipSignal(bool vipFlag);
private:
    Ui::registWindow *ui;
    QTableView *tbView1;
    QStandardItemModel *model1;
    QSqlQuery query;
    QSqlDatabase db;
};

#endif // REGISTWINDOW_H
