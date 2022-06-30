#include "widget.h"
#include <QApplication>
#include<qbstopwatch.h>
#include<mainwidget.h>
#include<third_widget.h>
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    widget w;
    w.show();
//    QBStopWatch tt;
//    tt.InitData();
    //tt.show();

//    third_widget w1;
//    //w1.show();
//    Database db;
//    db.connect();//1. 连接数据库
//    qDebug()<<"haha";
//    Selector selector(db);      //2. 实例化Selector类
//    auto dta = selector.datacollection(7);      //3. 调用datacollection函数，获取近n天的数据，修改参数为需要查询的天数
//    auto val = selector.getalldate(7);      //4. 调用getalldate函数，获取近n天的日期，修改参数为需要查询的天数
//    MainWidget t(0, dta, val);      //5. 实例化MainWidget类（参数无需修改）
//    t.resize(720, 480);     //6. 设置窗口大小，请勿修改
//    t.show();       //7. 显示窗口     //8. 断开数据库
//    qDebug()<<"b1"<<endl;
//    db.disconnect();


    return a.exec();
}
