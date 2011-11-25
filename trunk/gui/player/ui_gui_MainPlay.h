/********************************************************************************
** Form generated from reading UI file 'gui_MainPlay.ui'
**
** Created: Thu Nov 24 15:33:17 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUI_MAINPLAY_H
#define UI_GUI_MAINPLAY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QDial>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QRadioButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlayerWindow
{
public:
    QAction *actionGreat_Music;
    QWidget *centralwidget;
    QLabel *label;
    QCommandLinkButton *allSongsButton;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QListWidget *listWidget;
    QDial *dial;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_2;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QMenuBar *menubar;
    QMenu *menuNaptime_Music_Player;
    QMenu *menuListen_and_Enjoy;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *PlayerWindow)
    {
        if (PlayerWindow->objectName().isEmpty())
            PlayerWindow->setObjectName(QString::fromUtf8("PlayerWindow"));
        PlayerWindow->resize(395, 220);
        actionGreat_Music = new QAction(PlayerWindow);
        actionGreat_Music->setObjectName(QString::fromUtf8("actionGreat_Music"));
        centralwidget = new QWidget(PlayerWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(9, 9, 105, 17));
        allSongsButton = new QCommandLinkButton(centralwidget);
        allSongsButton->setObjectName(QString::fromUtf8("allSongsButton"));
        allSongsButton->setGeometry(QRect(200, 0, 121, 29));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 30, 311, 111));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(layoutWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        gridLayout->addWidget(listWidget, 0, 0, 1, 1);

        dial = new QDial(layoutWidget);
        dial->setObjectName(QString::fromUtf8("dial"));

        gridLayout->addWidget(dial, 0, 1, 1, 1);

        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 140, 311, 31));
        gridLayout_2 = new QGridLayout(layoutWidget1);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        radioButton = new QRadioButton(layoutWidget1);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        gridLayout_2->addWidget(radioButton, 0, 0, 1, 1);

        radioButton_2 = new QRadioButton(layoutWidget1);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        gridLayout_2->addWidget(radioButton_2, 0, 1, 1, 1);

        radioButton_3 = new QRadioButton(layoutWidget1);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));

        gridLayout_2->addWidget(radioButton_3, 0, 2, 1, 1);

        PlayerWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(PlayerWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 395, 25));
        menuNaptime_Music_Player = new QMenu(menubar);
        menuNaptime_Music_Player->setObjectName(QString::fromUtf8("menuNaptime_Music_Player"));
        menuListen_and_Enjoy = new QMenu(menubar);
        menuListen_and_Enjoy->setObjectName(QString::fromUtf8("menuListen_and_Enjoy"));
        PlayerWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(PlayerWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        PlayerWindow->setStatusBar(statusbar);

        menubar->addAction(menuNaptime_Music_Player->menuAction());
        menubar->addAction(menuListen_and_Enjoy->menuAction());
        menuListen_and_Enjoy->addAction(actionGreat_Music);

        retranslateUi(PlayerWindow);

        QMetaObject::connectSlotsByName(PlayerWindow);
    } // setupUi

    void retranslateUi(QMainWindow *PlayerWindow)
    {
        PlayerWindow->setWindowTitle(QApplication::translate("PlayerWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionGreat_Music->setText(QApplication::translate("PlayerWindow", "Great Music", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PlayerWindow", "Playlist Songs", 0, QApplication::UnicodeUTF8));
        allSongsButton->setText(QApplication::translate("PlayerWindow", "List All Songs", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("PlayerWindow", "Play", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("PlayerWindow", "Pause", 0, QApplication::UnicodeUTF8));
        radioButton_3->setText(QApplication::translate("PlayerWindow", "Rewind", 0, QApplication::UnicodeUTF8));
        menuNaptime_Music_Player->setTitle(QApplication::translate("PlayerWindow", "Naptime Music Player", 0, QApplication::UnicodeUTF8));
        menuListen_and_Enjoy->setTitle(QApplication::translate("PlayerWindow", "Listen and Enjoy", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PlayerWindow: public Ui_PlayerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_MAINPLAY_H
