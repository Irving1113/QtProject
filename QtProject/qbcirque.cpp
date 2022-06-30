/*
 * 此文件参考了部分开源代码
 */

#include "qbcirque.h"
#include <QPainter>
#include <QDebug>

QBCirque::QBCirque(QWidget *parent)
    : QWidget(parent)
{
    m_nCirqueWidth = 50;
    m_nTotalWidth = 200;
    m_backColor = QColor(255, 0, 0);
    m_mainColor = QColor(0, 255, 0);
    m_percent = 0.0;
    m_rotateDirection = true;
    this->setFixedSize(m_nTotalWidth*2, m_nTotalWidth*2);
#ifdef Q_OS_MACX
#include "../TSCommon/MacToolHelper.h"
    setMacHasShadow(this, false);
#endif
}

QBCirque::QBCirque(int nCirqueWidth, int nCircleWidth, QColor colorCirque, QColor colorBack,QWidget *parent)
    : QWidget(parent)
{
    m_nCirqueWidth = nCirqueWidth;
    m_nTotalWidth = nCircleWidth;
    m_backColor = colorBack;
    m_mainColor = colorCirque;
    m_percent = 0.0;
    m_rotateDirection = true;
    this->setFixedSize(m_nTotalWidth*2, m_nTotalWidth*2);
#ifdef Q_OS_MACX
#include "../TSCommon/MacToolHelper.h"
    setMacHasShadow(this, false);
#endif
}


void QBCirque::gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QColor color)
{
    // 渐变色
    QRadialGradient gradient(0, 0, radius);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1.0, color);
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
}

void QBCirque::setPercent(double percent)
{
    m_percent = percent/100;
    this->update();
}

double QBCirque::getPercent()
{
    return m_percent*100;
}

void QBCirque::setParam(QColor color, bool rotate)
{
    m_mainColor = color;
    m_rotateDirection = rotate;
    update();
}

void QBCirque::setParam(int nCirqueWidth, int nCircleWidth)
{
    m_nCirqueWidth = nCirqueWidth;
    m_nTotalWidth = nCircleWidth;
    this->setFixedSize(m_nTotalWidth*2, m_nTotalWidth*2);
    update();
}


void QBCirque::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
#ifdef Q_OS_MACX
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(this->rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
#endif
    painter.setRenderHint(QPainter::Antialiasing, true);
    // >> 1（右移1位）相当于width() / 2
    painter.translate(width() >> 1, height() >> 1);
    double startAngle = 90;
    double sweepLength = 360 * m_percent;
    /**
         * 参数二：半径
         * 参数三：开始的角度
         * 参数四：指扫取的角度-顺时针（360度 / 8 = 45度）
         * 参数五：圆环的高度
         * 参数六：填充色
        **/
    gradientArc(&painter, m_nTotalWidth, 0,  360, m_nCirqueWidth, m_backColor);//背景圆环
    if(m_rotateDirection)
        gradientArc(&painter, m_nTotalWidth, startAngle, -sweepLength, m_nCirqueWidth, m_mainColor);//进度圆环
    else
        gradientArc(&painter, m_nTotalWidth, startAngle, sweepLength, m_nCirqueWidth, m_mainColor);//进度圆环
}
