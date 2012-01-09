/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * Defines the user interface for the player window.
 * The player is the interface to find and play mp3 
 * files as well as share them.
 */
#ifndef PLAYER_H
#define PLAYER_H

#ifdef EM2440
#define IS_EM2440 1
#else
#define IS_EM2440 0
#endif

#include <QMainWindow>
#include <buttonthread.h>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void playStop(void);
	void up(void);
	void down(void);
	void setButtons(int);

private:
	Ui::MainWindow *ui;
	ButtonThread *buttonThread;
};

#endif // PLAYER_H

