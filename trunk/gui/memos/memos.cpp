/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * references:
 * http://www.qtforum.org/article/3079/howto-start-an-external-program-from-a-qt-application.html
 * http://doc.qt.nokia.com/latest/qprocess.html
 *
 */

#include <QProcess>
#include <QDir>
#include <QMessageBox>
#include "memos.h"
#include "ui_memos.h"

/**
 * Memo window constructor. Creates placeholders for the madplay and sendmemo processes.
 * Needs to get configuration info from the p2p scripts to know where to put the memos.
 */
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	playmemo(new QProcess),
	currentMemo(new Memo)
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

	refreshPeers();
	refreshMemos();
}

/**
 * Memo window destructor. Cleans up ui. 
 * Shuts down the madplay and sendmemo processes if they are still running.
 */
MainWindow::~MainWindow()
{
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
		ui->btnPlayMemo->setText("Play");
		currentMemo->setState(Stopped);
		playmemo->terminate();
		QProcess::execute("killall aplay");
	} else {
		ui->btnPlayMemo->setText("Stop");
		currentMemo->setState(Playing);
		currentMemo->setMemo(memo);
		playmemo->start("/bin/playmemo",QStringList() << memo);
	}
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
	int ret;
	QProcess recordmemo;
	QString message;

	ret = QMessageBox::question(this,"Record a Memo","Click OK to start recording.",QMessageBox::Cancel|QMessageBox::Ok);

	if (ret == QMessageBox::Cancel) return;

	recordmemo.start("/bin/recordmemo");
	QMessageBox::information(this,"Recording Message","Click OK to stop recording");
	message = recordmemo.readAllStandardOutput();

	// doesn't seem to be getting stopped?
	recordmemo.terminate();
	QProcess::execute("killall arecord");

	QMessageBox::information(this,"Saved Message","Saved "+message);
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
 * Scan the memos directory for any new memos.
 */
void MainWindow::refreshMemos(void)
{
	QDir memos;
	QProcess getmemodir;
	QString memodir;

	memodir = getMemodir();
	memos = QDir(memodir,QString("*.wav"),QDir::Name,QDir::Files);
	if (memos.exists() == false) {
		QMessageBox::critical(this,"ERROR","directory "+memodir+" does not exist!");
		return;
	}
	ui->listMemos->clear();
	ui->listMemos->addItems(memos.entryList());
}

/**
 * Run napping to update list of peers on the network.
 */
void MainWindow::refreshPeers(void)
{
	QProcess getpeers;
	QString peers;

	getpeers.start("/bin/getpeers");
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

	ret = QMessageBox::question(this,"Send memo?","Send memo "+memo+" to peer "+peer+"?",QMessageBox::Cancel|QMessageBox::Ok);

	if (ret == QMessageBox::Ok) {
		sendmemo.start("/bin/sendmemo",QStringList() << peer << memo);
		sendmemo.waitForFinished();
		res = QString(sendmemo.readAllStandardOutput());
		QMessageBox::information(this,"Memo Send","Send result:\n"+res);
	}
	refreshAll();
}

/**
 * Get the current memo directory from the napconfig script.
 */
QString MainWindow::getMemodir(void) 
{
	QString dir;
	QProcess getmemodir;

	getmemodir.start("/bin/getmemodir");
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

