#pragma execution_character_set("utf-8")

#include "tumblerdatetime.h"
#include "qboxlayout.h"
#include "tumbler.h"
#include "qprocess.h"
#include "qtimer.h"
#include "qdebug.h"
TumblerDateTime::TumblerDateTime(QWidget *parent) : QWidget(parent)
{
    foreground = QColor(140, 140, 140);
    background = QColor(40, 40, 40);
    lineColor = QColor(46, 142, 180, 200);
    textColor = QColor(255, 255, 255);
    clickEnable = true;

    showDateTime = false;
    writeDateTime = false;
    dateTime = QDateTime::currentDateTime();

    initWidget();
}

TumblerDateTime::~TumblerDateTime()
{
    if (timer->isActive()) {
        timer->stop();
    }
}

void TumblerDateTime::initWidget()
{
    /*//年份选择器
    tumblerYear = new Tumbler(this);
    QStringList listYear;
    for (int i = 2015; i <= 2030; i++) {
        listYear << QString("%1年").arg(i);
    }

    tumblerYear->setListValue(listYear);

    //月份选择器
    tumblerMonth = new Tumbler(this);
    QStringList listMonth;
    for (int i = 1; i <= 12; i++) {
        listMonth << QString("%1月").arg(i, 2, 10, QChar('0'));
    }

    tumblerMonth->setListValue(listMonth);

    //日期选择器
    tumblerDay = new Tumbler(this);
    QStringList listDay;
    for (int i = 1; i <= 31; i++) {
        listDay << QString("%1日").arg(i, 2, 10, QChar('0'));
    }

    tumblerDay->setListValue(listDay);

    //年月日联动
    connect(tumblerYear, SIGNAL(currentValueChanged(QString)), this, SLOT(currentValueChanged(QString)));
    connect(tumblerMonth, SIGNAL(currentValueChanged(QString)), this, SLOT(currentValueChanged(QString)));*/

    //时钟选择器
    tumblerHour = new Tumbler(this);
    QStringList listHour;
    for (int i = 0; i <= 23; i++) {
        listHour << QString("%1时").arg(i, 2, 10, QChar('0'));
    }

    tumblerHour->setListValue(listHour);

    //分钟选择器
    tumblerMin = new Tumbler(this);
    QStringList listMin;
    for (int i = 0; i <= 59; i++) {
        listMin << QString("%1分").arg(i, 2, 10, QChar('0'));
    }

    tumblerMin->setListValue(listMin);

    /*//秒钟选择器
    tumblerSec = new Tumbler(this);
    QStringList listSec;
    for (int i = 0; i <= 59; i++) {
        listSec << QString("%1秒").arg(i, 2, 10, QChar('0'));
    }

    tumblerSec->setListValue(listSec);*/

    QPushButton* button=new QPushButton("确定");

    //将选择器添加到布局
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    /*layout->addWidget(tumblerYear, 3);
    layout->addWidget(tumblerMonth, 2);
    layout->addWidget(tumblerDay, 2);*/
    layout->addWidget(tumblerHour, 2);
    layout->addWidget(tumblerMin, 2);
    //layout->addWidget(tumblerSec, 2);


    QVBoxLayout *mainlayout=new QVBoxLayout(this);
    mainlayout->addLayout(layout);
    mainlayout->addWidget(button);

    this->setFixedSize(300,400); // 设置后页面大小不可调整，一直处于系统推荐大小的状态
    /*connect(tumblerYear, SIGNAL(clicked()), this, SLOT(clicked()));
    connect(tumblerMonth, SIGNAL(clicked()), this, SLOT(clicked()));
    connect(tumblerDay, SIGNAL(clicked()), this, SLOT(clicked()));*/
    connect(tumblerHour, SIGNAL(clicked()), this, SLOT(clicked()));
    connect(tumblerMin, SIGNAL(clicked()), this, SLOT(clicked()));
    //connect(tumblerSec, SIGNAL(clicked()), this, SLOT(clicked()));

    connect(button,SIGNAL(clicked()),SLOT(confirmclick()));
    /*connect(tumblerYear, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged()));
    connect(tumblerMonth, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged()));
    connect(tumblerDay, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged()));*/
    connect(tumblerHour, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged()));
    connect(tumblerMin, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged()));
    //connect(tumblerHour,SIGNAL(currentValueChanged(const QString &)),SLOT(currentValueChanged(const QString &)));
    //connect(tumblerSec, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged()));

    //启动定时器显示时间,并立即执行一次
    isTimer = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(doTimer()));
    timer->start(1000);
    doTimer();
}

void TumblerDateTime::initStyle()
{
    QStringList qss;
    qss.append(QString("Tumbler{qproperty-foreground:%1;}").arg(foreground.name()));
    qss.append(QString("Tumbler{qproperty-background:%1;}").arg(background.name()));
    qss.append(QString("Tumbler{qproperty-lineColor:%1;}").arg(lineColor.name()));
    qss.append(QString("Tumbler{qproperty-textColor:%1;}").arg(textColor.name()));
    qss.append(QString("Tumbler{qproperty-clickEnable:%1;}").arg(clickEnable));

    this->setStyleSheet(qss.join(""));
}

void TumblerDateTime::doTimer()
{
    if (!showDateTime) {
        return;
    }

    isTimer = true;
    setDateTime(QDateTime::currentDateTime());
    isTimer = false;
}

void TumblerDateTime::clicked()
{
    if (timer->isActive()) {
        isTimer = false;
        timer->stop();
    }

    //单击立即更新时间
    QString year = tumblerYear->getCurrentValue().left(4);
    QString month = tumblerMonth->getCurrentValue().left(2);
    QString day = tumblerDay->getCurrentValue().left(2);
    QString hour = tumblerHour->getCurrentValue().left(2);
    QString min = tumblerMin->getCurrentValue().left(2);
    QString sec = tumblerSec->getCurrentValue().left(2);
    qDebug()<<getcurtime();
    //setSystemDateTime(year, month, day, hour, min, sec);

    if (showDateTime) {
        isTimer = true;
        timer->start();
    }
}

void TumblerDateTime::currentIndexChanged()
{
    emit currentDateTimeChanged(dateTime);
}

void TumblerDateTime::currentValueChanged(const QString &)
{
    int month = tumblerMonth->getCurrentValue().left(2).toInt();
    qDebug()<<getcurtime();
    //记住之前的日期
    int day = tumblerDay->getCurrentValue().left(2).toInt();

    //计算该月最大日期
    int maxDay = 30;
    if (month == 2) {
        //平年28天 闰年29天
        int year = tumblerYear->getCurrentValue().left(4).toInt();
        bool isLoopYear = (((0 == (year % 4)) && (0 != (year % 100))) || (0 == (year % 400)));
        if (isLoopYear) {
            maxDay = 29;
        } else {
            maxDay = 28;
        }
    } else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
        maxDay = 31;
    }

    QStringList listDay;
    for (int i = 1; i <= maxDay; i++) {
        listDay << QString("%1日").arg(i, 2, 10, QChar('0'));
    }
    tumblerDay->setListValue(listDay);

    //如果上次的日期大于最大的日期则设置为最大的日期
    if (day > maxDay) {
        tumblerDay->setCurrentIndex(maxDay - 1);
    } else {
        tumblerDay->setCurrentIndex(day - 1);
    }
    getcurtime();
}

QColor TumblerDateTime::getForeground() const
{
    return this->foreground;
}

QColor TumblerDateTime::getBackground() const
{
    return this->background;
}

QColor TumblerDateTime::getLineColor() const
{
    return this->lineColor;
}

QColor TumblerDateTime::getTextColor() const
{
    return this->textColor;
}

bool TumblerDateTime::getClickEnable() const
{
    return this->clickEnable;
}

bool TumblerDateTime::getShowDateTime() const
{
    return this->showDateTime;
}

bool TumblerDateTime::getWriteDateTime() const
{
    return this->writeDateTime;
}

QDateTime TumblerDateTime::getDateTime() const
{
    return this->dateTime;
}

QSize TumblerDateTime::sizeHint() const
{
    return QSize(450, 200);
}

QSize TumblerDateTime::minimumSizeHint() const
{
    return QSize(150, 70);
}

void TumblerDateTime::setForeground(const QColor &foreground)
{
    if (this->foreground != foreground) {
        this->foreground = foreground;
        initStyle();
    }
}

void TumblerDateTime::setBackground(const QColor &background)
{
    if (this->background != background) {
        this->background = background;
        initStyle();
    }
}

void TumblerDateTime::setLineColor(const QColor &lineColor)
{
    if (this->lineColor != lineColor) {
        this->lineColor = lineColor;
        initStyle();
    }
}

void TumblerDateTime::setTextColor(const QColor &textColor)
{
    if (this->textColor != textColor) {
        this->textColor = textColor;
        initStyle();
    }
}

void TumblerDateTime::setClickEnable(bool clickEnable)
{
    if (this->clickEnable != clickEnable) {
        this->clickEnable = clickEnable;
        initStyle();
    }
}

void TumblerDateTime::setShowDateTime(bool showDateTime)
{
    if (this->showDateTime != showDateTime) {
        this->showDateTime = showDateTime;
    }
}

void TumblerDateTime::setWriteDateTime(bool writeDateTime)
{
    if (this->writeDateTime != writeDateTime) {
        this->writeDateTime = writeDateTime;
    }
}

void TumblerDateTime::setDateTime(const QDateTime &dateTime)
{
    QString str = dateTime.toString("yyyy-MM-dd-HH-mm-ss");
    QStringList list = str.split("-");
    //setDateTime(list.at(0), list.at(1), list.at(2), list.at(3), list.at(4), list.at(5));
}

void TumblerDateTime::setDateTime(const QString &hour, const QString &min)
{
    //QString str = QString("%1-%2-%3 %4:%5:%6").arg(year).arg(month).arg(day).arg(hour).arg(min).arg(sec);
    //dateTime = QDateTime::fromString(str, "yyyy-MM-dd HH:mm:ss");
    //tumblerYear->setCurrentValue(QString("%1年").arg(year));
    //tumblerMonth->setCurrentValue(QString("%1月").arg(month));
    //tumblerDay->setCurrentValue(QString("%1日").arg(day));
    tumblerHour->setCurrentValue(QString("%1时").arg(hour));
    tumblerMin->setCurrentValue(QString("%1分").arg(min));
    //tumblerSec->setCurrentValue(QString("%1秒").arg(sec));

    //setSystemDateTime(year, month, day, hour, min, sec);
}

int TumblerDateTime:: getcurtime()
{
    QString hourstring=tumblerHour->getCurrentValue();
    int ans=0;
    ans+=hourstring.left(2).toInt()*60;
    QString minstring=tumblerMin->getCurrentValue();
    ans+=minstring.left(2).toInt();
    qDebug()<<"sucess";
    return ans;
}

void TumblerDateTime::confirmclick()
{
    int ans=getcurtime();
    qDebug()<<"tumblerdatetime"<<ans;
    emit sendsignal(ans);//发出信号
    QWidget::close();
}
void TumblerDateTime::setSystemDateTime(const QString &year, const QString &month, const QString &day,
                                        const QString &hour, const QString &min, const QString &sec)
{
    if (!writeDateTime || isTimer) {
        return;
    }
#ifdef Q_OS_WIN
    QProcess p(0);
    p.start("cmd");
    p.waitForStarted();
    p.write(QString("date %1-%2-%3\n").arg(year).arg(month).arg(day).toLatin1());
    p.closeWriteChannel();
    p.waitForFinished(1000);
    p.close();
    p.start("cmd");
    p.waitForStarted();
    p.write(QString("time %1:%2:%3.00\n").arg(hour).arg(min).arg(sec).toLatin1());
    p.closeWriteChannel();
    p.waitForFinished(1000);
    p.close();
#else
    QString cmd = QString("date %1%2%3%4%5.%6").arg(month).arg(day).arg(hour).arg(min).arg(year).arg(sec);
    system(cmd.toLatin1());
    system("hwclock -w");
#endif
}
