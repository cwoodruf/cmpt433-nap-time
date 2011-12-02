/********************************************************************************
** Form generated from reading UI file 'gui_PlaylistCreate.ui'
**
** Created: Fri Dec 2 03:02:48 2011
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
    QPushButton *refreshButton;
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
        label->setGeometry(QRect(10, 170, 141, 16));
        saveButton = new QPushButton(centralwidget);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setGeometry(QRect(140, 161, 85, 27));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(10, 30, 301, 130));
        backButton = new QCommandLinkButton(centralwidget);
        backButton->setObjectName(QString::fromUtf8("backButton"));
        backButton->setGeometry(QRect(230, 161, 81, 27));
        refreshButton = new QPushButton(centralwidget);
        refreshButton->setObjectName(QString::fromUtf8("refreshButton"));
        refreshButton->setGeometry(QRect(10, 2, 191, 27));
        PlaylistWindow->setCentralWidget(centralwidget);
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
        refreshButton->setText(QApplication::translate("PlaylistWindow", "Refresh Available Music", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PlaylistWindow: public Ui_PlaylistWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_PLAYLISTCREATE_H
