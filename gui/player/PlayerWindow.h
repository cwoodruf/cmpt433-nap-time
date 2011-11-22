#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QMainWindow>
#include <QItemSelection>
//#include "buttonthread.h"

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
    void musicSelected (const QItemSelection &, const QItemSelection &);
    void printout ();

private:
    Ui::PlayerWindow *ui;
    //ButtonThread *thread;
};

#endif // PLAYERWINDOW_H
