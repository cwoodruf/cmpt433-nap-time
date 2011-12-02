#include "PlaylistWindow.h"
#include "ui_gui_PlaylistCreate.h"
#include <QCloseEvent>
#include "PlayerWindow.h"
#include <QDebug>

extern "C" {
    #include "buttondrv.h"
}

PlaylistWindow::PlaylistWindow(SongList* songList, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlaylistWindow)
{
    this->songList = songList;
    ui->setupUi(this);
    displaySongList ();

    QObject::connect(ui->backButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    QObject::connect(ui->saveButton, SIGNAL(clicked(bool)), this, SLOT(savePlaylist()));
    QObject::connect(ui->refreshButton, SIGNAL(pressed()), this, SLOT(refresh()));
    connect(ui->listWidget, SIGNAL(itemChanged (QListWidgetItem*)),
        this, SLOT(changePlaylist(QListWidgetItem*)));
}

PlaylistWindow::~PlaylistWindow()
{
    delete ui;
}

void PlaylistWindow::refresh()
{
    songList->refresh ();
    displaySongList ();
    PlayerWindow* parent = static_cast<PlayerWindow*>(parentWidget ());
    parent->displaySongsList ();
}

void PlaylistWindow::setButtonThread (ButtonThread* thread) {
    buttonThread = thread;
    QObject::connect(buttonThread, SIGNAL(buttonsChanged(int)), this, SLOT(setButtons(int)));
}

void PlaylistWindow::setButtons(int btnMask)
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

void PlaylistWindow::closeEvent(QCloseEvent *event)
{
    disconnect(ui->listWidget, SIGNAL(itemChanged (QListWidgetItem*)),
        this, SLOT(changePlaylist(QListWidgetItem*)));
    for (QList<int>::const_iterator it = toggled.begin (); it != toggled.end (); it++) {
        QListWidgetItem* tmp = ui->listWidget->item (*it);
        if (tmp->checkState () == Qt::Checked) {
            tmp->setCheckState (Qt::Unchecked);
        } else {
            tmp->setCheckState (Qt::Checked);
        }
    }
    connect(ui->listWidget, SIGNAL(itemChanged (QListWidgetItem*)),
        this, SLOT(changePlaylist(QListWidgetItem*)));
    toggled.clear ();
    QObject::disconnect(buttonThread, SIGNAL(buttonsChanged(int)), this, SLOT(setButtons(int)));
    QObject::connect(buttonThread, SIGNAL(buttonsChanged(int)), parentWidget (), SLOT(setButtons(int)));
    event->accept ();
}

void PlaylistWindow::displaySongList () {
    ui->listWidget->clear ();

    int size = songList->getSize ();
    if (size > 0) {
        for (int i = 0; i < size; i++) {
            const Song* song = songList->getSongInfo (i);
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
            if (song->inPlaylist) {
                tmp->setCheckState(Qt::Checked);
            } else {
                tmp->setCheckState (Qt::Unchecked);
            }
        }
        ui->listWidget->setCurrentRow (0);
    }
}

void PlaylistWindow::changePlaylist (QListWidgetItem* item) {
    int currentRow = ui->listWidget->currentRow ();
    int index = toggled.indexOf (currentRow);
    if (index != -1) {
        toggled.removeAt (index);
    } else {
        toggled.push_back (currentRow);
    }
}

void PlaylistWindow::savePlaylist () {
    PlayerWindow* parent = static_cast<PlayerWindow*>(parentWidget ());
    for (QList<int>::const_iterator it = toggled.begin (); it != toggled.end (); it++) {
        parent->toggleVisibility (*it);
    }
    parent->fixCurrentSelection ();
    toggled.clear ();
    close ();
}
