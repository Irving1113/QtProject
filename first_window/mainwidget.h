/****************************************************************************
**Reference: Qt/Legendmarkers Example
****************************************************************************/

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGraphicsGridLayout>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtCharts/QLineSeries>
#include <QDateTime>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>

QT_USE_NAMESPACE
QT_CHARTS_USE_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0,
                            QMap<QString, QVector<int> > series = QMap<QString, QVector<int> >(),
                            QVector<QDateTime> days = QVector<QDateTime>());

public slots:
    void addSeries(QString set_name, QVector<int> set_data, QVector<QDateTime> days);
    void removeSeries();
    void connectMarkers();
    void disconnectMarkers();

    void handleMarkerClicked();
    void mousePressEvent(QMouseEvent *ev);

private:

    QChart *m_chart;
    QList<QLineSeries *> m_series;

    QChartView *m_chartView;
    QGridLayout *m_mainLayout;
    QGridLayout *m_fontLayout;
    QDateTimeAxis * axisX;
    QValueAxis * axisY;
    int n;

};

#endif // MAINWIDGET_H
