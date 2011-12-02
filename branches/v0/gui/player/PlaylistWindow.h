#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include <QMainWindow>
#include "buttonthread.h"
#include "SongList.h"
#include <QList>
#include <QListWidgetItem>

namespace Ui {
    class PlaylistWindow;
}

class PlaylistWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlaylistWindow(SongList* songList, QWidget *parent = 0);
    ~PlaylistWindow();
    void setButtonThread (ButtonThread* thread);

protected:
    virtual void closeEvent(QCloseEvent *event);

public slots:
    void setButtons (int btnMask);
    void savePlaylist ();
    void changePlaylist (QListWidgetItem* item);
    void refresh ();
private:
    Ui::PlaylistWindow *ui;
    ButtonThread *buttonThread;
    SongList* songList;
    void displaySongList ();
    QList<int> toggled;
};

#endif // PLAYLISTWINDOW_H
