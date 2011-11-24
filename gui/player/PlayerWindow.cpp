#include "PlayerWindow.h"
#include "ui_gui_MainPlay.h"
#include <QDebug>
#include <stdio.h>
#include <QStringList>
#include <QString>
#include <QIcon>
#include <QListWidgetItem>
#include <QCheckBox>
#include "PlaylistWindow.h"

extern "C" {
    #include "buttondrv.h"
}

PlayerWindow::PlayerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerWindow)
{
    ui->setupUi(this);

    displaySongsList ();

    playlistWindow = new PlaylistWindow (&musicList, this);

    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
        this, SLOT(playSong(QListWidgetItem *)));
    QObject::connect(ui->allSongsButton, SIGNAL(clicked (bool)), this, SLOT(showAllSongs()));
    // display music list
    qDebug()<<"Starting thread...";
    buttonThread = new ButtonThread();
    QObject::connect(buttonThread, SIGNAL(buttonsChanged(int)), this, SLOT(setButtons(int)));
    buttonThread->start();
}

PlayerWindow::~PlayerWindow()
{
    delete ui;
    if (buttonThread) {
        qDebug()<<"Deleting thread...";
        buttonThread->wait(1);
        buttonThread->terminate();
        delete buttonThread;
    }
}

void PlayerWindow::playSong (QListWidgetItem* item) {
    QString text = item->text ();
    qDebug () << text;
}

void PlayerWindow::displaySongsList () {
    ui->listWidget->clear ();

    int size = musicList.getSize ();
    if (size == 0) {
        qDebug () << "no songs found";
    } else {
        for (int i = 0; i < size; i++) {
            const Song* song = musicList.getSongInfo (i);
            QIcon icon = QIcon::fromTheme("edit-cut");
            QString fileName = song->title;
            if (song->title == "no title") {
                fileName = song->filename;
            }
            if (song->artist != "no artist") {
                fileName += " - " + song->artist;
            }
            QListWidgetItem *tmp = new QListWidgetItem (icon, fileName);
            ui->listWidget->addItem(tmp);
            if (song->inPlaylist == false) {
                tmp->setHidden (true);
            }
        }
        ui->listWidget->setCurrentRow (0);
    }
}

void PlayerWindow::setButtons(int btnMask)
{
    if (btnMask > 0) {
        if (btnMask & (1<<BTN_BIT_UP)) {
            int currentRow = ui->listWidget->currentRow ();
            if (currentRow > 0) {
                ui->listWidget->setCurrentRow (currentRow - 1);
            }
        }
        if (btnMask & (1<<BTN_BIT_DOWN)) {
            int currentRow = ui->listWidget->currentRow ();
            if (currentRow < ui->listWidget->count () - 1) {
                ui->listWidget->setCurrentRow (currentRow + 1);
            }
        }
/*
        if (btnMask & (1<<BTN_BIT_LEFT))
            str += "Left ";
        if (btnMask & (1<<BTN_BIT_RIGHT))
            str += "Right ";*/
    }
}

void PlayerWindow::showAllSongs () {
    QObject::disconnect(buttonThread, SIGNAL(buttonsChanged(int)), this, SLOT(setButtons(int)));
    playlistWindow->setButtonThread (buttonThread);
    playlistWindow->showMaximized();
}

void PlayerWindow::toggleVisibility (int row) {
    QListWidgetItem* tmp = ui->listWidget->item (row);
    tmp->setHidden (!tmp->isHidden ());
}
