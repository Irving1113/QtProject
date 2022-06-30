#include "myfloat.h"
#include "ui_myfloat.h"

myfloat::myfloat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myfloat)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //setAttribute(Qt::WA_TranslucentBackground);
    ui->setupUi(this);
    initial();

    QPalette Palette;
    Palette.setColor(QPalette::Window, QColor(155, 220, 242));
    Palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    Palette.setColor(QPalette::HighlightedText, Qt::black);

    //setPalette(Palette);
    //qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");


}

myfloat::~myfloat()
{
    delete ui;
}
void myfloat::update_remark(){

    QString tt=ui->mainbox->toPlainText();
    re.remarks=tt;

}
void myfloat::recieve_drop(QStringList a){
    if(a.empty()){
        qDebug()<<"aaa";
        return;
    }
    re.name=a[0];
    re.label=a[1];
    re.last_time=a[2].toInt();
    re.begin_time=a[3];
    re.end_time=a[4];
    re.remarks=a[5];
    qDebug()<<re.name<<"  "<<1;
    initial();
}

void myfloat::initial(){

    ui->l1->setText(re.name);
    ui->l2->setText(re.label);
    ui->l3->setText(QString::number(re.last_time)+" min");
    ui->l4->setText(re.begin_time+" - "+re.end_time);
    ui->mainbox->setText(re.remarks);

    connect(ui->mainbox,SIGNAL(textChanged()),this,SLOT(update_remark()));
}



