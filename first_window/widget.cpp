#include "widget.h"
#include "ui_widget.h"
#include<QDebug>
#include<Qlabel>
#include<QPushButton>
#include<QPixmap>
#include<QMenu>
#include<QPropertyAnimation>
#include<QDesktopWidget>
#include<Qpainter>
#include<QBitmap>
#include<Qtime>
#include<QPoint>
widget::widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget)
{
    ui->setupUi(this);
    p4=new new_page();
    connect(p4,SIGNAL(signal4_1(int)),this,SLOT(reshow(int)));
    this->setWindowFlags(this->windowFlags()|Qt::Tool|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    //取消标题，置顶，取消边框
    QPixmap pix;
    setWindowTitle(tr("Context Menu Show 1"));
    //为窗口添加QActions
    //addAction(new QAction(tr("&OpenWeb"), this));     //实际添加功能时，应在.h中声明，在这里添加槽函数进行连接
    //addAction(new QAction(tr("&CloseWeb"), this));
    //addAction(new QAction(tr("&Quit"), this));
    //设置contextMenuPolicy属性值为 '以Actions为弹出菜单的菜单项组成菜单'
    setContextMenuPolicy(Qt::ActionsContextMenu);
    pix.load(":/new/prefix1/4");   //图片添加
    resize(pix.size());
    setMask(pix.mask());
    sonstate=false;

    first=new alter;
    first->pp=this;
    sonstate=false;
    first->show();
    first->hide();
    connect(first,SIGNAL(signal1_2(Work)),this,SLOT(show2(Work)));
    p1=new QBStopWatch;
    p1->InitData();

    connect(p1,SIGNAL(signal2_1(int)),this,SLOT(reshow(int)));
    connect(p1,SIGNAL(signal2_3(Work)),this,SLOT(initial3(Work)));
    p3=new third_widget;
    connect(p3,SIGNAL(signal3_1()),this,SLOT(reset_all()));
    connect(p3,SIGNAL(signal3_2()),this,SLOT(reshow2()));
   }
widget::~widget()
{
    delete ui;
    qDebug()<<"delete";
}
void widget::reset_all()
{
    p1->hide();
    p3->hide();
    first->hide();
    delete p1;
    delete p3;
    delete first;
    first=new alter;
    connect(first,SIGNAL(signal1_2(Work)),this,SLOT(show2(Work)));
    p1=new QBStopWatch;
    p1->InitData();
    connect(p1,SIGNAL(signal2_1(int)),this,SLOT(reshow(int)));
    connect(p1,SIGNAL(signal2_3(Work)),this,SLOT(initial3(Work)));
    p3=new third_widget;
    connect(p3,SIGNAL(signal3_1()),this,SLOT(reset_all()));
    connect(p3,SIGNAL(signal3_2()),this,SLOT(reshow2()));
    this->show();
}
void widget::reshow2()
{
    p3->hide();
    p1->show();
}
void widget::show2(Work a)
{
    first->hide();
    sonstate=false;
    a.begin_time=QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    p1->show();
    qDebug()<<this->x()<<this->y();
    p1->move(this->x()-592,this->y()-292);
    p1->initinfor=a;
    this->hide();
}
void widget::initial3(Work a)
{
    first->hide();
    sonstate=false;
    qDebug()<<a.name<<" "<<a.label;
    qDebug()<<a.begin_time<<" "<<a.end_time;
    qDebug()<<a.last_time;
    p1->hide();
    p3->show();
    this->hide();
    p3->setinit(a);
}
void widget::reshow(int a)
{
    if(a==0)
    {
        qDebug()<<"window 2 reshow";
        p1->hide();
        this->show();
        first->show();
        sonstate=true;
    }
    else if(a==1)
    {
        p1->hide();
        this->show();
        first->hide();
        sonstate=false;
    }
}
void widget::mousePressEvent(QMouseEvent *event)     //左键点击拖动，中键关闭  右键点击打开选项菜单
{
    this->setWindowOpacity(1);
    if(event->button()==Qt::MidButton){
        this->close();
        exit(0);
    }
    if(event->button()==Qt::LeftButton){
      relativePos=this->pos()-event->globalPos();
      if(sonstate==false)
      {
          sonstate=true;
          first->show();
          QPoint tp=this->pos();
          tp.rx()-=first->frameSize().width();
          first->move(tp);
          qDebug()<<"should send page1 open signal";
      }
      else
      {
          sonstate=false;
          first->hide();
          qDebug()<<"should send page1 close signal";
      }
    }
    if(event->button()==Qt::RightButton)
    {
        qDebug()<<"should show page 4";
        p4->show();
        p4->data_load();
        this->hide();
    }
}
void widget::mouseMoveEvent(QMouseEvent *event){       //鼠标可以点击任意位置拖动悬浮球
    this->move(event->globalPos()+relativePos);
    QPoint tp=event->globalPos()+relativePos;
    tp.rx()-=first->frameSize().width();
    first->move(tp);
}
void widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,QPixmap(":/new/prefix1/4"));
}
void widget::mouseDoubleClickEvent(QMouseEvent *event)     //松开鼠标后自动贴边隐藏并且透明
{
   /* QTime time;
    time.start();
    while(time.elapsed() < 5000)             //等待时间流逝5秒钟
        QCoreApplication::processEvents();   //处理事件*/
    QPropertyAnimation *pAnimation=new QPropertyAnimation(this,"geometry");
    QDesktopWidget *pDesktopWidget=QApplication::desktop();//桌面
    int tx=width();
    int ty=height();
    int lx=pDesktopWidget->availableGeometry().width();
    first->hide();
    sonstate=false;
    if (this->x()<lx/2)
    {
        pAnimation->setDuration(100);
        pAnimation->setStartValue(QRect(this->x(),this->y(),width(),height()));   //起始位置
        pAnimation->setEndValue(QRect(-60,this->y(),width(),height()));       //终点位置
        pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        this->setWindowOpacity(0.3);
    }
    else
    {
        pAnimation->setDuration(100);
        pAnimation->setStartValue(QRect(this->x(),this->y(),width(),height()));
        pAnimation->setEndValue(QRect(60+lx-tx,this->y(),width(),height()));
        pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        this->setWindowOpacity(0.3);
    }
}




