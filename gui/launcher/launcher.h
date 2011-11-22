/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * Defines the user interface for the launcher window.
 * As the name suggests this simply runs other programs.
 */
#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QProcess>
#include "dialogconfig.h"

/* define this via make when compiling for the board */
#ifndef EM2440
#define IS_EM2440 0
#else
#define IS_EM2440 1
#endif

#include <QMainWindow>

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
};

#endif // LAUNCHER_H

