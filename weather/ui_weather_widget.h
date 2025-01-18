/********************************************************************************
** Form generated from reading UI file 'weather_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEATHER_WIDGET_H
#define UI_WEATHER_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WeatherWidget
{
public:

    void setupUi(QWidget *WeatherWidget)
    {
        if (WeatherWidget->objectName().isEmpty())
            WeatherWidget->setObjectName(QString::fromUtf8("WeatherWidget"));
        WeatherWidget->resize(800, 600);

        retranslateUi(WeatherWidget);

        QMetaObject::connectSlotsByName(WeatherWidget);
    } // setupUi

    void retranslateUi(QWidget *WeatherWidget)
    {
        WeatherWidget->setWindowTitle(QApplication::translate("WeatherWidget", "Widget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WeatherWidget: public Ui_WeatherWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEATHER_WIDGET_H
