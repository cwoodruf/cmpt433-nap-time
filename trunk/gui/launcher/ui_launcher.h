/********************************************************************************
** Form generated from reading UI file 'launcher.ui'
**
** Created: Mon Dec 5 12:02:52 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAUNCHER_H
#define UI_LAUNCHER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionConfig;
    QAction *actionNfs;
    QAction *actionNapListener;
    QAction *actionClose;
    QAction *actionChime_test;
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QCommandLinkButton *btnPlayer;
    QCommandLinkButton *btnMemos;
    QCommandLinkButton *btnIntercom;
    QMenuBar *menubar;
    QMenu *menuConfig;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(320, 240);
        actionConfig = new QAction(MainWindow);
        actionConfig->setObjectName(QString::fromUtf8("actionConfig"));
        actionNfs = new QAction(MainWindow);
        actionNfs->setObjectName(QString::fromUtf8("actionNfs"));
        actionNapListener = new QAction(MainWindow);
        actionNapListener->setObjectName(QString::fromUtf8("actionNapListener"));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionChime_test = new QAction(MainWindow);
        actionChime_test->setObjectName(QString::fromUtf8("actionChime_test"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(70, 10, 170, 122));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        btnPlayer = new QCommandLinkButton(layoutWidget);
        btnPlayer->setObjectName(QString::fromUtf8("btnPlayer"));

        gridLayout->addWidget(btnPlayer, 0, 0, 1, 1);

        btnMemos = new QCommandLinkButton(layoutWidget);
        btnMemos->setObjectName(QString::fromUtf8("btnMemos"));

        gridLayout->addWidget(btnMemos, 1, 0, 1, 1);

        btnIntercom = new QCommandLinkButton(layoutWidget);
        btnIntercom->setObjectName(QString::fromUtf8("btnIntercom"));

        gridLayout->addWidget(btnIntercom, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 320, 25));
        menuConfig = new QMenu(menubar);
        menuConfig->setObjectName(QString::fromUtf8("menuConfig"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuConfig->menuAction());
        menuConfig->addAction(actionConfig);
        menuConfig->addSeparator();
        menuConfig->addAction(actionChime_test);
        menuConfig->addAction(actionNapListener);
        menuConfig->addSeparator();
        menuConfig->addAction(actionClose);

        retranslateUi(MainWindow);
        QObject::connect(actionClose, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Nap Time Systems", 0, QApplication::UnicodeUTF8));
        actionConfig->setText(QApplication::translate("MainWindow", "Configuration", 0, QApplication::UnicodeUTF8));
        actionNfs->setText(QApplication::translate("MainWindow", "Remount /mnt/remote nfs share", 0, QApplication::UnicodeUTF8));
        actionNapListener->setText(QApplication::translate("MainWindow", "Restart naplistener", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("MainWindow", "Close", 0, QApplication::UnicodeUTF8));
        actionChime_test->setText(QApplication::translate("MainWindow", "Chime test", 0, QApplication::UnicodeUTF8));
        btnPlayer->setText(QApplication::translate("MainWindow", "Player", 0, QApplication::UnicodeUTF8));
        btnMemos->setText(QApplication::translate("MainWindow", "Memos", 0, QApplication::UnicodeUTF8));
        btnIntercom->setText(QApplication::translate("MainWindow", "Intercom", 0, QApplication::UnicodeUTF8));
        menuConfig->setTitle(QApplication::translate("MainWindow", "Configuration", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAUNCHER_H
