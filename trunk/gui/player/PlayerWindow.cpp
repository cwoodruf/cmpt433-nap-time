#include "PlayerWindow.h"
#include "ui_gui_MainPlay.h"
#include <QDebug>
#include <QStringListModel>
#include <QAbstractItemModel>
#include <QStringList>
#include <QItemSelectionModel>
#include <QItemSelection>
#include "MusicList.h"
#include <stdio.h>
#include <QModelIndex>

//extern "C" {
//    #include "buttondrv.h"
//}

PlayerWindow::PlayerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerWindow)
{
    ui->setupUi(this);

    // display music list
    MusicList musicList;
    musicList.setReadSource ("music.txt");
    QStringList music;
    if (musicList.isEmpty ()) {
        music << "No music available";
    } else {
        music = musicList.getMusicList ();
    }
    QAbstractItemModel *model = new QStringListModel(music);
    ui->listView->setModel (model);

    // signal handling
    // click on the music list
    connect(ui->listView->selectionModel (), SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this, SLOT(musicSelected (const QItemSelection &, const QItemSelection &)));
    connect(ui->listView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(printout()));

    //qDebug()<<"Starting thread...";
    //thread = new ButtonThread();
    //QObject::connect(thread, SIGNAL(buttonsChanged(int)), this, SLOT(setButtons(int)));
    //thread->start();
}

PlayerWindow::~PlayerWindow()
{
    delete ui;
/*    if (thread) {
        qDebug()<<"Deleting thread...";
        thread->wait(1);
        thread->terminate();
        delete thread;
    }*/
}

void PlayerWindow::musicSelected (const QItemSelection &, const QItemSelection &) {
    printf ("music selected:");
}

void PlayerWindow::printout () {
    printf ("music selected:");
}
/*
void ButtonWindow::setButtons(int btnMask)
{
    QString str = "";
    if (btnMask == -1) {
        str = "Unable to open buttons.";
    } else if (btnMask == 0) {
        str = "None";
    } else {
        if (btnMask & (1<<BTN_BIT_UP))
            str += "Up ";
        if (btnMask & (1<<BTN_BIT_DOWN))
            str += "Down ";
        if (btnMask & (1<<BTN_BIT_LEFT))
            str += "Left ";
        if (btnMask & (1<<BTN_BIT_RIGHT))
            str += "Right ";
    }
    qDebug() << "ButtonWindow::setButtons() Data =" << btnMask << " = " << str;
    ui->lblDisplay->setText(str);

}*/
