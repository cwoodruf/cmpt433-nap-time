/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * Defines the user interface for the launcher window.
 * As the name suggests this simply runs other programs.
 */
#ifndef LAUNCHER_H
#define LAUNCHER_H

#ifdef EM2440
#define IS_EM2440 1
#else
#define IS_EM2440 0
#endif

#include <QProcess>
#include <QMainWindow>
#include "periodicthread.h"
#include "dialogconfig.h"

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
	void startPlayer(void);
	void startMemos(void);
	void startIntercom(void);
	void showDialogConfig(void);
	void restartNapListener(void);
	void mountNfs(void);

private:
	Ui::MainWindow *ui;
	DialogConfig *dialogconfig;
	QProcess *player;
	QProcess *memos;
	QProcess *intercom;
	QProcess *naplistener;
	// blinks leds when we have new memos
	PeriodicThread *memoblink;
};

#endif // LAUNCHER_H

