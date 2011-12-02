/********************************************************************************
** Form generated from reading UI file 'intercom.ui'
**
** Created: Fri Dec 2 12:57:47 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTERCOM_H
#define UI_INTERCOM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
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
    QAction *actionRefreshPeers;
    QAction *actionClose;
    QAction *actionChime_test;
    QWidget *centralwidget;
    QPushButton *btnRefreshPeers;
    QPushButton *btnConnect;
    QListWidget *listPeers;
    QLabel *labelPeers;
    QLabel *label;
    QMenuBar *menubar;
    QMenu *menuIntercom;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(320, 240);
        actionRefreshPeers = new QAction(MainWindow);
        actionRefreshPeers->setObjectName(QString::fromUtf8("actionRefreshPeers"));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionChime_test = new QAction(MainWindow);
        actionChime_test->setObjectName(QString::fromUtf8("actionChime_test"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        btnRefreshPeers = new QPushButton(centralwidget);
        btnRefreshPeers->setObjectName(QString::fromUtf8("btnRefreshPeers"));
        btnRefreshPeers->setGeometry(QRect(20, 130, 121, 27));
        btnConnect = new QPushButton(centralwidget);
        btnConnect->setObjectName(QString::fromUtf8("btnConnect"));
        btnConnect->setGeometry(QRect(150, 130, 151, 27));
        listPeers = new QListWidget(centralwidget);
        listPeers->setObjectName(QString::fromUtf8("listPeers"));
        listPeers->setGeometry(QRect(50, 50, 241, 71));
        labelPeers = new QLabel(centralwidget);
        labelPeers->setObjectName(QString::fromUtf8("labelPeers"));
        labelPeers->setGeometry(QRect(20, 50, 31, 17));
        labelPeers->setStyleSheet(QString::fromUtf8("font: 8pt \"Ubuntu\";"));
        labelPeers->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 271, 41));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label->setWordWrap(true);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 320, 25));
        menuIntercom = new QMenu(menubar);
        menuIntercom->setObjectName(QString::fromUtf8("menuIntercom"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuIntercom->menuAction());
        menuIntercom->addAction(actionRefreshPeers);
        menuIntercom->addAction(actionChime_test);
        menuIntercom->addSeparator();
        menuIntercom->addAction(actionClose);

        retranslateUi(MainWindow);
        QObject::connect(actionClose, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionRefreshPeers->setText(QApplication::translate("MainWindow", "Refresh Peers", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("MainWindow", "Close", 0, QApplication::UnicodeUTF8));
        actionChime_test->setText(QApplication::translate("MainWindow", "Chime test", 0, QApplication::UnicodeUTF8));
        btnRefreshPeers->setText(QApplication::translate("MainWindow", "Refresh Peers", 0, QApplication::UnicodeUTF8));
        btnConnect->setText(QApplication::translate("MainWindow", "Connect", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        listPeers->setToolTip(QApplication::translate("MainWindow", "Peers", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelPeers->setText(QApplication::translate("MainWindow", "Peers", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Select a peer and click Connect to start the intercom", 0, QApplication::UnicodeUTF8));
        menuIntercom->setTitle(QApplication::translate("MainWindow", "Intercom", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTERCOM_H
