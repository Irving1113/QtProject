#ifndef DATABASE_H
#define DATABASE_H

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QMap>
#include <QDateTime>

class Work{     //数据读写格式规范
public:
    QString name;
    QString begin_time;
    QString end_time;
    int last_time;
    QString label;
    QString remarks;
};

class Database{     //数据库相关操作
public:
    QSqlDatabase db;
    Database();
    void connect();     //建立连接
    void disconnect();      //断开连接
    void insert_worklist(QVector<Work> &worklist);      //插入若干条Work
    void insert_data(QString cmd);      //不要调用！
    QVector<QVector<QString> > get_data(QString cmd, int n);        //不要调用！
    static int connection_count;
};

class Selector{     //读取数据相关操作
public:
    Database & db;
    Selector(Database & _db);
    QVector<QPair<QString, int> > getdailystat(int n);      //按日期获取一天中各个label及对应时长
    QVector<Work> getdaylabel(int n, QString label);        //获取某一日期和label下各个事项的数据
    QString getdate(int n);     //获取从当前日期开始n天前的日期
    QVector<QDateTime> getalldate(int n);     //获取从当前日期开始n天内全部日期
    QMap<QString, QVector<int> > datacollection(int n);     //获取近n天中各个label的使用情况：<lable, <date, last_time>>
    QVector<QString> getlabels_days(int n);     //获取近n天中全部的label
    QVector<QString> getnames(int n);       //获取最近创建的n个事项名称
    QVector<QString> getlabels(int n);       //获取最近创建的n个标签名称
    QVector<Work> getday(QString d);     //通过日期获取当天全部事项
};

#endif // DATABASE_H
