/********************************************************************************
** Form generated from reading UI file 'dialogconfig.ui'
**
** Created: Mon Dec 5 12:02:52 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCONFIG_H
#define UI_DIALOGCONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogConfig
{
public:
    QWidget *widget;
    QGridLayout *gridLayout;
    QTextBrowser *textBrowserConfig;
    QPushButton *btnNapping;
    QPushButton *btnClose;

    void setupUi(QDialog *DialogConfig)
    {
        if (DialogConfig->objectName().isEmpty())
            DialogConfig->setObjectName(QString::fromUtf8("DialogConfig"));
        DialogConfig->resize(320, 240);
        widget = new QWidget(DialogConfig);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(30, 0, 258, 191));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        textBrowserConfig = new QTextBrowser(widget);
        textBrowserConfig->setObjectName(QString::fromUtf8("textBrowserConfig"));

        gridLayout->addWidget(textBrowserConfig, 0, 0, 1, 2);

        btnNapping = new QPushButton(widget);
        btnNapping->setObjectName(QString::fromUtf8("btnNapping"));

        gridLayout->addWidget(btnNapping, 1, 0, 1, 1);

        btnClose = new QPushButton(widget);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));

        gridLayout->addWidget(btnClose, 1, 1, 1, 1);


        retranslateUi(DialogConfig);
        QObject::connect(btnClose, SIGNAL(clicked()), DialogConfig, SLOT(close()));

        QMetaObject::connectSlotsByName(DialogConfig);
    } // setupUi

    void retranslateUi(QDialog *DialogConfig)
    {
        DialogConfig->setWindowTitle(QApplication::translate("DialogConfig", "Configuration", 0, QApplication::UnicodeUTF8));
        btnNapping->setText(QApplication::translate("DialogConfig", "Find Neighbours", 0, QApplication::UnicodeUTF8));
        btnClose->setText(QApplication::translate("DialogConfig", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogConfig: public Ui_DialogConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCONFIG_H
