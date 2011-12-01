/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * references:
 * http://www.qtforum.org/article/3079/howto-start-an-external-program-from-a-qt-application.html
 * http://doc.qt.nokia.com/latest/qprocess.html
 * http://thesmithfam.org/blog/2010/02/07/talking-to-qt-threads/ - timers
 *
 */

#include <QProcess>
#include <QDir>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include "memos.h"
#include "ui_memos.h"

/**
 * Memo window constructor. 
 * Needs to get configuration info from the p2p scripts to know where to put the memos.
 */
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	playmemo(new QProcess),
	currentMemo(new Memo),
	refreshtimer(new QTimer)
{
	ui->setupUi(this);

	// Setup connections:
	QObject::connect(ui->btnPlayMemo, SIGNAL(clicked()), this, SLOT(setMemoState()));
	QObject::connect(ui->btnDeleteMemo, SIGNAL(clicked()), this, SLOT(deleteMemo()));
	QObject::connect(ui->btnRecordMemo, SIGNAL(clicked()), this, SLOT(recordMemo()));
	QObject::connect(ui->btnSendMemo, SIGNAL(clicked()), this, SLOT(sendMemo()));
	QObject::connect(ui->btnRefreshAll, SIGNAL(clicked()), this, SLOT(refreshAll()));
	QObject::connect(ui->actionRefreshPeers, SIGNAL(triggered()), this, SLOT(refreshPeers()));
	QObject::connect(ui->actionRefreshMemos, SIGNAL(triggered()), this, SLOT(refreshMemos()));
	QObject::connect(playmemo, SIGNAL(finished()), this, SLOT(stopMemo()));
	QObject::connect(playmemo, SIGNAL(error(QProcess::ProcessError)), 
					this, SLOT(crashMemo(QProcess::ProcessError)));
	QObject::connect(refreshtimer, SIGNAL(timeout()), this, SLOT(refreshNewMemos()));

	ui->listPeers->setSortingEnabled(true);
	ui->listMemos->setSortingEnabled(true);
	refreshPeers();
	refreshMemos();
	refreshtimer->start(10000);
}

/**
 * Memo window destructor. Cleans up ui and deletes the refreshtimer and currentMemo placeholder. 
 */
MainWindow::~MainWindow()
{
	delete refreshtimer;
	delete ui;
	delete currentMemo;
}

/**
 * This slot takes care of the play and pause for the current memo.
 * Uses the madplay process to do the actual work of playing the memo.
 * Should pop up a dialog if no memo has been selected.
 */
void MainWindow::setMemoState(void)
{
	QString memo;

	memo = getSelectedMemo();
	if (memo.length() == 0) return;

	if (currentMemo->getMemo() != memo) {
		currentMemo->setMemo(memo);
		currentMemo->setState(NotStarted);
		if (playmemo->state() != QProcess::NotRunning) playmemo->terminate();
	}
	if (currentMemo->getState() == Playing) {
		stopMemo();
	} else {
		ui->btnPlayMemo->setText("Stop");
		ui->listMemos->setDisabled(true);
		currentMemo->setState(Playing);
		currentMemo->setMemo(memo);
		playmemo->start("playmemo",QStringList() << memo);
		QMessageBox::information(this,"Play Memo",
			"Click OK to stop play.",QMessageBox::Ok);
		QProcess::execute("killmemo");
		stopMemo();
	}
}

/**
 * This slot stops a memo from playing. What this means is that
 * the play process is stopped entirely but we still remember which
 * memo we are looking at. 
 */
void MainWindow::stopMemo(void)
{
	currentMemo->setState(Stopped);
	playmemo->terminate();
	QProcess::execute("stopplaymemo");
	ui->btnPlayMemo->setText("Play");
	refreshMemos();
}

/**
 * If the memo crashed while playing optionally pop up an alert - mainly for debug.
 */
void MainWindow::crashMemo(QProcess::ProcessError e)
{
	QString msg;
	switch (e) {
	case QProcess::FailedToStart: msg = QString("FailedToStart"); break;
	case QProcess::Crashed: msg = QString("Crashed"); break;
	case QProcess::Timedout: msg = QString("Timedout"); break;
	case QProcess::WriteError: msg = QString("WriteError"); break;
	case QProcess::ReadError: msg = QString("ReadError"); break;
	case QProcess::UnknownError: msg = QString("UnknownError"); break;
	default: msg = QString("Unknown error code: "+e);
	}
	// QMessageBox::critical(this,"Error",msg);
	stopMemo();
}

/**
 * Creates a new memo using an external process to handle the recording.
 */
void MainWindow::recordMemo(void)
{
	int ret;
	QProcess recordmemo;
	QString message;

	ret = QMessageBox::question(this,"Record a Memo",
		"Click OK to start recording.",QMessageBox::Cancel|QMessageBox::Ok);

	if (ret == QMessageBox::Cancel) return;

	recordmemo.start("recordmemo");
	QMessageBox::information(this,"Recording Message","Click OK to stop recording");
	message = recordmemo.readAllStandardOutput();

	// doesn't seem to be getting stopped?
	recordmemo.terminate();
	QProcess::execute("stoprecordmemo");
	refreshMemos();
}

/**
 * Deletes a memo after confirmation via dialog box.
 */
void MainWindow::deleteMemo(void)
{
	QString memodir, memo;
	int ret;
	
	memodir = getMemodir();
	if (memodir.length() == 0) return;

	memo = getSelectedMemo();
	if (memo.length() == 0) return;

	ret = QMessageBox::question(this,"Delete Memo","Delete memo "+memo+"?",QMessageBox::Cancel|QMessageBox::Ok);

	if (ret == QMessageBox::Ok) {
		if (!QFile::remove(memodir+"/"+memo)) {
			QMessageBox::warning(this,"Error","Couldn't delete "+memodir+"/"+memo);
			return;
		}
		refreshMemos();
	}
		
}

/**
 * refresh the memos window if we have new memos
 */
void MainWindow::refreshNewMemos(void)
{
	QString memos;
	QStringList memolist;
	int i;
	bool different = false;

	if (currentMemo->getState() == Playing) return;

	memolist = QStringList(getMemos());
	if (memolist.count() == ui->listMemos->count()) {
		for (i = 0; i < ui->listMemos->count(); ++i) {
			QListWidgetItem *it = ui->listMemos->item(i);
			if ( ! memolist.contains(it->text())) {
				different = true;
				break;
			}
		}
	} else {
		different = true;
	}
	if (different) {
		refreshMemos();
	}
}

/**
 * returns a list of memos
 */
QStringList MainWindow::getMemos(void) 
{
	QString memos;
	QProcess getmemos;

	getmemos.start("getmemos");
	getmemos.waitForFinished();
	memos = QString(getmemos.readAllStandardOutput());
	memos = QString(memos.trimmed());
	return memos.split("\n");
}

/**
 * Scan the memos directory for any new memos.
 */
void MainWindow::refreshMemos(void)
{
	QStringList memos;
	
	memos = QStringList(getMemos());
	ui->listMemos->clear();
	ui->listMemos->addItems(memos);
	ui->listMemos->setEnabled(true);
}

/**
 * Run napping to update list of peers on the network.
 */
void MainWindow::refreshPeers(void)
{
	QProcess getpeers;
	QString peers;

	getpeers.start("getpeers");
	getpeers.waitForFinished();
	peers = QString(getpeers.readAllStandardOutput());
	ui->listPeers->clear();
	ui->listPeers->addItems(peers.split("\n",QString::SkipEmptyParts));
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
	QProcess sendmemo;
	QString peer,memo,res;
	int ret;

	peer = getSelectedPeer();
	if (peer.length() == 0) return;
	memo = getSelectedMemo();
	if (memo.length() == 0) return;

	ret = QMessageBox::question(this,"Send memo?",
		"Send memo "+memo+" to peer "+peer+"?",QMessageBox::Cancel|QMessageBox::Ok);

	if (ret == QMessageBox::Ok) {
		sendmemo.start("sendmemo",QStringList() << peer << memo);
		sendmemo.waitForFinished();
		res = QString(sendmemo.readAllStandardOutput());
		if (res.length()) QMessageBox::information(this,"Memo Send","Send result:\n"+res);
		else QMessageBox::warning(this,"Memo Send Error","No send result. Try sending again.");
	}
}

/**
 * Get the current memo directory from the napconfig script.
 */
QString MainWindow::getMemodir(void) 
{
	QString dir;
	QProcess getmemodir;

	getmemodir.start("getmemodir");
	getmemodir.waitForFinished();
	dir = QString(getmemodir.readAllStandardOutput());
	return dir.trimmed();
}

/**
 * Get the first memo selected from the gui.
 */
QString MainWindow::getSelectedMemo(void)
{
	QList<QListWidgetItem *> selectedMemos;

	selectedMemos = ui->listMemos->selectedItems();
	if (selectedMemos.count() == 0) {
		QMessageBox::warning(this,"Error","Please select a memo!");
		return QString("");
	} 
	return selectedMemos[0]->text();
}

/**
 * Get the first peer selected from the gui.
 */
QString MainWindow::getSelectedPeer(void)
{
	QList<QListWidgetItem *> selectedPeers;

	selectedPeers = ui->listPeers->selectedItems();
	if (selectedPeers.count() == 0) {
		QMessageBox::warning(this,"Error","Please select a peer!");
		return QString("");
	} 
	return selectedPeers[0]->text();
}

