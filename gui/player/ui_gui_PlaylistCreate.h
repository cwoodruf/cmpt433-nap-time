/********************************************************************************
** Form generated from reading UI file 'gui_PlaylistCreate.ui'
**
** Created: Tue Nov 22 22:28:57 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUI_PLAYLISTCREATE_H
#define UI_GUI_PLAYLISTCREATE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QListView *listView;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QListView *listView_2;
    QLabel *label_2;
    QMenuBar *menubar;
    QMenu *menuNaptime_Music_Player;
    QMenu *menuPlaylists;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(320, 240);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 2);

        listView = new QListView(centralwidget);
        listView->setObjectName(QString::fromUtf8("listView"));

        gridLayout_2->addWidget(listView, 1, 0, 1, 2);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout_2->addWidget(pushButton, 2, 0, 1, 1);

        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout_2->addWidget(pushButton_2, 2, 1, 1, 1);

        listView_2 = new QListView(centralwidget);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));

        gridLayout_2->addWidget(listView_2, 3, 0, 1, 2);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 4, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 320, 25));
        menuNaptime_Music_Player = new QMenu(menubar);
        menuNaptime_Music_Player->setObjectName(QString::fromUtf8("menuNaptime_Music_Player"));
        menuPlaylists = new QMenu(menubar);
        menuPlaylists->setObjectName(QString::fromUtf8("menuPlaylists"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuNaptime_Music_Player->menuAction());
        menubar->addAction(menuPlaylists->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Available Music", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("MainWindow", "Remove", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Playlist", 0, QApplication::UnicodeUTF8));
        menuNaptime_Music_Player->setTitle(QApplication::translate("MainWindow", "Naptime Music Player", 0, QApplication::UnicodeUTF8));
        menuPlaylists->setTitle(QApplication::translate("MainWindow", "Playlists", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_PLAYLISTCREATE_H
