#include "mainwidget.h"
#include "database.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Database db;
    db.connect();       //1. 连接数据库
    Selector selector(db);      //2. 实例化Selector类
    auto dta = selector.datacollection(7);      //3. 调用datacollection函数，获取近n天的数据，修改参数为需要查询的天数
    auto val = selector.getalldate(7);      //4. 调用getalldate函数，获取近n天的日期，修改参数为需要查询的天数
    MainWidget w(0, dta, val);      //5. 实例化MainWidget类（参数无需修改）
    w.resize(720, 480);     //6. 设置窗口大小，请勿修改
    w.show();       //7. 显示窗口
    db.disconnect();        //8. 断开数据库

    return a.exec();
}
