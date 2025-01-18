/********************************************************************************
** Form generated from reading UI file 'main_menu.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_MENU_H
#define UI_MAIN_MENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainMenu
{
public:
    QPushButton *imageButton;
    QPushButton *musicButton;
    QPushButton *videoButton;
    QPushButton *weatherButton;
    QPushButton *explorerButton;
    QPushButton *messageButton;

    void setupUi(QWidget *MainMenu)
    {
        if (MainMenu->objectName().isEmpty())
            MainMenu->setObjectName(QString::fromUtf8("MainMenu"));
        MainMenu->resize(600, 400);
        imageButton = new QPushButton(MainMenu);
        imageButton->setObjectName(QString::fromUtf8("imageButton"));
        imageButton->setGeometry(QRect(25, 25, 80, 80));
        musicButton = new QPushButton(MainMenu);
        musicButton->setObjectName(QString::fromUtf8("musicButton"));
        musicButton->setGeometry(QRect(25, 160, 80, 80));
        videoButton = new QPushButton(MainMenu);
        videoButton->setObjectName(QString::fromUtf8("videoButton"));
        videoButton->setGeometry(QRect(25, 295, 80, 80));
        weatherButton = new QPushButton(MainMenu);
        weatherButton->setObjectName(QString::fromUtf8("weatherButton"));
        weatherButton->setGeometry(QRect(160, 25, 80, 80));
        explorerButton = new QPushButton(MainMenu);
        explorerButton->setObjectName(QString::fromUtf8("explorerButton"));
        explorerButton->setGeometry(QRect(160, 160, 80, 80));
        messageButton = new QPushButton(MainMenu);
        messageButton->setObjectName(QString::fromUtf8("messageButton"));
        messageButton->setGeometry(QRect(160, 295, 80, 80));

        retranslateUi(MainMenu);

        QMetaObject::connectSlotsByName(MainMenu);
    } // setupUi

    void retranslateUi(QWidget *MainMenu)
    {
        MainMenu->setWindowTitle(QApplication::translate("MainMenu", "MainMenu", nullptr));
        imageButton->setText(QApplication::translate("MainMenu", "PushButton", nullptr));
        musicButton->setText(QApplication::translate("MainMenu", "PushButton", nullptr));
        videoButton->setText(QApplication::translate("MainMenu", "PushButton", nullptr));
        weatherButton->setText(QApplication::translate("MainMenu", "PushButton", nullptr));
        explorerButton->setText(QApplication::translate("MainMenu", "PushButton", nullptr));
        messageButton->setText(QApplication::translate("MainMenu", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainMenu: public Ui_MainMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_MENU_H
