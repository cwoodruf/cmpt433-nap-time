/********************************************************************************
** Form generated from reading UI file 'gui_MainPlay.ui'
**
** Created: Tue Nov 22 22:28:57 2011
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
#include <QtGui/QListView>
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
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QCommandLinkButton *commandLinkButton;
    QCommandLinkButton *commandLinkButton_2;
    QWidget *widget;
    QGridLayout *gridLayout;
    QListView *listView;
    QDial *dial;
    QMenuBar *menubar;
    QMenu *menuNaptime_Music_Player;
    QMenu *menuListen_and_Enjoy;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *PlayerWindow)
    {
        if (PlayerWindow->objectName().isEmpty())
            PlayerWindow->setObjectName(QString::fromUtf8("PlayerWindow"));
        PlayerWindow->resize(395, 240);
        actionGreat_Music = new QAction(PlayerWindow);
        actionGreat_Music->setObjectName(QString::fromUtf8("actionGreat_Music"));
        centralwidget = new QWidget(PlayerWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(9, 9, 105, 17));
        radioButton = new QRadioButton(centralwidget);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setGeometry(QRect(10, 110, 60, 22));
        radioButton_2 = new QRadioButton(centralwidget);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setGeometry(QRect(75, 110, 71, 22));
        radioButton_3 = new QRadioButton(centralwidget);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));
        radioButton_3->setGeometry(QRect(152, 110, 79, 22));
        commandLinkButton = new QCommandLinkButton(centralwidget);
        commandLinkButton->setObjectName(QString::fromUtf8("commandLinkButton"));
        commandLinkButton->setGeometry(QRect(10, 130, 151, 36));
        commandLinkButton_2 = new QCommandLinkButton(centralwidget);
        commandLinkButton_2->setObjectName(QString::fromUtf8("commandLinkButton_2"));
        commandLinkButton_2->setGeometry(QRect(168, 130, 131, 36));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 30, 291, 71));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        listView = new QListView(widget);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::PointingHandCursor)));

        gridLayout->addWidget(listView, 0, 0, 1, 1);

        dial = new QDial(widget);
        dial->setObjectName(QString::fromUtf8("dial"));

        gridLayout->addWidget(dial, 0, 1, 1, 1);

        PlayerWindow->setCentralWidget(centralwidget);
        label->raise();
        listView->raise();
        dial->raise();
        radioButton->raise();
        radioButton_2->raise();
        radioButton_3->raise();
        commandLinkButton->raise();
        commandLinkButton_2->raise();
        listView->raise();
        listView->raise();
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
        label->setText(QApplication::translate("PlayerWindow", "Available Music", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("PlayerWindow", "Play", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("PlayerWindow", "Pause", 0, QApplication::UnicodeUTF8));
        radioButton_3->setText(QApplication::translate("PlayerWindow", "Rewind", 0, QApplication::UnicodeUTF8));
        commandLinkButton->setText(QApplication::translate("PlayerWindow", "Download To Local", 0, QApplication::UnicodeUTF8));
        commandLinkButton_2->setText(QApplication::translate("PlayerWindow", "Playlists", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        listView->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        menuNaptime_Music_Player->setTitle(QApplication::translate("PlayerWindow", "Naptime Music Player", 0, QApplication::UnicodeUTF8));
        menuListen_and_Enjoy->setTitle(QApplication::translate("PlayerWindow", "Listen and Enjoy", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PlayerWindow: public Ui_PlayerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_MAINPLAY_H
