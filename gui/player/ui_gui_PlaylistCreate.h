/********************************************************************************
** Form generated from reading UI file 'gui_PlaylistCreate.ui'
**
** Created: Wed Nov 30 19:00:09 2011
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
    QPushButton *saveButton;
    QListWidget *listWidget;
    QCommandLinkButton *backButton;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *PlaylistWindow)
    {
        if (PlaylistWindow->objectName().isEmpty())
            PlaylistWindow->setObjectName(QString::fromUtf8("PlaylistWindow"));
        PlaylistWindow->resize(319, 240);
        centralwidget = new QWidget(PlaylistWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 140, 141, 16));
        saveButton = new QPushButton(centralwidget);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setGeometry(QRect(155, 135, 85, 27));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(10, 30, 301, 101));
        backButton = new QCommandLinkButton(centralwidget);
        backButton->setObjectName(QString::fromUtf8("backButton"));
        backButton->setGeometry(QRect(244, 135, 61, 27));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 1, 191, 27));
        PlaylistWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(PlaylistWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 319, 25));
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
        label->setText(QApplication::translate("PlaylistWindow", "Check selections", 0, QApplication::UnicodeUTF8));
        saveButton->setText(QApplication::translate("PlaylistWindow", "Save", 0, QApplication::UnicodeUTF8));
        backButton->setText(QApplication::translate("PlaylistWindow", "Back", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("PlaylistWindow", "Refresh Available Music", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PlaylistWindow: public Ui_PlaylistWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_PLAYLISTCREATE_H
