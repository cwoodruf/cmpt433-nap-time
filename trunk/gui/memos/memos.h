/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * Defines the user interface for the memos window.
 */
#ifndef MEMOS_H
#define MEMOS_H

#include <QMainWindow>
#include <QProcess>
#include <QString>

/* define this via make when compiling for the board */
#ifndef EM2440
#define IS_EM2440 0
#else
#define IS_EM2440 1
#endif

enum MemoState {
	NotStarted,
	Stopped,
	Playing,
	Paused
};

class Memo {
public:
	Memo(QString p) 	{ setMemo(p); setState(NotStarted); }
	Memo() 			{ setMemo(QString("")); setState(NotStarted); }
	~Memo()			{ };

	void setMemo(QString p) 	{ memo = p; }
	QString getMemo(void) 		{ return memo; }
	void setState(MemoState m) 	{ state = m; }
	MemoState getState(void) 	{ return state; }

private:
	QString memo;
	MemoState state;
};

namespace Ui {
	class MainWindow;
	class Memo;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void setMemoState(void);
	void stopMemo(void);
	void crashMemo(QProcess::ProcessError);
	void recordMemo(void);
	void deleteMemo(void);
	void refreshMemos(void);
	void refreshNewMemos(void);
	void refreshPeers(void);
	void refreshAll(void);
	void sendMemo(void);

private:
	Ui::MainWindow *ui;
	QProcess *playmemo;
	Memo *currentMemo;
	QTimer *refreshtimer;
	QString getMemodir(void);
	QStringList getMemos(void);
	QString getSelectedMemo(void);
	QString getSelectedPeer(void);
};

#endif // MEMOS_H

