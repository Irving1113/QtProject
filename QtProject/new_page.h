#ifndef NEW_PAGE_H
#define NEW_PAGE_H

#include <QWidget>
#include<myfloat.h>
#include<database.h>
#include<QtWidgets>
#include<mainwidget.h>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QAbstractItemView;
class QItemSelectionModel;
QT_END_NAMESPACE

namespace Ui {
class new_page;
}

class new_page : public QWidget
{
    Q_OBJECT

public:
    explicit new_page(QWidget *parent = 0);
    ~new_page();
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
    class one_item{
    public:
        int whole_time=0;
        QStringList whole_bt{};
        QStringList whole_et{};
        QStringList whole_rm{};
    };
    QString color[10][10];

private:
    void setupModel();
    void setupViews();
    //从数据库中读取数据 得到需要的东西
public:
    void data_load();
private:
    bool flag[150]{};
    MainWidget* p5;

signals:
    void drop(QStringList a);
    void signal4_1(int);

private slots:
    void slot_of_b1();
    void slot_of_b2();
    void slot_of_b3();
    void open_float(const QModelIndex& tt);
private:
    MainWidget* ww;
    QAbstractItemModel *model;
    QAbstractItemView *pieChart;
    QItemSelectionModel *selectionModel;
    myfloat* pp;
private:
    Ui::new_page *ui;
};

#endif // NEW_PAGE_H
