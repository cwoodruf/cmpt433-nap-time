#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include <QMainWindow>
#include "buttonthread.h"

namespace Ui {
    class PlaylistWindow;
}

class PlaylistWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlaylistWindow(QWidget *parent = 0);
    ~PlaylistWindow();
    void setButtonThread (ButtonThread* thread);

protected:
    virtual void closeEvent(QCloseEvent *event);

public slots:
    void setButtons (int btnMask);
private:
    Ui::PlaylistWindow *ui;
    ButtonThread *buttonThread;
};

#endif // PLAYLISTWINDOW_H
