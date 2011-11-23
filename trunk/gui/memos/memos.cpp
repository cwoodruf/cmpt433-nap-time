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
	QObject::connect(ui->btnDeleteMemo, SIGNAL(clicked()), this, SLOT(deleteMemo()));
	QObject::connect(ui->btnRecordMemo, SIGNAL(clicked()), this, SLOT(recordMemo()));
	QObject::connect(ui->btnSendMemo, SIGNAL(clicked()), this, SLOT(sendMemo()));
	QObject::connect(ui->btnRefreshPeers, SIGNAL(clicked()), this, SLOT(refreshPeers()));
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
 * This slot takes care of the play, pause, stop etc for the current memo.
 * Uses the madplay process to do the actual work of playing the memo.
 * Should pop up a dialog if no memo has been selected.
 */
void MainWindow::setMemoState(void)
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
 * Uses the sendmemo p
 * Should pop up a dialog if no memo and peer have been selected.
 */
void MainWindow::sendMemo(void)
{
}

