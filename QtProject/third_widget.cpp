#include "third_widget.h"
#include "ui_third_widget.h"
third_widget::third_widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->comboBox->addItem("学习");
    ui->comboBox->addItem("科研");
    ui->comboBox->addItem("休息");
    ui->comboBox->addItem("运动");
    /*worklist[0].name="程设";
    worklist[0].last_time=130;
    ui->comboBox->setCurrentIndex(1);
    worklist[0].label=ui->comboBox->currentText();
    ui->workname->setText(worklist[0].name);
    worklist[0].begin_time=QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    worklist[0].end_time=QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");*/
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

    //滚轮基础设置开始
    timewindow=new TumblerDateTime;
    connect(timewindow,SIGNAL(sendsignal(int)),this,SLOT(changetime(int)));

    //生成下一个界面
    //pp=new new_page();
}

third_widget::~third_widget()
{
    delete ui;
}
void third_widget::setinit(Work a)
{
    worklist[0]=a;
    //数字管初始化开始
    QString last_time_default;
    worklist[0].last_time=std::ceil(worklist[0].last_time/(double)60);
    int defaulttime=worklist[0].last_time;
    if(defaulttime/60<10)
        last_time_default+="0"+QString::number((int)(defaulttime/60));
    else
        last_time_default+="0"+QString::number(defaulttime/60);
    last_time_default+=':';
    if(defaulttime%60<10)
        last_time_default+="0"+QString::number(defaulttime%60);
    else
        last_time_default+=QString::number(defaulttime%60);
    ui->lcdNumber->display(last_time_default);
    //数字管初始化结束
    //文本框初始化开始
    ui->workname->setText(worklist[0].name);
    //文本框初始化结束
    ui->comboBox->setCurrentText(worklist[0].label);
}

void third_widget::dbwrite()
{
    Database db;
    db.connect();
    QVector<Work>temp;
    temp.push_back(worklist[0]);
    db.insert_worklist(temp);
    qDebug()<<"数据库写入内容";
    qDebug()<<"name"<<worklist[0].name<<"标签"<<worklist[0].label;
    qDebug()<<"持续时间"<<worklist[0].last_time;
    qDebug()<<"开始时间"<<worklist[0].begin_time<<"结束时间"<<worklist[0].end_time;
    qDebug()<<"备注"<<worklist[0].remarks;
    qDebug()<<"write successfully";
    db.disconnect();
}
void third_widget::dbquery()
{
    qDebug()<<"goto dbquery";
    Database db;
    db.connect();
    QVector<Work>temp;
    Selector selector(db);
    temp=selector.getdaylabel(0,"学习");
    if(!temp.empty())
        for(auto p=temp.begin();p!=temp.end();p++)
        {
            Work showtemp=*p;
            qDebug()<<"数据库输出";
            qDebug()<<showtemp.begin_time;
            qDebug()<<showtemp.end_time;
            qDebug()<<showtemp.last_time;
            qDebug()<<showtemp.name;
            qDebug()<<showtemp.label;
            qDebug()<<showtemp.remarks;
        }
    else
        qDebug()<<"it's empty";
    db.disconnect();
    return;
}
void third_widget::on_workremark_textChanged()
{
    worklist[0].remarks=ui->workremark->toPlainText();
    qDebug()<<"workremarkchange:"<<worklist[0].remarks;
}


void third_widget::on_comboBox_activated(const QString &arg1)
{
    worklist[0].label=ui->comboBox->currentText();
    qDebug()<<"worklabelchange:"<<worklist[0].label;
}



void third_widget::on_goback_clicked()
{
    qDebug()<<"go back to page 2";
    emit signal3_2();
}





void third_widget::on_workname_textChanged(const QString &arg1)
{
    worklist[0].name=ui->workname->text();
    qDebug()<<"worknamechange"<<worklist[0].name;
}

void third_widget::on_workname_selectionChanged()
{
    ui->workname->clear();
}


void third_widget::on_goback_2_clicked()
{
    qDebug()<<"go to page 4";
    dbwrite();
    dbquery();
    emit signal3_1();

    //pp->show();
}

void third_widget::on_Button_for_time_clicked()
{
   timewindow->show();
}
void third_widget::changetime(int ans)
{
    worklist[0].last_time=ans;
    qDebug()<<"change"<<worklist[0].last_time;
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
