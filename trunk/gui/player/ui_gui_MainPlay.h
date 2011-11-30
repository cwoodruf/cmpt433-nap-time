/********************************************************************************
** Form generated from reading UI file 'gui_MainPlay.ui'
**
** Created: Wed Nov 30 02:30:45 2011
**      by: Qt User Interface Compiler version 4.7.4
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
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
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
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QListWidget *listWidget;
    QDial *dial;
    QPushButton *playButton;
    QPushButton *prevButton;
    QPushButton *pauseButton;
    QPushButton *nextButton;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *PlayerWindow)
    {
        if (PlayerWindow->objectName().isEmpty())
            PlayerWindow->setObjectName(QString::fromUtf8("PlayerWindow"));
        PlayerWindow->resize(319, 240);
        actionGreat_Music = new QAction(PlayerWindow);
        actionGreat_Music->setObjectName(QString::fromUtf8("actionGreat_Music"));
        centralwidget = new QWidget(PlayerWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(9, 9, 181, 17));
        allSongsButton = new QCommandLinkButton(centralwidget);
        allSongsButton->setObjectName(QString::fromUtf8("allSongsButton"));
        allSongsButton->setGeometry(QRect(200, 0, 121, 29));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(250, 130, 67, 17));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(242, 100, 16, 21));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(296, 100, 21, 21));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(11, 31, 221, 111));
        dial = new QDial(centralwidget);
        dial->setObjectName(QString::fromUtf8("dial"));
        dial->setGeometry(QRect(240, 50, 71, 71));
        playButton = new QPushButton(centralwidget);
        playButton->setObjectName(QString::fromUtf8("playButton"));
        playButton->setGeometry(QRect(11, 151, 41, 27));
        prevButton = new QPushButton(centralwidget);
        prevButton->setObjectName(QString::fromUtf8("prevButton"));
        prevButton->setGeometry(QRect(120, 151, 81, 27));
        pauseButton = new QPushButton(centralwidget);
        pauseButton->setObjectName(QString::fromUtf8("pauseButton"));
        pauseButton->setGeometry(QRect(60, 151, 51, 27));
        nextButton = new QPushButton(centralwidget);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));
        nextButton->setGeometry(QRect(210, 151, 85, 27));
        PlayerWindow->setCentralWidget(centralwidget);
        playButton->raise();
        prevButton->raise();
        nextButton->raise();
        label->raise();
        allSongsButton->raise();
        label_2->raise();
        label_3->raise();
        label_4->raise();
        listWidget->raise();
        dial->raise();
        pauseButton->raise();
        statusbar = new QStatusBar(PlayerWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        PlayerWindow->setStatusBar(statusbar);

        retranslateUi(PlayerWindow);

        QMetaObject::connectSlotsByName(PlayerWindow);
    } // setupUi

    void retranslateUi(QMainWindow *PlayerWindow)
    {
        PlayerWindow->setWindowTitle(QApplication::translate("PlayerWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionGreat_Music->setText(QApplication::translate("PlayerWindow", "Great Music", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PlayerWindow", "Playlist Songs", 0, QApplication::UnicodeUTF8));
        allSongsButton->setText(QApplication::translate("PlayerWindow", "List All Songs", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("PlayerWindow", "Volume", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PlayerWindow", "0", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("PlayerWindow", "10", 0, QApplication::UnicodeUTF8));
        playButton->setText(QApplication::translate("PlayerWindow", "Play", 0, QApplication::UnicodeUTF8));
        prevButton->setText(QApplication::translate("PlayerWindow", "Previous", 0, QApplication::UnicodeUTF8));
        pauseButton->setText(QApplication::translate("PlayerWindow", "Pause", 0, QApplication::UnicodeUTF8));
        nextButton->setText(QApplication::translate("PlayerWindow", "Next", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PlayerWindow: public Ui_PlayerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_MAINPLAY_H
