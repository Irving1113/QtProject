QT       += core gui
QT       += core gui sql
QT       +=sql
QT       += core gui svg multimedia network
QT       += charts widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    alter.cpp \
    main.cpp \
    widget.cpp \
    qbcirque.cpp \
    qbstopwatch.cpp \
    third_widget.cpp \
    tumblerdatetime.cpp \
    tumbler.cpp \
    myfloat.cpp \
    new_page.cpp \
    pieview.cpp \
    mainwidget.cpp \
    database.cpp

HEADERS += \
    alter.h \
    widget.h \
    qbcirque.h \
    qbstopwatch.h \
    third_widget.h \
    tumblerdatetime.h \
    tumbler.h \
    new_page.h \
    myfloat.h \
    pieview.h \
    mainwidget.h \
    database.h

FORMS += \
    alter.ui \
    widget.ui \
    third_widget.ui \
    myfloat.ui \
    new_page.ui \
    mainwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
