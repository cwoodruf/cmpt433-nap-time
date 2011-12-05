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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <QMessageBox>

extern "C" {
    #include "buttondrv.h"
}

PlayerWindow::PlayerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerWindow),
    madplay(new QProcess),
    stopsong(new QProcess)
{
    ui->setupUi(this);

    displaySongsList ();

    playlistWindow = new PlaylistWindow (&musicList, this);

    connect(ui->listWidget, SIGNAL(currentRowChanged (int)),
        this, SLOT(currentSongChanged(int)));
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
        this, SLOT(playSongDoubleClick(QListWidgetItem *)));
    QObject::connect(ui->allSongsButton, SIGNAL(clicked (bool)), this, SLOT(showAllSongs()));
    connect(ui->prevButton, SIGNAL(pressed ()), this, SLOT(prevSong()));
    connect(ui->nextButton, SIGNAL(pressed ()), this, SLOT(nextSong()));

    // display music list
    // qDebug()<<"Starting thread...";
    buttonThread = new ButtonThread();
    QObject::connect(buttonThread, SIGNAL(buttonsChanged(int)), this, SLOT(setButtons(int)));
    buttonThread->start();

    QObject::connect(ui->playButton, SIGNAL(clicked(bool)), this, SLOT(playSong()));
    //QObject::connect(ui->pauseButton, SIGNAL(pressed()), this, SLOT(pauseSong()));
    QObject::connect(ui->pauseButton, SIGNAL(pressed()), this, SLOT(stopSong()));
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

void PlayerWindow::playSongDoubleClick (QListWidgetItem* item) {
    playSong ();
}

void PlayerWindow::currentSongChanged (int current) {
    prevSongIndex = currentSongIndex;
    currentSongIndex = current;
}

void PlayerWindow::prevSong () {
    if (currentSongIndex == -1) {
        return;
    }
    printf ("1\n");
    int next, i;
    for (i = 1; i < musicList.getSize (); i++) {
        next = currentSongIndex - i;
        if (next < 0) {
            next += musicList.getSize ();
        }
        QListWidgetItem* tmp = ui->listWidget->item (next);
        if (tmp->isHidden () == false) {
            break;
        }
    }
    printf ("%d, %d\n", i, next);
    if (i != musicList.getSize ()) {
        disconnect(ui->listWidget, SIGNAL(currentRowChanged (int)),
        this, SLOT(currentSongChanged(int)));
        ui->listWidget->setCurrentRow (next);
        currentSongChanged (next);
        connect(ui->listWidget, SIGNAL(currentRowChanged (int)),
        this, SLOT(currentSongChanged(int)));
        playSong ();
    }
}

void PlayerWindow::nextSong () {
    if (currentSongIndex == -1) {
        return;
    }
    printf ("1\n");
    int next, i;
    for (i = 1; i < musicList.getSize (); i++) {
        next = currentSongIndex + i;
        if (next >= musicList.getSize ()) {
            next -= musicList.getSize ();
        }
        QListWidgetItem* tmp = ui->listWidget->item (next);
        if (tmp->isHidden () == false) {
            break;
        }
    }
    printf ("%d, %d\n", i, next);
    if (i != musicList.getSize ()) {
        disconnect(ui->listWidget, SIGNAL(currentRowChanged (int)),
        this, SLOT(currentSongChanged(int)));
        ui->listWidget->setCurrentRow (next);
        currentSongChanged (next);
        connect(ui->listWidget, SIGNAL(currentRowChanged (int)),
        this, SLOT(currentSongChanged(int)));
        playSong ();
    } else {
        QListWidgetItem* tmp = ui->listWidget->item (currentSongIndex);
        if (tmp->isHidden () == true) {
            stopSong ();
        }
    }
}

void PlayerWindow::fixCurrentSelection () {
    if (currentSongIndex == -1) {
        currentSongIndex = 0;
    } else {
        QListWidgetItem* tmp = ui->listWidget->item (currentSongIndex);
        if (tmp->isHidden () == true) {
            nextSong ();
        }
    }
}
    

void PlayerWindow::playSong () {
    //QString text = item->text ();
    //qDebug () << text;
    if (currentSongIndex == -1) {
        return;
    }
    if ((currentSongIndex == prevSongIndex) && (isPlay == true)) {
        return;
    }
    if (isPlay == true) {
        stopSong ();
    }
    prevSongIndex = currentSongIndex;
    isPlay = true;
    Song* song = musicList.getSongInfo (currentSongIndex);

/*
    if (song->path[0] != '/') {
        downloadSong (song);
    }
    QStringList arguments;
    arguments << "--tty-control" << song->path;
    qDebug () << song->path;
    madplay->start("madplay", arguments);
*/
    madplay->start("playsong", QStringList() << song->path);
    if (!madplay->waitForStarted())
        qDebug () << "cannot start madplay process";
/*
    madplay->closeWriteChannel ();
    if (!madplay->waitForFinished())
        return false;
    qDebug () << returnValue;
*/
}

void writeToPipe () {
    int fd = open ("/tmp/madplayFIFO", O_WRONLY);
    char pause[2] = {'p', 0};
    write (fd, pause, 1);
    close (fd);
    printf ("opened madplayFIFIO");
    
}

void PlayerWindow::downloadSong (Song* song) {
    QMessageBox message (QMessageBox::Information, "Download",
                         "Downloading" + song->filename,
                         QMessageBox::NoButton, this);
    message.setDisabled (true);
    message.open ();
    /*QProcess download = (this);
    download.setWorkingDirectory ("/mnt/sd/cache");
    QStringList arguments;
    arguments << "\"http://" + song->path + "\"" << "-O" << "-";
    download.start ("wget", arguments);
    qDebug () << arguments;

    if (!download.waitForStarted())
        qDebug () << "cannot start download process";
    sleep (3);
    download.closeWriteChannel ();
    if (download.exitCode())
        qDebug () << "something is wrong when downloading";
    qDebug () << "started";

    if (download.waitForFinished () == false) {
        qDebug () << "can't finish download process";
    }
    QByteArray returnValue = download.readAllStandardOutput();
    qDebug () << "return value: " << returnValue;*/
    QString params = "wget -O /mnt/sd/cache/" + song->filename + " \"http://" + song->path + "\"";
    system (params.toLocal8Bit().data());

    // song->path = "/mnt/sd/cache/" + song->filename;
    message.close ();
}

void PlayerWindow::pauseSong () {
    //QString text = item->text ();
    //qDebug () << text;
    QProcess pause;
    QStringList arguments;
    arguments << "./pause.sh";
    if (system("sh ./pause.sh") == -1) {
        qDebug () << "cannot start pause.sh";
    }
    printf ("done\n");
}

void PlayerWindow::stopSong () {
    madplay->terminate();
    madplay->waitForFinished();
    stopsong->start("stopsong");
/*
    madplay->kill();
    if (!madplay->waitForFinished())
        qDebug () << "cannot kill madplay process";
*/
    isPlay = false;
}

void PlayerWindow::displaySongsList () {
    ui->listWidget->clear ();
    currentSongIndex = prevSongIndex = -1;
    isPlay = false;

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
        currentSongIndex = 0;
    }
}

/**
 * when we press a hardware button do something in the UI
 */
void PlayerWindow::setButtons(int btnMask)
{
    if (btnMask > 0) {
	int currentRow = ui->listWidget->currentRow ();
        if (btnMask & (1<<BTN_BIT_UP)) {
		if (currentRow > 0) ui->listWidget->setCurrentRow(currentRow-1);
        }
        else if (btnMask & (1<<BTN_BIT_DOWN)) {
		if (currentRow < ui->listWidget->count()) ui->listWidget->setCurrentRow(currentRow+1);
        }
        else if (btnMask & (1<<BTN_BIT_LEFT)) {
		stopSong();
        }
        else if (btnMask & (1<<BTN_BIT_RIGHT)) {
		playSong();
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
