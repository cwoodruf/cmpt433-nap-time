#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "buttonthread.h"
#include "PlaylistWindow.h"
#include "SongList.h"
#include <QProcess>

namespace Ui {
    class PlayerWindow;
}

class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = 0);
    ~PlayerWindow();
    void toggleVisibility (int row);
    void fixCurrentSelection ();
    void displaySongsList ();
public slots:
    void playSong ();
    void pauseSong ();
    void stopSong ();
    void prevSong ();
    void nextSong ();
    void playSongDoubleClick(QListWidgetItem *);
    void setButtons (int btnMask);
    void showAllSongs ();
    void currentSongChanged (int current);
private:
    Ui::PlayerWindow *ui;
    PlaylistWindow *playlistWindow;
    SongList musicList;
    ButtonThread *buttonThread;
    QProcess *madplay;
    QProcess *stopsong;
    int currentSongIndex;
    int prevSongIndex;
    bool isPlay;
    void downloadSong (Song* song);
};

#endif // PLAYERWINDOW_H
