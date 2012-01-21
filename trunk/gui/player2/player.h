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
#include <QProcess>
#include <QString>
#include <QListWidgetItem>

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
	void setShared(QListWidgetItem *);
	void playStop(void);
	void prevItem(void);
	void nextItem(void);
	void randomItem(void);
	void refreshSources(void);
	void shareMedia(void);
	void unshareMedia(void);
	void playNext(int madret, QProcess::ExitStatus madstatus);
	void setPlay_All(bool);
	void setShuffle(bool);

private:
	Ui::MainWindow *ui;
	QProcess *napconfig;
	QProcess *madplay;
	QProcess *stopsong;
	void displayListSelector();
	void setItem(int row);
	void share(QString item);
	bool getNapConfig(QString);
	int timeout;
	bool isPlay;
	QString currSource;
	QString playing;
};

#endif // PLAYER_H

