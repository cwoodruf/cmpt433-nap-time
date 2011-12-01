/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * Defines the user interface for the intercom window
 */
#ifndef INTERCOMCONTROL_H
#define INTERCOMCONTROL_H

#include <QMainWindow>

/* define this via make when compiling for the board */
#ifndef EM2440
#define IS_EM2440 0
#else
#define IS_EM2440 1
#endif

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	void setNameHostIp(char *name, char *host, char *ip);

public slots:
	void closePeer();

private:
	Ui::MainWindow *ui;
};

#endif // INTERCOMCONTROL_H

