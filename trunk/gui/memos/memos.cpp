/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * references:
 * http://www.qtforum.org/article/3079/howto-start-an-external-program-from-a-qt-application.html
 * http://doc.qt.nokia.com/latest/qprocess.html
 *
 */

#include <QProcess>
#include "memos.h"
#include "ui_memos.h"

/**
 * Memo window constructor. Creates placeholders for the madplay and sendmemo processes.
 * Needs to get configuration info from the p2p scripts to know where to put the memos.
 */
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	madplay(new QProcess()),
	sendmemo(new QProcess())
{
	ui->setupUi(this);

	// Setup connections:
	QObject::connect(ui->btnPlayMemo, SIGNAL(clicked()), this, SLOT(setMemoState()));
	QObject::connect(ui->btnStopMemo, SIGNAL(clicked()), this, SLOT(stopMemo()));
	QObject::connect(ui->btnDeleteMemo, SIGNAL(clicked()), this, SLOT(deleteMemo()));
	QObject::connect(ui->btnRecordMemo, SIGNAL(clicked()), this, SLOT(recordMemo()));
	QObject::connect(ui->btnSendMemo, SIGNAL(clicked()), this, SLOT(sendMemo()));
	QObject::connect(ui->btnRefreshAll, SIGNAL(clicked()), this, SLOT(refreshAll()));
	QObject::connect(ui->actionRefreshPeers, SIGNAL(triggered()), this, SLOT(refreshPeers()));
	QObject::connect(ui->actionRefreshMemos, SIGNAL(triggered()), this, SLOT(refreshMemos()));
}

/**
 * Memo window destructor. Cleans up ui. 
 * Shuts down the madplay and sendmemo processes if they are still running.
 */
MainWindow::~MainWindow()
{
	if (madplay->state() != QProcess::NotRunning) madplay->terminate();
	if (sendmemo->state() != QProcess::NotRunning) sendmemo->terminate();
	delete ui;
}

/**
 * This slot takes care of the play and pause for the current memo.
 * Uses the madplay process to do the actual work of playing the memo.
 * Should pop up a dialog if no memo has been selected.
 */
void MainWindow::setMemoState(void)
{
}

/**
 * This slot stops a memo from playing. What this means is that
 * the play process is stopped entirely but we still remember which
 * memo we are looking at. If we aren't looking at a memo then do
 * nothing.
 */
void MainWindow::stopMemo(void)
{
}

/**
 * Creates a new memo using an external process to handle the recording.
 */
void MainWindow::recordMemo(void)
{
}

/**
 * Deletes a memo after confirmation via dialog box.
 */
void MainWindow::deleteMemo(void)
{
}

/**
 * Scan the memos directory for any new memos.
 */
void MainWindow::refreshMemos(void)
{
}

/**
 * Run napping to update list of peers on the network.
 */
void MainWindow::refreshPeers(void)
{
}

/**
 * Update both peers and memo lists.
 */
void MainWindow::refreshAll(void)
{
	refreshMemos();
	refreshPeers();
}

/**
 * Uses the sendmemo p
 * Should pop up a dialog if no memo and peer have been selected.
 */
void MainWindow::sendMemo(void)
{
}

