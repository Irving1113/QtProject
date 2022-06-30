#ifndef ALTER_H
#define ALTER_H

#include <QWidget>
#include<QDateTime>
#include<set>
#include<database.h>

namespace Ui {
class alter;
}

struct remeber_in_2{
    QString label{};
    QString item{};
    QDateTime tt;

    //用来将选择清空
    void refresh_(){
        label.clear();
        item.clear();
        tt=QDateTime::currentDateTime();
    }

};

class alter : public QWidget
{
    Q_OBJECT

public:
    explicit alter(QWidget *parent = 0);
    ~alter();
    QWidget* pp;

    //存储相关
    remeber_in_2 re;//存储用户的选项

    QList<QString> up_user_edit{};//之前用户输入的东西会被存储在里面
    QList<QString> down_user_edit{};

    QString button_re[10]{};


    //void real_re();//每次结束窗口2时负责进行真正的存
    void refresh();

signals:
    void signal1_2(Work a);

private slots:
    void on_up_box_activated(const QString &arg1);

    void on_down_box_activated(const QString &arg1);

    void on_up_box_highlighted(const QString &arg1);

    void on_down_box_highlighted(const QString &arg1);

    void on_back_clicked();

    void on_up_button1_clicked();

    void on_up_button2_clicked();

    void on_up_button3_clicked();

    void on_down_button1_clicked();

    void on_down_button2_clicked();

    void on_down_button3_clicked();

    void on_go_clicked();

private:
    Ui::alter *ui;
};

#endif // ALTER_H
