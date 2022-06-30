#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QPixmap>
#include <QDesktopServices>
#include <QUrl>
#include <qlabel.h>
#include <QDialog>
#include <QPaintEvent>
#include "alter.h"
#include"qbstopwatch.h"
#include"third_widget.h"
#include<new_page.h>
namespace Ui {
class widget;
}

class widget : public QWidget
{
    Q_OBJECT

public:
    explicit widget(QWidget *parent = 0);
    ~widget();
    bool sonstate;
    QRect m_endRect;
    alter*first;
    QBStopWatch* p1;
    third_widget*p3;
    new_page* p4;
private:
    Ui::widget *ui;
    QPoint relativePos;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void openweb();
signals:
    void signal1_2(Work);
    void signal2_1(int);
    void signal2_3(Work);
    void signal3_1();
    void signal3_2();
    void signal4_1(int);
private slots:
    void show2(Work a);
    void reshow(int);
    void initial3(Work);
    void reset_all();
    void reshow2();
};

#endif // WIDGET_H
