/********************************************************************************
** Form generated from reading UI file 'gui_MainPlay.ui'
**
** Created: Fri Dec 2 03:02:48 2011
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
    QListWidget *listWidget;
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
        label->setGeometry(QRect(9, 10, 181, 17));
        allSongsButton = new QCommandLinkButton(centralwidget);
        allSongsButton->setObjectName(QString::fromUtf8("allSongsButton"));
        allSongsButton->setGeometry(QRect(150, 0, 151, 29));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(11, 31, 291, 128));
        playButton = new QPushButton(centralwidget);
        playButton->setObjectName(QString::fromUtf8("playButton"));
        playButton->setGeometry(QRect(11, 160, 41, 27));
        prevButton = new QPushButton(centralwidget);
        prevButton->setObjectName(QString::fromUtf8("prevButton"));
        prevButton->setGeometry(QRect(120, 160, 81, 27));
        pauseButton = new QPushButton(centralwidget);
        pauseButton->setObjectName(QString::fromUtf8("pauseButton"));
        pauseButton->setGeometry(QRect(60, 160, 51, 27));
        nextButton = new QPushButton(centralwidget);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));
        nextButton->setGeometry(QRect(210, 160, 85, 27));
        PlayerWindow->setCentralWidget(centralwidget);
        playButton->raise();
        prevButton->raise();
        nextButton->raise();
        label->raise();
        allSongsButton->raise();
        listWidget->raise();
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
        playButton->setText(QApplication::translate("PlayerWindow", "Play", 0, QApplication::UnicodeUTF8));
        prevButton->setText(QApplication::translate("PlayerWindow", "Previous", 0, QApplication::UnicodeUTF8));
        pauseButton->setText(QApplication::translate("PlayerWindow", "Stop", 0, QApplication::UnicodeUTF8));
        nextButton->setText(QApplication::translate("PlayerWindow", "Next", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PlayerWindow: public Ui_PlayerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_MAINPLAY_H
