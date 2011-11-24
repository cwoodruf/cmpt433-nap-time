/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * Defines the user interface for the launcher window.
 * As the name suggests this simply runs other programs.
 */
#ifndef MEMOS_H
#define MEMOS_H

#include <QProcess>

/* define this via make when compiling for the board */
#ifndef EM2440
#define IS_EM2440 0
#else
#define IS_EM2440 1
#endif

#include <QMainWindow>

enum MemoState {
	NotStarted,
	Stopped,
	Playing,
	Paused
};

class Memo {
public:
	Memo(QString p) 	{ setPath(p); setState(NotStarted); }
	Memo() 			{ setState(NotStarted); }
	~Memo();

	void setPath(QString p) 	{ path = p; }
	QString getPath(void) 		{ return path; }
	void setState(MemoState m) 	{ state = m; }
	MemoState getState(void) 	{ return state; }

private:
	QString path;
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
	void recordMemo(void);
	void deleteMemo(void);
	void refreshMemos(void);
	void refreshPeers(void);
	void refreshAll(void);
	void sendMemo(void);

private:
	Ui::MainWindow *ui;
	QProcess *madplay;
	QProcess *sendmemo;
	Memo *currentMemo;
};

#endif // MEMOS_H

