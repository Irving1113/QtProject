#ifndef QBStopWatch_H
#define QBStopWatch_H

#include <QWidget>
#include <QLCDNumber>
#include <QButtonGroup>
#include <QTimer>
#include <QSound>
#include <QLabel>
#include <QToolButton>
#include "qbcirque.h"

class QBTcpSocket;
class QBStopWatch : public QWidget
{
    Q_OBJECT
public:
    explicit QBStopWatch(QWidget *parent = 0);
    void CreateUI();
    void InitData();
    void ScaledIcon();
    QToolButton* CreatecommonBtn(const QIcon &icon,QSize size);
    void gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QColor color);
    void timeUpadata(int);
    void hideTimeButton(bool);

public slots:
    void on_startstop_slot();
    void on_reset_slot();
    void on_rightRadio_slot(bool);
    void on_leftRadio_slot(bool);
    void on_timeBtnGroup_slot(int);
    void on_timeout_slot();
    void on_belltimeout_slot();
    void on_timerButton_slot();
    void on_close_slot();
    void on_finish_slot();
    void on_sec_timeout();
protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual bool event(QEvent *event);

private:
    QWidget*        m_mainWid;
    bool            m_bPressed;
    QPoint          m_point;
    QBCirque*       m_pCirque;

    QToolButton*   m_startstopBtn;
    QToolButton*   m_resetBtn;
    bool            m_bIsRightBtn; //默认正计
    QLCDNumber*     m_lcdNumber;   //时间显示

    QToolButton*   m_sedOneUp;    //秒调整按钮
    QToolButton*   m_sedOneDown;
    QToolButton*   m_sedTwoUp;
    QToolButton*   m_sedTwoDown;

    QToolButton*   m_minOneUp;     //分调整按钮
    QToolButton*   m_minOneDown;
    QToolButton*   m_minTwoUp;
    QToolButton*   m_minTwoDown;

    QToolButton* finishbutton;

    QButtonGroup*   m_timeBtnGroup;

    QTimer*         m_bellTimer;     //闹铃特效定时器
    QPixmap         m_bellPix;       //闹铃背景
    QLabel*         m_clockBell;     //闹铃图标
    QSound*         m_bellplay;      //铃声播放
    int             m_bIsLeft;

    int             m_sedOneNumber;  //秒个位
    int             m_sedTwoNumber;  //秒十位
    int             m_minOneNumber;  //分个位
    int             m_minTwoNumber;  //分十位
    int             cursec;
    QTimer*         m_timer;
    int             m_totalsed;      //倒计时总时长
    int             m_finishsed;     //完成时长
    bool            m_bIsStart;      //开始
    bool            m_bIstouchBegin; //是否手势缩放
    QToolButton*    m_closeBtn;

    QTimer*         sectimer;

    QPoint               m_centerPoint;
    QPainterPath         m_criquePath;
    float                m_magnifyRadius;          //放大的半径
    float                m_currentCriqueWidth;     //当前圆环宽度
    float                m_currentFontsize;        //当前时钟字体大小
    float                m_smallCriqueWidth;       //当前进度圆环的宽度
    float                m_startResetBtnWidth;     //开始重置按钮大小宽
    float                m_lcdNumberHeight;        //LCD数字初始高度
    float                m_currentBtnSize;         //当前按钮大小
    int                  m_startResetPressed;      //0--都没按下 1--开始按下  2--重置按下
    QTimer*              m_timerButton;
};

#endif // QBStopWatch_H
