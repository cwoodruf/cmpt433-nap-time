#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "buttonthread.h"
#include "PlaylistWindow.h"
#include "SongList.h"

namespace Ui {
    class PlayerWindow;
}

class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = 0);
    ~PlayerWindow();

public slots:
    void playSong (QListWidgetItem *);
    void setButtons (int btnMask);
    void showAllSongs ();
private:
    Ui::PlayerWindow *ui;
    PlaylistWindow *playlistWindow;
    SongList musicList;
    ButtonThread *buttonThread;
    void displaySongsList ();
};

#endif // PLAYERWINDOW_H
