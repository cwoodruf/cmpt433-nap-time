/********************************************************************************
** Form generated from reading UI file 'gui_PlaylistCreate.ui'
**

** Created: Thu Nov 24 01:27:21 2011
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUI_PLAYLISTCREATE_H
#define UI_GUI_PLAYLISTCREATE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlaylistWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QPushButton *pushButton;
    QListWidget *listWidget;
    QCommandLinkButton *backButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *PlaylistWindow)
    {
        if (PlaylistWindow->objectName().isEmpty())
            PlaylistWindow->setObjectName(QString::fromUtf8("PlaylistWindow"));
        PlaylistWindow->resize(395, 220);
        centralwidget = new QWidget(PlaylistWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(9, 9, 231, 17));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(11, 135, 85, 27));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(10, 30, 301, 101));
        backButton = new QCommandLinkButton(centralwidget);
        backButton->setObjectName(QString::fromUtf8("backButton"));
        backButton->setGeometry(QRect(250, 0, 61, 31));
        PlaylistWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(PlaylistWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 395, 25));
        PlaylistWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(PlaylistWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        PlaylistWindow->setStatusBar(statusbar);

        retranslateUi(PlaylistWindow);

        QMetaObject::connectSlotsByName(PlaylistWindow);
    } // setupUi

    void retranslateUi(QMainWindow *PlaylistWindow)
    {
        PlaylistWindow->setWindowTitle(QApplication::translate("PlaylistWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PlaylistWindow", "Check to add to playlist", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("PlaylistWindow", "Save", 0, QApplication::UnicodeUTF8));
        backButton->setText(QApplication::translate("PlaylistWindow", "Back", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PlaylistWindow: public Ui_PlaylistWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_PLAYLISTCREATE_H
