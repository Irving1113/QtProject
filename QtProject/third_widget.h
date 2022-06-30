#ifndef Third_widget_H
#define Third_widget_H

#include <QWidget>
#include<QDebug>
#include<QTextEdit>
#include<QDial>
#include<QProgressBar>
#include<QMenu>
#include<QAction>
#include<QGraphicsDropShadowEffect>
#include"tumblerdatetime.h"
#include"database.h"
#include<new_page.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
/*class Work
{
public:
    QString name;//任务具体名称
    QString begin_time;//开始时间，使用时间转字符串函数格式："yyyy-MM-dd hh:mm:ss"
    QString end_time;
    int last_time;//以分钟为单位
    QString label;
    QString remarks;
};*/

class third_widget : public QWidget
{
    Q_OBJECT

public:
    Work*worklist=new Work[20];
    QSet<QString>labellist;
    third_widget(QWidget *parent = nullptr);
    new_page* pp;
    ~third_widget();

private slots:


    void on_workremark_textChanged();


    void on_comboBox_activated(const QString &arg1);


    void on_goback_clicked();

    void on_workname_textChanged(const QString &arg1);

    void on_workname_selectionChanged();

    void on_goback_2_clicked();

    void on_Button_for_time_clicked();

private slots:
    void changetime(int);//子窗口返回槽函数
signals:
    void sendsignal(int);//子窗口传递信号参数

private:
    TumblerDateTime* timewindow;//子窗口指针

private:
    void dbwrite();
    void dbquery();
private:
    Ui::Widget *ui;
public:
    void setinit(Work a);
signals:
    void signal3_1();
    void signal3_2();

};
#endif // WIDGET_H
