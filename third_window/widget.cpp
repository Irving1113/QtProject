#include "widget.h"
#include "ui_widget.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    labellist.insert("学习");
    labellist.insert("运动");
    labellist.insert("科研");
    labellist.insert("休息");
    for(auto p=labellist.begin();p!=labellist.end();p++)
        ui->comboBox->addItem(*p);
    worklist[0].name="程设";
    worklist[0].last_time=30;
    ui->workname->setText(worklist[0].name);

    //按钮效果开始
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(4);
    effect->setColor(QColor(45, 45, 46, 90));      //阴影的颜色
    effect->setOffset(5,5);          //阴影的偏移量
    ui->goback->setGraphicsEffect(effect);
    //按钮效果结束

    QGraphicsDropShadowEffect *effect2 = new QGraphicsDropShadowEffect;
    effect2->setBlurRadius(4);
    effect2->setColor(QColor(45, 45, 46, 90));      //阴影的颜色
    effect2->setOffset(5,5);          //阴影的偏移量
    ui->goback_2->setGraphicsEffect(effect2);
    //数字管初始化开始
    int defaulttime=worklist[0].last_time;
    QString last_time_default;
    if(defaulttime/60<10)
        last_time_default+="0"+QString::number(defaulttime/60);
    else
        last_time_default+="0"+QString::number(defaulttime/60);
    last_time_default+=':';
    if(defaulttime%60<10)
        last_time_default+="0"+QString::number(defaulttime%60);
    else
        last_time_default+=QString::number(defaulttime%60);
    ui->lcdNumber->display(last_time_default);
    //数字管初始化结束

    //滚轮基础设置开始
    timewindow=new TumblerDateTime;
    connect(timewindow,SIGNAL(sendsignal(int)),this,SLOT(changetime(int)));
}

Widget::~Widget()
{
    delete ui;
}




void Widget::on_workremark_textChanged()
{
    worklist[0].remarks=ui->workremark->toPlainText();
    qDebug()<<"workremarkchange:"<<worklist[0].remarks;
}


void Widget::on_comboBox_activated(const QString &arg1)
{
    worklist[0].label=ui->comboBox->currentText();
    qDebug()<<"worklabelchange:"<<worklist[0].label;
}



void Widget::on_goback_clicked()
{
    qDebug()<<"go back to page 1";
}





void Widget::on_workname_textChanged(const QString &arg1)
{
    worklist[0].name=ui->workname->text();
    qDebug()<<"worknamechange"<<worklist[0].name;
}

void Widget::on_workname_selectionChanged()
{
    ui->workname->clear();
}


void Widget::on_goback_2_clicked()
{
    qDebug()<<"go to page 4";
}

void Widget::on_Button_for_time_clicked()
{
   timewindow->show();
}
void Widget::changetime(int ans)
{
    worklist[0].last_time=ans;
    int newtime=worklist[0].last_time;
    QString new_time;
    if(newtime/60<10)
        new_time="0"+QString::number(newtime/60);
    else
        new_time=QString::number(newtime/60);
    new_time+=':';
    if(newtime%60<10)
        new_time+="0"+QString::number(newtime%60);
    else
        new_time+=QString::number(newtime%60);
    ui->lcdNumber->display(new_time);
}
