#ifndef TUMBLERDATETIME_H
#define TUMBLERDATETIME_H

/**
 * 日期时间滑动选择器 作者:feiyangqingyun(QQ:517216493) 2017-8-11
 * 1:可设置年月日时分秒
 * 2:可鼠标或者手指滑动选择年月日时分秒
 * 3:支持自定义数值范围
 * 4:支持鼠标滚轮选择
 * 5:年月日自动联动计算
 */

#include <QWidget>
#include <QDateTime>
#include<QPushButton>
class QTimer;
class Tumbler;
class TumblerDateTime;

#ifdef quc
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
#include <QtDesigner/QDesignerExportWidget>
#else
#include <QtUiPlugin/QDesignerExportWidget>
#endif

class QDESIGNER_WIDGET_EXPORT TumblerDateTime : public QWidget
#else
class TumblerDateTime : public QWidget
#endif

{
    Q_OBJECT
    Q_PROPERTY(QColor foreground READ getForeground WRITE setForeground)
    Q_PROPERTY(QColor background READ getBackground WRITE setBackground)
    Q_PROPERTY(QColor lineColor READ getLineColor WRITE setLineColor)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
    Q_PROPERTY(bool clickEnable READ getClickEnable WRITE setClickEnable)

    Q_PROPERTY(bool showDateTime READ getShowDateTime WRITE setShowDateTime)
    Q_PROPERTY(bool writeDateTime READ getWriteDateTime WRITE setWriteDateTime)
    Q_PROPERTY(QDateTime dateTime READ getDateTime WRITE setDateTime)

public:
    explicit TumblerDateTime(QWidget *parent = 0);
    ~TumblerDateTime();

private:
    QColor foreground;              //前景色
    QColor background;              //背景色
    QColor lineColor;               //线条颜色
    QColor textColor;               //当前文本颜色
    bool clickEnable;               //是否启用单击选择

    bool showDateTime;              //显示日期时间
    bool writeDateTime;             //写入日期时间
    QDateTime dateTime;             //日期时间

    Tumbler *tumblerYear;           //年份选择器
    Tumbler *tumblerMonth;          //月份选择器
    Tumbler *tumblerDay;            //日期选择器
    Tumbler *tumblerHour;           //时钟选择器
    Tumbler *tumblerMin;            //分钟选择器
    Tumbler *tumblerSec;            //秒钟选择器

    bool isTimer;                   //是否定时器触发
    QTimer *timer;                  //显示时间定时器

private slots:
    void initWidget();
    void initStyle();
    void doTimer();
    void clicked();
    void currentIndexChanged();
    void currentValueChanged(const QString &);

    void confirmclick();//点击确定
signals:
    void sendsignal(int);//窗口返回信号

public:
    QColor getForeground()          const;
    QColor getBackground()          const;
    QColor getLineColor()           const;
    QColor getTextColor()           const;
    bool getClickEnable()           const;

    bool getShowDateTime()          const;
    bool getWriteDateTime()         const;
    QDateTime getDateTime()         const;

    QSize sizeHint()                const;
    QSize minimumSizeHint()         const;

public Q_SLOTS:
    //设置前景色
    void setForeground(const QColor &foreground);
    //设置背景色
    void setBackground(const QColor &background);
    //设置线条颜色
    void setLineColor(const QColor &lineColor);
    //设置文本颜色
    void setTextColor(const QColor &textColor);
    //设置是否启用单击
    void setClickEnable(bool clickEnable);

    //设置显示日期时间
    void setShowDateTime(bool showDateTime);
    //设置写入日期时间
    void setWriteDateTime(bool writeDateTime);

    //设置日期时间
    void setDateTime(const QDateTime &dateTime);
    void setDateTime(const QString &hour, const QString &min);

    //设置系统时间
    void setSystemDateTime(const QString &year, const QString &month, const QString &day,
                           const QString &hour, const QString &min, const QString &sec);

    int getcurtime();//获取当前时间

Q_SIGNALS:
    void currentDateTimeChanged(const QDateTime &dateTime);
};

#endif // TUMBLERDATETIME_H
