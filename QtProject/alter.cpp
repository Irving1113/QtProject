#include "alter.h"
#include "ui_alter.h"
#include<QMouseEvent>
#include<QDebug>
#include<QtWidgets>
alter::alter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::alter)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //setAttribute(Qt::WA_TranslucentBackground);
    refresh();

}

alter::~alter()
{
    delete ui;
}

//存储相关的函数
void alter::refresh(){
    re.refresh_();
    up_user_edit.clear();
    down_user_edit.clear();
    Database db;
    db.connect();
    Selector selector(db);
    QVector<QString> t1= selector.getlabels(5);
    for(auto k:t1){
        up_user_edit<<k;
    }
    t1= selector.getnames(5);
    for(auto k:t1){
        down_user_edit<<k;
    }
    db.disconnect();
    ui->up_box->addItems(up_user_edit);
    ui->down_box->addItems(down_user_edit);

}

//two combox
void alter::on_up_box_activated(const QString &arg1)
{
    re.label=arg1;
    up_user_edit<<re.label;
}

void alter::on_down_box_activated(const QString &arg1)
{
    re.item=arg1;
    down_user_edit<<re.item;
}

void alter::on_up_box_highlighted(const QString &arg1)
{
    re.label=arg1;
    up_user_edit<<re.label;
}

void alter::on_down_box_highlighted(const QString &arg1)
{
    re.item=arg1;
    down_user_edit<<re.item;
}

//功能键
void alter::on_back_clicked()
{
    for(auto k:up_user_edit){
         qDebug()<<k;
    }

    refresh();
    hide();
}




//几个按钮
void alter::on_up_button1_clicked()
{
    re.label=ui->up_button1->text();
    up_user_edit<<re.label;
}


void alter::on_up_button2_clicked()
{
    re.label=ui->up_button2->text();
        up_user_edit<<re.label;
}

void alter::on_up_button3_clicked()
{
    re.label=ui->up_button3->text();
        up_user_edit<<re.label;
}

void alter::on_down_button1_clicked()
{
    re.item =ui->down_button1->text();
        up_user_edit<<re.item;
}

void alter::on_down_button2_clicked()
{
    re.item=ui->down_button2->text();
        up_user_edit<<re.item;
}

void alter::on_down_button3_clicked()
{
    re.item=ui->down_button3->text();
        up_user_edit<<re.item;
}

void alter::on_go_clicked()
{
    if(re.item==""||re.label==""){
            return;
    }
    Work temp;
    temp.label=re.label;
    temp.name=re.item;
    qDebug()<<"signal1_2"<<temp.label<<temp.name;
    emit signal1_2(temp);
}
