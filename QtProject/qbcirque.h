#ifndef QBCIRQUE_H
#define QBCIRQUE_H

#include <QWidget>

class QBCirque : public QWidget
{
    Q_OBJECT
public:
    explicit QBCirque(QWidget *parent = 0);
    QBCirque(int nCirqueWidth, int nCircleWidth, QColor colorCirque=QColor(0, 255, 0), QColor colorBack=QColor(255, 0, 0),QWidget *parent = 0);
    void gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QColor color);
    void setPercent(double percent);
    double getPercent();
    void setParam(QColor color,bool rotate=true);
    void setParam(int nCirqueWidth, int nCircleWidth);
protected:
    virtual void paintEvent(QPaintEvent*);
private:
    int    m_nCirqueWidth;    //圆环半径
    int    m_nTotalWidth;     //圆总半径
    QColor m_mainColor;       //圆环进度颜色
    QColor m_backColor;       //圆环背景颜色
    double m_percent;         //百分比值
    bool   m_rotateDirection; //进度方向 默认顺时针
};

#endif
