#include "QBStopWatch.h"
#include <QPainter>
#include <QMouseEvent>
#include <QEvent>
#include <QTimer>
#include <QTime>
#include <QLayout>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <qmath.h>
#include <QSvgRenderer>
#include <QGestureEvent>
#include<QDateTime>

#define CIRQUERADIUS     150        //时钟半径
#define CIRQUEWIDTH      15         //时钟圆环宽度
#define SMALLCIRQUEWIDTH 4          //进度圆环的宽度
#define FONTPOINTSIZE    18         //时钟数字字体大小
#define BELLICONSIZE     25         //闹铃、调节按钮图标初始大小
#define STARTICONSIZE    52         //开始重置图标初始大小
#define LCDNUMBERHEIGHT  85         //LCD数字初始高度
#define SWITCHBUTTONSIZE 24         //计时器模式切换按钮初始大小

QBStopWatch::QBStopWatch(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    resize(QApplication::desktop()->width(),QApplication::desktop()->height());
    CreateUI();
    m_clockBell->installEventFilter(this);
    //注册大屏缩放手势
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
}

void QBStopWatch::CreateUI()
{
    //进度圆环
    m_pCirque = new QBCirque(m_smallCriqueWidth,m_magnifyRadius-m_currentCriqueWidth/2+m_smallCriqueWidth/2,QColor(44, 118, 255),QColor(255, 170, 0),this);
    m_pCirque->move(m_centerPoint.rx()-(m_magnifyRadius-m_currentCriqueWidth/2+m_smallCriqueWidth/2),m_centerPoint.ry()-(m_magnifyRadius-m_currentCriqueWidth/2+m_smallCriqueWidth/2));

    m_lcdNumber = new QLCDNumber;   //时间显示
    m_lcdNumber->display("00:00");

    m_sedOneUp = CreatecommonBtn(QIcon(":/images/sedup.svg"),QSize(BELLICONSIZE,BELLICONSIZE));    //秒调整按钮
    m_sedOneDown = CreatecommonBtn(QIcon(":/images/seddown.svg"),QSize(BELLICONSIZE,BELLICONSIZE));
    m_sedTwoUp = CreatecommonBtn(QIcon(":/images/sedup.svg"),QSize(BELLICONSIZE,BELLICONSIZE));
    m_sedTwoDown = CreatecommonBtn(QIcon(":/images/seddown.svg"),QSize(BELLICONSIZE,BELLICONSIZE));

    m_minOneUp = CreatecommonBtn(QIcon(":/images/sedup.svg"),QSize(BELLICONSIZE,BELLICONSIZE));     //分调整按钮
    m_minOneDown = CreatecommonBtn(QIcon(":/images/seddown.svg"),QSize(BELLICONSIZE,BELLICONSIZE));
    m_minTwoUp = CreatecommonBtn(QIcon(":/images/sedup.svg"),QSize(BELLICONSIZE,BELLICONSIZE));
    m_minTwoDown = CreatecommonBtn(QIcon(":/images/seddown.svg"),QSize(BELLICONSIZE,BELLICONSIZE));

    cursec=0;
    m_timeBtnGroup = new QButtonGroup;
    m_timeBtnGroup->addButton(m_sedOneUp,0);
    m_timeBtnGroup->addButton(m_sedOneDown,1);
    m_timeBtnGroup->addButton(m_sedTwoUp,2);
    m_timeBtnGroup->addButton(m_sedTwoDown,3);
    m_timeBtnGroup->addButton(m_minOneUp,4);
    m_timeBtnGroup->addButton(m_minOneDown,5);
    m_timeBtnGroup->addButton(m_minTwoUp,6);
    m_timeBtnGroup->addButton(m_minTwoDown,7);

    QHBoxLayout* timeuplayout = new QHBoxLayout;
    timeuplayout->addWidget(m_minTwoUp);
    timeuplayout->addSpacing(-5);
    timeuplayout->addWidget(m_minOneUp);
    timeuplayout->addSpacing(8);
    timeuplayout->addWidget(m_sedTwoUp);
    timeuplayout->addSpacing(-5);
    timeuplayout->addWidget(m_sedOneUp);
    QHBoxLayout* timedownlayout = new QHBoxLayout;
    timedownlayout->addWidget(m_minTwoDown);
    timedownlayout->addSpacing(-5);
    timedownlayout->addWidget(m_minOneDown);
    timedownlayout->addSpacing(8);
    timedownlayout->addWidget(m_sedTwoDown);
    timedownlayout->addSpacing(-5);
    timedownlayout->addWidget(m_sedOneDown);

    QString path = QCoreApplication::applicationDirPath() + "/Data/ToolsResource/bell.wav";
    path.replace("/","\\");
    m_bellplay = new QSound(path);
    m_bellplay->setLoops(QSound::Infinite);
    m_bellTimer = new QTimer;
    m_bellTimer->setInterval(50);
    m_clockBell = new QLabel;     //闹铃图标
    m_clockBell->setFixedSize(QSize(BELLICONSIZE,BELLICONSIZE));
    QHBoxLayout* belllayout = new QHBoxLayout;
    belllayout->addStretch();
    belllayout->addWidget(m_clockBell);
    belllayout->addStretch();

    finishbutton=CreatecommonBtn(QIcon(":/images/finish.png"),QSize(BELLICONSIZE,BELLICONSIZE));
    QHBoxLayout* finishlayout=new QHBoxLayout;
    finishlayout->addStretch();
    finishlayout->addWidget(finishbutton);
    finishlayout->addStretch();

    m_mainWid = new QWidget(this);
    QVBoxLayout* mainlayout = new QVBoxLayout(m_mainWid);
    mainlayout->addSpacing(25);
    mainlayout->addStretch();
    mainlayout->addLayout(timeuplayout);
    mainlayout->addWidget(m_lcdNumber);
    mainlayout->addLayout(timedownlayout);
    mainlayout->addStretch();
    mainlayout->addLayout(finishlayout);
    mainlayout->setContentsMargins(0,0,0,0);

    m_timer=new QTimer(this);
    m_timer->setInterval(1000);

    sectimer=new QTimer(this);
    sectimer->setInterval(1000);
    connect(sectimer,SIGNAL(timeout()),SLOT(on_sec_timeout()));

    m_closeBtn = CreatecommonBtn(QIcon(":/images/closeTool.svg"),QSize(24,24));
    m_closeBtn->setStyleSheet("background-repeat: no-repeat;background-position:center;border:none;}"
                              "background-color:rgb(255, 255, 255);border-radius:4px;border-style:solid;}");
    connect(m_closeBtn,SIGNAL(clicked()),this,SLOT(on_close_slot()));
    connect(finishbutton,SIGNAL(clicked()),this,SLOT(on_finish_slot()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(on_timeout_slot()));
    connect(m_bellTimer,SIGNAL(timeout()),this,SLOT(on_belltimeout_slot()));
    connect(m_timeBtnGroup,SIGNAL(buttonClicked(int)),this,SLOT(on_timeBtnGroup_slot(int)));
    m_timerButton = new QTimer(this);
    m_timerButton->setInterval(100);
    connect(m_timerButton,SIGNAL(timeout()),this,SLOT(on_timerButton_slot()));
}

void QBStopWatch::on_sec_timeout()
{
    cursec++;
    cursec%=60;
}
void QBStopWatch::on_finish_slot()
{
    qDebug()<<"finish activated";
    qDebug()<<"finish timestring"<<QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    int last_time=(m_sedOneNumber+10*m_sedTwoNumber)*60+(m_minOneNumber*60+m_minTwoNumber*600)*60+cursec;
    qDebug()<<"last_time"<<last_time;
    //on_startstop_slot();
}
void QBStopWatch::InitData()
{
    m_startResetBtnWidth = STARTICONSIZE;
    m_lcdNumberHeight    = LCDNUMBERHEIGHT;
    m_currentCriqueWidth = CIRQUEWIDTH;
    m_currentFontsize    = FONTPOINTSIZE;
    m_magnifyRadius      = CIRQUERADIUS;
    m_smallCriqueWidth   = SMALLCIRQUEWIDTH;
    m_currentBtnSize     = SWITCHBUTTONSIZE;
    m_bIstouchBegin      = false;
    m_bIsRightBtn        = true;
    m_startResetPressed  = 0;
    m_bIsLeft            = 0;
    m_centerPoint        = QPoint(QApplication::desktop()->width()/2,QApplication::desktop()->height()/2);
    //进度圆环移动
    m_pCirque->setParam(m_smallCriqueWidth,m_magnifyRadius-m_currentCriqueWidth/2+m_smallCriqueWidth/2);
    m_pCirque->move(m_centerPoint.rx()-(m_magnifyRadius-m_currentCriqueWidth/2+m_smallCriqueWidth/2),m_centerPoint.ry()-(m_magnifyRadius-m_currentCriqueWidth/2+m_smallCriqueWidth/2));
    update();
    ScaledIcon();
    on_reset_slot();
    update();
}

void QBStopWatch::ScaledIcon()
{
    float scaled = m_magnifyRadius/CIRQUERADIUS;
    float width = BELLICONSIZE*scaled;
    float height = BELLICONSIZE*scaled;
    //LCD数字高度放大
    m_lcdNumber->setFixedHeight(m_lcdNumberHeight*scaled);
    //调节图标放大
    m_sedOneUp->setIconSize(QSize(width,width));
    m_sedOneDown->setIconSize(QSize(width,height));
    m_sedTwoUp->setIconSize(QSize(width,width));
    m_sedTwoDown->setIconSize(QSize(width,width));
    m_minOneUp->setIconSize(QSize(width,width));
    m_minOneDown->setIconSize(QSize(width,width));
    m_minTwoUp->setIconSize(QSize(width,width));
    m_minTwoDown->setIconSize(QSize(width,width));
    //闹铃图标放大
    m_clockBell->update();
}

QToolButton* QBStopWatch::CreatecommonBtn(const QIcon &icon,QSize size)
{
    QToolButton* tempBtn = new QToolButton(this);
    tempBtn->setIconSize(size);
    tempBtn->setIcon(icon);
    tempBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    tempBtn->setStyleSheet("background-repeat: no-repeat;background-position:center;border:none;}");
    return tempBtn;
}

void QBStopWatch::on_startstop_slot()
{
    if(m_bIsStart)//暂停
    {
        m_timer->stop();
        m_bellplay->stop();
        m_bellTimer->stop();
        sectimer->stop();
        hideTimeButton(false);
        finishbutton->setVisible(true);
    }
    else//开始
    {
        if(!m_bIsRightBtn && m_totalsed == 0)
            return;
        m_timer->start();
        sectimer->start();
        hideTimeButton(false);
        finishbutton->setVisible(false);
        qDebug()<<"start time"<<QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    }
    m_bIsStart = !m_bIsStart;
}

void QBStopWatch::on_reset_slot()
{
    m_sedOneNumber = 0;
    m_sedTwoNumber = 0;
    m_minOneNumber = 0;
    m_minTwoNumber = 0;
    m_totalsed = 0;
    m_lcdNumber->display("00:00");
    m_pCirque->setPercent(0);
    m_finishsed = 0;
    m_bIsStart = false;
    m_bIsLeft = 0;
    cursec=0;
    m_timer->stop();
    m_bellTimer->stop();
    m_bellplay->stop();

    //倒计时 默认一分钟
    if(!m_bIsRightBtn)
    {
        m_minOneNumber = 1;
        m_totalsed = 60;
        m_lcdNumber->display("01:00");
    }
    hideTimeButton(false);
}

void QBStopWatch::on_rightRadio_slot(bool)
{
    m_bIsRightBtn = true;
    m_pCirque->setParam(QColor(44, 118, 255),true);
    on_reset_slot();
}

void QBStopWatch::on_leftRadio_slot(bool)
{
    m_bIsRightBtn = false;
    m_pCirque->setParam(QColor(44, 118, 255),false);
    on_reset_slot();
}

void QBStopWatch::on_timeBtnGroup_slot(int index)
{
    if(m_bIsStart)
        return;
    switch (index) {
    case 0:
        if(m_sedOneNumber == 9)
        {
            m_sedOneNumber = 0;
        }
        else
        {
            m_sedOneNumber++;
        }
        break;
    case 1:
        if(m_sedOneNumber == 0)
        {
            m_sedOneNumber = 9;
        }
        else
        {
            m_sedOneNumber--;
        }
        break;
    case 2:
        if(m_sedTwoNumber == 5)
        {
            m_sedTwoNumber = 0;
        }
        else
        {
            m_sedTwoNumber++;
        }
        break;
    case 3:
        if(m_sedTwoNumber == 0)
        {
            m_sedTwoNumber = 5;
        }
        else
        {
            m_sedTwoNumber--;
        }
        break;
    case 4:
        if(m_minOneNumber == 9)
        {
            m_minOneNumber = 0;
        }
        else
        {
            m_minOneNumber++;
        }
        break;
    case 5:
        if(m_minOneNumber == 0)
        {
            m_minOneNumber = 9;
        }
        else
            m_minOneNumber--;
        break;
    case 6:
        if(m_minTwoNumber == 5)
            m_minTwoNumber = 0;
        else
            m_minTwoNumber++;
        break;
    case 7:
        if(m_minTwoNumber == 0)
            m_minTwoNumber = 5;
        else
            m_minTwoNumber--;
        break;
    }
    QString clockTime = QString("%1%2:%3%4").arg(m_minTwoNumber)
            .arg(m_minOneNumber).arg(m_sedTwoNumber).arg(m_sedOneNumber);
    m_lcdNumber->display(clockTime);
    m_totalsed = m_sedOneNumber+m_sedTwoNumber*10+m_minOneNumber*60+m_minTwoNumber*60*10;
}

void QBStopWatch::on_timeout_slot()
{
    m_finishsed++;
    if(m_bIsRightBtn)
    {
        m_totalsed = 60;
        if(m_finishsed == m_totalsed)
        {
            timeUpadata(0);//加一秒
            m_finishsed = 0;
        }
        m_pCirque->setPercent(m_finishsed*100/m_totalsed);
    }
    else
    {
        timeUpadata(1);//减一秒
        m_pCirque->setPercent(m_finishsed*100/m_totalsed);
        if(m_finishsed == m_totalsed)
        {
            m_bellTimer->start();
            m_bellplay->play();
            on_reset_slot();
        }
    }
}

void QBStopWatch::on_belltimeout_slot()
{
    if(m_bIsLeft == 0)
        m_bIsLeft = 1;
    else if(m_bIsLeft == 1)
        m_bIsLeft = 2;
    else if(m_bIsLeft == 2)
        m_bIsLeft = 1;
    m_clockBell->update();
}

void QBStopWatch::on_timerButton_slot()
{
    m_timerButton->stop();
    m_startResetPressed = 0;
    update();
}

void QBStopWatch::on_close_slot()
{
    QApplication::quit();
}

void QBStopWatch::gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QColor color)
{
    float rx = radius-m_currentCriqueWidth/2;
    float yx = radius-m_currentCriqueWidth/2;
    painter->setBrush(QColor(255,255,255));
    painter->drawEllipse(QRectF(0 - rx, 0 - yx, 2 * rx, 2 * yx));
    //painter->drawEllipse(QPoint(0,0),radius-m_currentCriqueWidth/2,radius-m_currentCriqueWidth/2);
    //绘制外层圆环
    QRadialGradient gradient(0, 0, radius);

    gradient.setColorAt(0.0, color);
    gradient.setColorAt(0.3, QColor(120,120,120));
    gradient.setColorAt(0.5, QColor(255,255,255));
    gradient.setColorAt(0.8, QColor(120,120,120));
    gradient.setColorAt(1.0, QColor(255,255,255));

    painter->setBrush(gradient);
    // << 1（左移1位）相当于radius*2 即：150*2=300
    QRectF rect(-radius, -radius, radius << 1, radius << 1);
    QPainterPath path;
    path.arcTo(rect, startAngle, angleLength);
    QPainterPath subPath;
    subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));
    // path为扇形 subPath为椭圆
    path -= subPath;
    painter->setPen(Qt::NoPen);
    painter->drawPath(path);
    m_criquePath = path;
}

void QBStopWatch::timeUpadata(int index)
{
    switch (index) {
    case 0:
        if(m_sedOneNumber == 9)
        {
            m_sedOneNumber = 0;
            timeUpadata(2);
        }
        else
        {
            m_sedOneNumber++;
        }
        break;
    case 1:
        if(m_sedOneNumber == 0)
        {
            m_sedOneNumber = 9;
            timeUpadata(3);
        }
        else
        {
            m_sedOneNumber--;
        }
        break;
    case 2:
        if(m_sedTwoNumber == 5)
        {
            m_sedTwoNumber = 0;
            timeUpadata(4);
        }
        else
        {
            m_sedTwoNumber++;
        }
        break;
    case 3:
        if(m_sedTwoNumber == 0)
        {
            m_sedTwoNumber = 5;
            timeUpadata(5);
        }
        else
        {
            m_sedTwoNumber--;
        }
        break;
    case 4:
        if(m_minOneNumber == 9)
        {
            m_minOneNumber = 0;
            timeUpadata(6);
        }
        else
        {
            m_minOneNumber++;
        }
        break;
    case 5:
        if(m_minOneNumber == 0)
        {
            m_minOneNumber = 9;
            timeUpadata(7);
        }
        else
            m_minOneNumber--;
        break;
    case 6:
        if(m_minTwoNumber == 5)
            m_minTwoNumber = 0;
        else
            m_minTwoNumber++;
        break;
    case 7:
        if(m_minTwoNumber == 0)
            m_minTwoNumber = 5;
        else
            m_minTwoNumber--;
        break;
    }
    QString clockTime = QString("%1%2:%3%4").arg(m_minTwoNumber)
            .arg(m_minOneNumber).arg(m_sedTwoNumber).arg(m_sedOneNumber);
    m_lcdNumber->display(clockTime);
}

void QBStopWatch::hideTimeButton(bool status)
{
    m_sedOneUp->setVisible(status);
    m_sedOneDown->setVisible(status);
    m_sedTwoUp->setVisible(status);
    m_sedTwoDown->setVisible(status);
    m_minOneUp->setVisible(status);
    m_minOneDown->setVisible(status);
    m_minTwoUp->setVisible(status);
    m_minTwoDown->setVisible(status);
}

void QBStopWatch::paintEvent(QPaintEvent *)
{   
    QPainter painter(this);
#ifdef Q_OS_MACX
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(this->rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
#endif
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.translate(m_centerPoint);
    //绘制开始重置按钮
    QSvgRenderer startPic;
    startPic.load(QString(":/images/start2.svg"));
    if(m_startResetPressed == 1)
        startPic.render(&painter ,QRectF(-m_startResetBtnWidth/2,-m_magnifyRadius-m_startResetBtnWidth*0.5,m_startResetBtnWidth ,m_startResetBtnWidth));
    else
        startPic.render(&painter ,QRectF(-m_startResetBtnWidth/2,-m_magnifyRadius-m_startResetBtnWidth*0.75,m_startResetBtnWidth,m_startResetBtnWidth));
    QSvgRenderer resetPic;
    resetPic.load(QString(":/images/reset.svg"));
    if(m_startResetPressed == 2)
        resetPic.render(&painter ,QRectF(m_magnifyRadius*0.6-m_startResetBtnWidth/5,-m_magnifyRadius*0.95+m_startResetBtnWidth/5,m_startResetBtnWidth,m_startResetBtnWidth));
    else
        resetPic.render(&painter ,QRectF(m_magnifyRadius*0.6,-m_magnifyRadius*0.95,m_startResetBtnWidth,m_startResetBtnWidth));
    //绘制背景
    gradientArc(&painter, m_magnifyRadius, 0,  360, m_currentCriqueWidth, QColor(43,43,43));//背景圆环
    //缩放按钮图标以及LCD
    //ScaledIcon();
    //计时器布局移动
    m_mainWid->resize((m_magnifyRadius-m_currentCriqueWidth-m_currentFontsize)*2,m_magnifyRadius*2*0.70);
    m_mainWid->move(m_centerPoint.rx()-m_magnifyRadius+m_currentCriqueWidth+m_currentFontsize,m_centerPoint.ry()-m_magnifyRadius*0.70);
    //关闭按钮移动
    m_closeBtn->move(m_centerPoint.rx()+m_magnifyRadius/2,m_centerPoint.ry()-m_magnifyRadius-m_startResetBtnWidth/2);

    /*QSvgRenderer upPic;//正计按钮
    if(m_bIsRightBtn)
        upPic.load(QString(":/images/up2.svg"));
    else
        upPic.load(QString(":/images/up1.svg"));
    upPic.render(&painter ,QRectF(+m_currentBtnSize,-m_magnifyRadius*0.75,m_currentBtnSize ,m_currentBtnSize));

    QSvgRenderer downPic;//倒计按钮
    if(m_bIsRightBtn)
        downPic.load(QString(":/images/down1.svg"));
    else
        downPic.load(QString(":/images/down2.svg"));
    downPic.render(&painter ,QRectF(-m_currentBtnSize-m_currentBtnSize,-m_magnifyRadius*0.75,m_currentBtnSize ,m_currentBtnSize));*/
}

void QBStopWatch::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_point = event->pos();
        //开始 复位 正计 倒计按钮响应
        QRect rectStart(m_centerPoint.rx()-m_startResetBtnWidth/2,m_centerPoint.ry()-m_magnifyRadius-m_startResetBtnWidth*0.75,m_startResetBtnWidth,m_startResetBtnWidth*0.75);
        if(rectStart.contains(m_point))
        {
            m_timerButton->start();
            m_startResetPressed = 1;
            update();
            on_startstop_slot();
        }
        QRect rectReset(m_centerPoint.rx()+m_magnifyRadius*0.6,m_centerPoint.ry()-m_magnifyRadius*0.95,m_startResetBtnWidth,m_startResetBtnWidth*0.75);
        if(rectReset.contains(m_point))
        {
            m_timerButton->start();
            m_startResetPressed = 2;
            update();
            on_reset_slot();
        }
        QRectF rectUp(m_centerPoint.rx()+m_currentBtnSize,m_centerPoint.ry()-m_magnifyRadius*0.75,m_currentBtnSize ,m_currentBtnSize);
        if(rectUp.contains(m_point))
            on_rightRadio_slot(true);

        QRectF rectDown(m_centerPoint.rx()-m_currentBtnSize-m_currentBtnSize,m_centerPoint.ry()-m_magnifyRadius*0.75,m_currentBtnSize ,m_currentBtnSize);
        if(rectDown.contains(m_point))
            on_leftRadio_slot(true);
    }
}

void QBStopWatch::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bIstouchBegin || m_startResetPressed != 0)//大屏手势处理
        return;
    if (m_bPressed)
    {
        if(m_criquePath.contains(m_point-m_centerPoint))//放大
        {
            QPoint point = event->pos() - m_centerPoint;
            int currentRadiu = sqrt(qPow(point.rx(),2) + qPow(point.ry(),2)) + m_currentCriqueWidth/2;

            //缩放最大最小限制
            int windWidth = QApplication::desktop()->width()/2;
            int windHeight = QApplication::desktop()->height()/2;
            if(currentRadiu > windWidth || currentRadiu > windHeight || currentRadiu < CIRQUERADIUS)
                return;
            m_magnifyRadius = currentRadiu;
            m_point = event->pos();

            m_currentFontsize    = FONTPOINTSIZE*m_magnifyRadius/CIRQUERADIUS;
            m_currentCriqueWidth = CIRQUEWIDTH*m_magnifyRadius/CIRQUERADIUS;
            m_smallCriqueWidth   = SMALLCIRQUEWIDTH*m_magnifyRadius/CIRQUERADIUS;
            m_startResetBtnWidth = STARTICONSIZE*m_magnifyRadius/CIRQUERADIUS;
            m_currentBtnSize     = SWITCHBUTTONSIZE*m_magnifyRadius/CIRQUERADIUS;
            ScaledIcon();
        }
        else//移动
        {
            int x = event->pos().rx()-m_point.rx();
            int y = event->pos().ry()-m_point.ry();
            if(((m_centerPoint.rx()-m_magnifyRadius+x) < -m_magnifyRadius)
                    || ((m_centerPoint.rx()+m_magnifyRadius+x) > (QApplication::desktop()->width()+m_magnifyRadius))
                    || ((m_centerPoint.ry()-m_magnifyRadius+y) < -m_magnifyRadius)
                    || ((m_centerPoint.ry()+m_magnifyRadius+y) > (QApplication::desktop()->height()+m_magnifyRadius)))
                m_point = event->pos();
            else
            {
                m_centerPoint = QPoint(m_centerPoint.rx()+x,m_centerPoint.ry()+y);
                m_point = event->pos();
            }
        }
        //进度圆环移动
        m_pCirque->setParam(m_smallCriqueWidth,m_magnifyRadius-m_currentCriqueWidth/2+m_smallCriqueWidth/2);
        m_pCirque->move(m_centerPoint.rx()-(m_magnifyRadius-m_currentCriqueWidth/2+m_smallCriqueWidth/2),m_centerPoint.ry()-(m_magnifyRadius-m_currentCriqueWidth/2+m_smallCriqueWidth/2));
        update();
    }
}

void QBStopWatch::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bPressed = false;
}

bool QBStopWatch::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_clockBell && event->type() == QEvent::Paint)
    {
        QPainter painter(m_clockBell);
        QTransform tr;
        if(m_bIsLeft == 0)
            tr.rotate(0);
        else if(m_bIsLeft == 1)
            tr.rotate(5);
        else if(m_bIsLeft == 2)
            tr.rotate(-5);
        float scaled = m_magnifyRadius/CIRQUERADIUS;
        float width = BELLICONSIZE*scaled;
        float height = BELLICONSIZE*scaled;
        m_clockBell->setFixedSize(QSize(width,height));
        /*QSvgRenderer m_svgRender;
        m_svgRender.load(QString(":/images/bell.svg"));
        m_svgRender.render(&painter ,QRectF(0,0,width ,height));*/
    }
    return QWidget::eventFilter(obj,event);
}

//手势放大缩小事件处理
bool QBStopWatch::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::Gesture:
    {
        QGestureEvent* gesture_event = static_cast<QGestureEvent*>(event);
        QGesture* gesture = NULL;

        gesture = gesture_event->gesture(Qt::PinchGesture);
        QPinchGesture* pinch_gesture = dynamic_cast<QPinchGesture*>(gesture);
        if(pinch_gesture != NULL)
        {
            QRect rect(QPoint(m_centerPoint.rx()-m_magnifyRadius,m_centerPoint.ry()-m_magnifyRadius),QSize(m_magnifyRadius*2,m_magnifyRadius*2));
            if(rect.contains(QCursor::pos()))
            {
                switch(pinch_gesture->state())
                {
                case Qt::GestureStarted:
                {
                    m_bIstouchBegin = true;
                }
                    break;
                case Qt::GestureUpdated:
                {
                    int windHeight = QApplication::desktop()->height()/2;
                    int offset = (int)((qreal)m_magnifyRadius * (pinch_gesture->scaleFactor() -1));
                    if(offset < 0 && m_magnifyRadius + offset >= CIRQUERADIUS)//缩小
                    {
                        m_magnifyRadius += offset;
                    }
                    else if(offset > 0 && m_magnifyRadius + offset <= windHeight)//变大
                    {
                        m_magnifyRadius += offset;
                    }
                    m_currentFontsize    = FONTPOINTSIZE*m_magnifyRadius/CIRQUERADIUS;
                    m_currentCriqueWidth = CIRQUEWIDTH*m_magnifyRadius/CIRQUERADIUS;
                    m_smallCriqueWidth   = SMALLCIRQUEWIDTH*m_magnifyRadius/CIRQUERADIUS;
                    m_startResetBtnWidth = STARTICONSIZE*m_magnifyRadius/CIRQUERADIUS;
                    m_currentBtnSize     = SWITCHBUTTONSIZE*m_magnifyRadius/CIRQUERADIUS;
                    ScaledIcon();
                    //进度圆环移动
                    m_pCirque->setParam(m_smallCriqueWidth,m_magnifyRadius-m_currentCriqueWidth/2+m_smallCriqueWidth/2);
                    m_pCirque->move(m_centerPoint.rx()-(m_magnifyRadius-m_currentCriqueWidth/2+m_smallCriqueWidth/2),m_centerPoint.ry()-(m_magnifyRadius-m_currentCriqueWidth/2+m_smallCriqueWidth/2));
                    update();
                }
                    break;
                case Qt::GestureFinished:
                {
                    m_bIstouchBegin = false;
                }
                    break;
                }
                return true;
            }
        }
        break;
    }
    }
    return QWidget::event(event);
}
