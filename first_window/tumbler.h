#ifndef TUMBLER_H
#define TUMBLER_H


#include <QWidget>

#ifdef quc
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
#include <QtDesigner/QDesignerExportWidget>
#else
#include <QtUiPlugin/QDesignerExportWidget>
#endif

class QDESIGNER_WIDGET_EXPORT Tumbler : public QWidget
#else
class Tumbler : public QWidget
#endif

{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ getCurrentIndex WRITE setCurrentIndex)
    Q_PROPERTY(QString currentValue READ getCurrentValue WRITE setCurrentValue)
    Q_PROPERTY(bool horizontal READ getHorizontal WRITE setHorizontal)
    Q_PROPERTY(bool clickEnable READ getClickEnable WRITE setClickEnable)

    Q_PROPERTY(QColor foreground READ getForeground WRITE setForeground)
    Q_PROPERTY(QColor background READ getBackground WRITE setBackground)
    Q_PROPERTY(QColor lineColor READ getLineColor WRITE setLineColor)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)

public:
    explicit Tumbler(QWidget *parent = 0);

protected:
    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void drawBg(QPainter *painter);
    void drawLine(QPainter *painter);
    void drawText(QPainter *painter, int index, int offset);

private:
    QStringList listValue;          //值队列
    int currentIndex;               //当前索引
    QString currentValue;           //当前值
    bool horizontal;                //是否横向显示
    bool clickEnable;               //是否启用单击选择

    QColor foreground;              //前景色
    QColor background;              //背景色
    QColor lineColor;               //线条颜色
    QColor textColor;               //当前文本颜色

    int percent;                    //比例
    int offset;                     //偏离值
    bool pressed;                   //鼠标是否按下
    int pressedPos;                 //按下处坐标
    int releasePos;                 //松开处坐标
    int currentPos;                 //当前值对应起始坐标

public:
    QStringList getListValue()      const;
    int getCurrentIndex()           const;
    QString getCurrentValue()       const;
    bool getHorizontal()            const;
    bool getClickEnable()           const;

    QColor getForeground()          const;
    QColor getBackground()          const;
    QColor getLineColor()           const;
    QColor getTextColor()           const;

    QSize sizeHint()                const;
    QSize minimumSizeHint()         const;

public Q_SLOTS:
    //设置值队列
    void setListValue(const QStringList &listValue);
    //设置当前索引
    void setCurrentIndex(int currentIndex);
    //设置当前值
    void setCurrentValue(const QString &currentValue);
    //设置横向显示,如果为假则纵向显示
    void setHorizontal(bool horizontal);
    //设置是否启用单击
    void setClickEnable(bool clickEnable);

    //设置前景色
    void setForeground(const QColor &foreground);
    //设置背景色
    void setBackground(const QColor &background);
    //设置线条颜色
    void setLineColor(const QColor &lineColor);
    //设置文本颜色
    void setTextColor(const QColor &textColor);

Q_SIGNALS:
    void clicked();
    void currentIndexChanged(int currentIndex);
    void currentValueChanged(QString currentValue);
};

#endif
