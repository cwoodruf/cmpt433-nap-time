/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * Defines the user interface for the intercom window
 */
#ifndef INTERCOM_H
#define INTERCOM_H

#include <QMainWindow>
#include <QProcess>
#include <QString>

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

public slots:
	void connectCrash(QProcess::ProcessError);
	void refreshPeers(void);
	void connectPeer(void);
	void chimeTest(void);

private:
	Ui::MainWindow *ui;
	QProcess *naprtpconnect;
	QString getSelectedPeer(void);
};

#endif // INTERCOM_H

