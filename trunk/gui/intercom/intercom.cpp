/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * Intercom application: intiate a conference call to another nap node.
 */

#include <QProcess>
#include <QDir>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include "intercom.h"
#include "ui_intercom.h"

/**
 * Intercom window constructor. 
 */
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	naprtpsend(new QProcess),
	naprtprecv(new QProcess),
	naprtpconnect(new QProcess),
	naprtpstop(new QProcess)
{
	ui->setupUi(this);

	// Setup connections:
	QObject::connect(ui->btnConnect, SIGNAL(clicked()), this, SLOT(connectPeer()));
	QObject::connect(ui->btnRefreshPeers, SIGNAL(triggered()), this, SLOT(refreshPeers()));
	QObject::connect(ui->actionRefreshPeers, SIGNAL(triggered()), this, SLOT(refreshPeers()));
	QObject::connect(ui->actionChime_test, SIGNAL(triggered()), this, SLOT(chimeTest()));
	QObject::connect(naprtpconnect, SIGNAL(error(QProcess::ProcessError)), 
					this, SLOT(connectCrash(QProcess::ProcessError)));

	ui->listPeers->setSortingEnabled(true);
	refreshPeers();
}

/**
 * Intercom window destructor.
 */
MainWindow::~MainWindow()
{
	delete ui;
}

/**
 * Ring the chime on all active nodes: more for demonstrating the peering.
 */
void MainWindow::chimeTest(void) 
{
	QProcess chimetest;
	int ret;

	ret = QMessageBox::question(this,"Chime Test","Click Ok to chime all active peers.",
		QMessageBox::Cancel|QMessageBox::Ok);
	if (ret == QMessageBox::Ok) {
		chimetest.start("chimetest");
		chimetest.waitForFinished();
	}
}
/**
 * If the connect process crashed while optionally report it to the gui - mainly for debugging.
 */
void MainWindow::connectCrash(QProcess::ProcessError e)
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
 * Start the connect process
 * This is roughly:
 *
 * alert the other node via the web interface
 * if the other node responds with an ACK star the rtp send and receive processes.
 * when the end user is finished tear these down and alert the other node
 *
 * Should pop up a dialog if no peer has been selected.
 */
void MainWindow::connectPeer(void)
{
	QString peer,res;
	int ret;

	peer = getSelectedPeer();
	if (peer.length() == 0) return;

	ret = QMessageBox::question(this,"Intercom connect",
		"Connect to peer "+peer+"?",QMessageBox::Cancel|QMessageBox::Ok);

	if (ret == QMessageBox::Ok) {
/*
		naprtpstop->start("naprtpstop",QStringList() << peer);
		naprtpstop->waitForFinished();
		naprtpsend->start("naprtpsend",QStringList() << peer);
		naprtprecv->start("naprtprecv");
*/
		naprtpconnect->start("naprtpconnect",QStringList() << peer);
		naprtpconnect->waitForFinished();
		res = QString(naprtpconnect->readAllStandardOutput());
		QMessageBox::information(this,"Intercom",
			"Connection result: "+res+"\nClick OK to end session",QMessageBox::Ok);
		naprtpconnect->start("naprtpstop", QStringList() << peer);
		naprtpconnect->waitForFinished();
	}
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

