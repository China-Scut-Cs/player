/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/


#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QListWidget *listWidget;
    QSlider *positionslider;
    QLabel *time_label;
    QPushButton *btn_playmodel;
    QPushButton *btn_fullscreen;
    QLabel *label;
    QPushButton *btn_open;
    QPushButton *btn_next;
    QPushButton *btn_pre;
    QPushButton *btn_stop;
    QPushButton *btn_play;
    QPushButton *btn_delete;
    QSlider *volume_slider;
    QPushButton *btn_mute;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 600);
        Widget->setMaximumSize(QSize(16777215, 16777215));
        Widget->setLayoutDirection(Qt::LeftToRight);
        listWidget = new QListWidget(Widget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(560, 0, 201, 321));
        positionslider = new QSlider(Widget);
        positionslider->setObjectName(QString::fromUtf8("positionslider"));
        positionslider->setGeometry(QRect(60, 350, 601, 16));
        positionslider->setOrientation(Qt::Horizontal);
        time_label = new QLabel(Widget);
        time_label->setObjectName(QString::fromUtf8("time_label"));
        time_label->setGeometry(QRect(470, 490, 321, 16));
        btn_playmodel = new QPushButton(Widget);
        btn_playmodel->setObjectName(QString::fromUtf8("btn_playmodel"));
        btn_playmodel->setGeometry(QRect(340, 480, 81, 41));
        btn_fullscreen = new QPushButton(Widget);
        btn_fullscreen->setObjectName(QString::fromUtf8("btn_fullscreen"));
        btn_fullscreen->setGeometry(QRect(170, 500, 80, 41));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btn_fullscreen->sizePolicy().hasHeightForWidth());
        btn_fullscreen->setSizePolicy(sizePolicy);
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 30, 471, 291));
        btn_open = new QPushButton(Widget);
        btn_open->setObjectName(QString::fromUtf8("btn_open"));
        btn_open->setGeometry(QRect(21, 401, 80, 20));
        btn_next = new QPushButton(Widget);
        btn_next->setObjectName(QString::fromUtf8("btn_next"));
        btn_next->setGeometry(QRect(220, 401, 33, 20));
        btn_pre = new QPushButton(Widget);
        btn_pre->setObjectName(QString::fromUtf8("btn_pre"));
        btn_pre->setGeometry(QRect(144, 401, 33, 20));
        btn_stop = new QPushButton(Widget);
        btn_stop->setObjectName(QString::fromUtf8("btn_stop"));
        btn_stop->setGeometry(QRect(106, 401, 33, 20));
        btn_play = new QPushButton(Widget);
        btn_play->setObjectName(QString::fromUtf8("btn_play"));
        btn_play->setGeometry(QRect(182, 401, 33, 20));
        btn_delete = new QPushButton(Widget);
        btn_delete->setObjectName(QString::fromUtf8("btn_delete"));
        btn_delete->setGeometry(QRect(490, 401, 80, 20));
        volume_slider = new QSlider(Widget);
        volume_slider->setObjectName(QString::fromUtf8("volume_slider"));
        volume_slider->setGeometry(QRect(350, 400, 84, 16));
        volume_slider->setMaximum(100);
        volume_slider->setValue(100);
        volume_slider->setOrientation(Qt::Horizontal);
        btn_mute = new QPushButton(Widget);
        btn_mute->setObjectName(QString::fromUtf8("btn_mute"));
        btn_mute->setGeometry(QRect(300, 400, 33, 20));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        time_label->setText(QString());
        btn_playmodel->setText(QString());
        btn_fullscreen->setText(QCoreApplication::translate("Widget", "FullScreen", nullptr));
        label->setText(QString());
        btn_open->setText(QCoreApplication::translate("Widget", "open", nullptr));
        btn_next->setText(QString());
        btn_pre->setText(QString());
        btn_stop->setText(QString());
        btn_play->setText(QString());
        btn_delete->setText(QCoreApplication::translate("Widget", "delete", nullptr));
        btn_mute->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
