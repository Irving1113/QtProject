/****************************************************************************
**Reference: Qt/Legendmarkers Example
****************************************************************************/

#include "mainwidget.h"
#include "database.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtCore/QDebug>
#include <QtCharts/QLegend>
#include <QtWidgets/QFormLayout>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QLineSeries>
#include <QtCharts/QXYLegendMarker>
#include <QtCore/QtMath>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCharts/QValueAxis>
#include <QtMath>

QT_USE_NAMESPACE
QT_CHARTS_USE_NAMESPACE

MainWidget::MainWidget(QWidget *parent, QMap<QString, QVector<int> > series, QVector<QDateTime> days) :
    QWidget(parent)
{
    n = days.count();
    int maxval = 0;
    for (auto line: series.values()) {
        for (auto t: line)
            maxval = qMax(maxval, t);
    }
    // Create chart view
    m_chart = new QChart();
    m_chartView = new QChartView(m_chart, this);

    m_mainLayout = new QGridLayout();
    m_mainLayout->addWidget(m_chartView, 0, 1, 3, 1);
    setLayout(m_mainLayout);

    // Set the axises
    axisX = new QDateTimeAxis;
    axisX->setTickCount(days.count());
    axisX->setFormat("M月d日");
    m_chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("持续时长/min");
    axisY->setRange(0, maxval);
    m_chart->addAxis(axisY, Qt::AlignLeft);

    for (auto it = series.begin(); it != series.end(); it++)
        addSeries(it.key(), it.value(), days);

    connectMarkers();

    // Set the title and show legend
    m_chart->setTitle("你在过去的" + QString::number(days.count(), 10) + "天中......");
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);

    m_chartView->setRenderHint(QPainter::Antialiasing);
}

void MainWidget::addSeries(QString set_name, QVector<int> set_data, QVector<QDateTime> days)
{
    QLineSeries *series = new QLineSeries();
    m_series.append(series);

    series->setName(set_name);

    for (int i = 0; i < set_data.count(); i++)
        series->append(days[i].toMSecsSinceEpoch(), set_data[i]);

    m_chart->addSeries(series);

    series->attachAxis(axisX);
    series->attachAxis(axisY);
}

void MainWidget::removeSeries()
{
    // Remove last series from chart
    if (m_series.count() > 0) {
        QLineSeries *series = m_series.last();
        m_chart->removeSeries(series);
        m_series.removeLast();
        delete series;
    }
}

void MainWidget::connectMarkers()
{
    // Connect all markers to handler
    const auto markers = m_chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &MainWidget::handleMarkerClicked);
        QObject::connect(marker, &QLegendMarker::clicked, this, &MainWidget::handleMarkerClicked);
    }
}

void MainWidget::disconnectMarkers()
{
    const auto markers = m_chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &MainWidget::handleMarkerClicked);
    }
}

void MainWidget::mousePressEvent(QMouseEvent *ev) {
    if (ev->button() == Qt::LeftButton) {
        int x = ev->x();
        int y = ev->y();
        double sep = (640 - 105) / (n - 1);
        if (y >= 363 && y <= 377 && x >= 80 && x <= 665) {
            int t = floor((x - 80) / sep);
            if ((x - 80 - t * sep) <= 50)
                qDebug() << "!!!" << t;     //待加入界面4的接口
        }
    }
}

void MainWidget::handleMarkerClicked()
{
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);
    switch (marker->type())
    {
    case QLegendMarker::LegendMarkerTypeXY:
        {
        // Toggle visibility of series
        marker->series()->setVisible(!marker->series()->isVisible());
        marker->setVisible(true);
        qreal alpha = 1.0;

        if (!marker->series()->isVisible())
            alpha = 0.5;

        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);
        break;
        }
    default:
        {
        qDebug() << "Unknown marker type";
        break;
        }
    }
}
