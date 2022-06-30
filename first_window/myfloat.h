#ifndef MYFLOAT_H
#define MYFLOAT_H

#include <QWidget>
#include <QtWidgets>

namespace Ui {
class myfloat;
}

class work
{
public:
    QString name;//任务具体名称
    QString begin_time;//开始时间，使用时间转字符串函数格式："yyyy-MM-dd hh:mm:ss"
    QString end_time;
    int last_time;//以分钟为单位
    QString label;
    QString remarks;

    bool operator <(const work& a)const {
        if(name<a.name)
            return true;
        if(label<a.label)
            return true;
        return false;
    }
};

class myfloat : public QWidget
{
    Q_OBJECT

public:
    explicit myfloat(QWidget *parent = 0);
    void initial();
    ~myfloat();

private slots:
    void update_remark();

    //这个槽函数还承担着数据读取的任务
    void recieve_drop(QStringList a);



private:
    Ui::myfloat *ui;
    work re;
};

#endif // MYFLOAT_H
