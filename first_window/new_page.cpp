#include "new_page.h"
#include "ui_new_page.h"
#include"pieview.h"
#include"widget.h"
#include <QtWidgets>

new_page::new_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::new_page)
{
   //ui->setupUi(this);
    p5=new MainWidget();
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setupModel();
    setupViews();

    setWindowTitle(tr("Chart"));

    //初始化颜色
    color[0][0]="#f1340e";
    color[0][1]="#f76041";
    color[0][2]="#f77b62";
    color[0][3]="#f89c89";
    color[0][4]="#f8b2a4";



    color[1][0]="#f3c707";
    color[1][1]="#f8d22a";
    color[1][2]="#f8d849";
    color[1][3]="#f8e077";
    color[1][4]="#faeaa0";
    color[1][5]="#faf4d6";
    color[1][6]="#a4971d";
    color[1][7]="#8a7f1a";
    color[1][8]="#aaa67f";


    color[2][0]="#8ef507";
    color[2][1]="#a5f73a";
    color[2][2]="#bbf86a";
    color[2][3]="#c8f38f";
    color[2][4]="#e4f8c9";

    color[3][0]="#0975f8";
    color[3][1]="#4d9af8";
    color[3][2]="#82b5f5";
    color[3][3]="#c8e0fe";
    color[3][4]="#4a75aa";

    color[4][0]="#d504f3";
    color[4][1]="#df3ff7";
    color[4][2]="#e988f7";
    color[4][3]="#f1d8f5";
    color[4][4]="#a032af";

    color[5][0]="#02f8e8";
    color[5][1]="#50f8ed";
    color[5][2]="#9abcba";
    color[5][3]="#4cb5ae";
    color[5][4]="#117f77";

    color[7][0]="#a4971d";
    color[6][1]="#8a7f1a";
    color[6][2]="#aaa67f";


//    for(int i=0;i<10;++i){
//        for(int j=0;j<10;++j){
//            color[i][j]="";
//        }
//    }

    data_load();

    resize(650, 550);

}

//初始化model
//model是将数据脱离开图形界面存储的机制 相当于独立构建的数据结构
//继承自QAbstractItemModel
//基于项数据的标准数据模型，可以处理二维数据。
//维护一个二维的项数据数组，每个项是一个 QStandardltem 类的变量
//用于存储项的数据、字体格式、对齐方式等。

//
void new_page::data_load(){
   Database db;
   db.connect();
    Selector selector(db);
    model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());

    QDate date(QDate::currentDate());
    QString qStr = date.toString("yyyy-MM-dd");
  for(auto k:selector.getday(qStr)){
      qDebug()<<k.label;
  }
    int ct{};//普通计数器
    int cl{};//记录是第几个标签
    int col_num[20]{};//记录每个标签出现了几次
    QMap<QString,int> re_label{};
    qDebug()<<qStr<<endl<<endl<<endl;
    for(auto k:selector.getday(qStr)){
        int col_n;
        if(re_label.count(k.label)){
            col_n=re_label[k.label];
            col_num[re_label[k.label]]++;
        }
        else{
            col_n=cl;
            re_label[k.label]=cl;
            cl++;
        }
        model->insertRows(ct, 1, QModelIndex());
        model->setData(model->index(ct, 0, QModelIndex()),k.name);
        model->setData(model->index(ct, 1, QModelIndex()),k.last_time);
        model->setData(model->index(ct, 2, QModelIndex()),k.label);
        model->setData(model->index(ct, 0, QModelIndex()),
                       QColor(color[col_n][col_num[col_n]]), Qt::DecorationRole);

        ct++;
    }

//    model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());
//    for(int row=0;row<8;++row){
//        model->insertRows(row, 1, QModelIndex());
//        model->setData(model->index(row, 0, QModelIndex()),"1");
//        model->setData(model->index(row, 1, QModelIndex()),2);
//        model->setData(model->index(row, 2, QModelIndex()),"3");
//        model->setData(model->index(row, 0, QModelIndex()),
//                       QColor("#8070b3"), Qt::DecorationRole);
//    }
//     model->setData(model->index(2, 1, QModelIndex()),10);
    setupViews();
    db.disconnect();
}

void new_page::setupModel()
{
    //设model尺寸
    model = new QStandardItemModel(50,3, this);
    model->setHeaderData(0, Qt::Horizontal, tr("项目"));
    model->setHeaderData(1, Qt::Horizontal, tr("时间"));
    model->setHeaderData(2, Qt::Horizontal, tr("标签"));

    model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());
}


void new_page::slot_of_b1(){
    Database db;
    db.connect();//1. 连接数据库
    qDebug()<<"haha";
    Selector sselector(db);
    //2. 实例化Selector类
    auto dta = sselector.datacollection(7);      //3. 调用datacollection函数，获取近n天的数据，修改参数为需要查询的天数
    for (auto k: dta.keys()) {
        qDebug() << k << ' ' << dta[k] << ' ';
    }
    auto val = sselector.getalldate(7);      //4. 调用getalldate函数，获取近n天的日期，修改参数为需要查询的天数
    for (auto k: val) {
        qDebug() << k << ' ';
    }
    db.disconnect();
    ww=new     MainWidget(0, dta, val);
      //5. 实例化MainWidget类（参数无需修改）
    ww->resize(720, 480);     //6. 设置窗口大小，请勿修改
    ww->show();       //7. 显示窗口     //8. 断开数据库
    qDebug()<<"b1"<<endl;
}
void new_page::slot_of_b2(){
    //1. 连接数据库
        Database db;
    qDebug()<<"haha3";
    db.connect();
    Selector selector(db);      //2. 实例化Selector类
    auto dta = selector.datacollection(30);      //3. 调用datacollection函数，获取近n天的数据，修改参数为需要查询的天数
    auto val = selector.getalldate(30);      //4. 调用getalldate函数，获取近n天的日期，修改参数为需要查询的天数
    ww=new MainWidget (0, dta, val);      //5. 实例化MainWidget类（参数无需修改）
    ww->resize(720, 480);     //6. 设置窗口大小，请勿修改
    ww->show();       //7. 显示窗口
     //8. 断开数据库
    db.disconnect();
    qDebug()<<"b2"<<endl;
}
void new_page::slot_of_b3(){
    qDebug()<<"b3"<<endl;
    hide();
    emit(signal4_1(1));
}

void new_page::open_float(const QModelIndex& tt){
    int cc=tt.row();
    Database db;
    db.connect();
    qDebug()<<cc;
    QStringList things{};
    Selector selector(db);
    QDate date(QDate::currentDate());
    QString qStr = date.toString("yyyy-MM-dd");
    int ct{};
    for(Work k:selector.getday(qStr)){
        if(cc==ct){
            things<<k.name<<k.label<<QString::number(k.last_time)<<k.begin_time<<k.end_time<<k.remarks;
//            for(auto k:things){
//                qDebug()<<k;
//            }
            break;
        }
        ct++;
    }
    db.disconnect();
    //项目 标签 时间
    pp=new myfloat;
    connect(this,SIGNAL(drop(QStringList)),pp,SLOT(recieve_drop(QStringList)));
    emit drop(things);
    pp->show();

}

void new_page::setupViews()
{
    //分离条
    QSplitter *splitter = new QSplitter;
    //表格
    QTableView *table = new QTableView;
    //pieveiw
    pieChart = new PieView;

    //设置界面布局 并且通过确定父界面的方式直接变成主界面

    QGridLayout *t1=new QGridLayout(this);

    QPushButton* b1=new QPushButton("7 DAYS");
    QPushButton *b2=new QPushButton("30 DAYS");
    QPushButton *b3=new QPushButton("BACK");
    connect(b1,SIGNAL(clicked(bool)),this,SLOT(slot_of_b1()));
    connect(b2,SIGNAL(clicked(bool)),this,SLOT(slot_of_b2()));
    connect(b3,SIGNAL(clicked(bool)),this,SLOT(slot_of_b3()));

    QFont ff ( "华文琥珀", 25, 75);
    QFont f1 ( "华文琥珀", 15, 65);
    QLabel* ll=new QLabel("    YOUR DAY!!!");
    ll->setFont(ff);
    b1->setFont(f1);
    b2->setFont(f1);
    b3->setFont(f1);

    t1->addWidget(ll,0,0);
    t1->addWidget(b1,1,0);
    t1->addWidget(b2,1,1);
    t1->addWidget(b3,1,2);
    t1->addWidget(splitter,2,0,1,3);

    t1->setRowStretch(0,14);
    t1->setRowStretch(1,10);
    t1->setRowStretch(2,100);
    t1->setHorizontalSpacing(50);
    t1->setVerticalSpacing(10);

    splitter->addWidget(table);
    splitter->addWidget(pieChart);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    //直接在view上setmodel 也就是说为其显示了数据
    table->setModel(model);
    pieChart->setModel(model);

    //这个东西真高级 可以以model 为基础 让model中的结构被选中时同步记录
    //这个是用来发信号的 重点掌握
    selectionModel = new QItemSelectionModel(model);
    table->setSelectionModel(selectionModel);
    pieChart->setSelectionModel(selectionModel);
    connect(selectionModel,SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(open_float(const QModelIndex&)));


    QHeaderView *headerView = table->horizontalHeader();
    headerView->setStretchLastSection(true);

    //tt.resize(870, 550);
    //tt.show();
}



//对数据进行读取

//大致的过程是将文件读取进来后 直接转换村春到model里面


new_page::~new_page()
{
    delete ui;
}
