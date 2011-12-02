/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * references:
 * http://www.qtforum.org/article/3079/howto-start-an-external-program-from-a-qt-application.html
 * http://doc.qt.nokia.com/latest/qprocess.html
 *
 */

#include <QProcess>
#include "launcher.h"
#include "dialogconfig.h"
#include "periodicthread.h"
#include "ui_launcher.h"

/**
 * Launcher constructor. Sets up ui. Also starts thread that listens 
 * for intercomm connections. Should also check that naplistener is 
 * running and if not start it.
 */
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	player(new QProcess()),
	memos(new QProcess()),
	intercom(new QProcess()),
	naplistener(new QProcess()),
	memoblink(new PeriodicThread())
{
	ui->setupUi(this);

	// Setup connections:
	QObject::connect(ui->btnPlayer, SIGNAL(clicked()), this, SLOT(startPlayer()));
	QObject::connect(ui->btnMemos, SIGNAL(clicked()), this, SLOT(startMemos()));
	QObject::connect(ui->btnIntercom, SIGNAL(clicked()), this, SLOT(startIntercom()));
	QObject::connect(ui->actionConfig, SIGNAL(triggered()), this, SLOT(showDialogConfig()));
	QObject::connect(ui->actionNapListener, SIGNAL(triggered()), this, SLOT(restartNapListener()));
	QObject::connect(ui->actionChime_test, SIGNAL(triggered()), this, SLOT(chimeTest()));
	QObject::connect(ui->actionNfs, SIGNAL(triggered()), this, SLOT(mountNfs()));

	restartNapListener();
	memoblink->setAll(2,"memoblink",QStringList());
	memoblink->start();
}

/**
 * Launcher destructor. Cleans up ui. 
 * Shuts down thread that listens for intercomm connections.
 */
MainWindow::~MainWindow()
{
	restartNapListener();
	if (naplistener->state() != QProcess::NotRunning) naplistener->terminate();
	if (memos->state() != QProcess::NotRunning) memos->terminate();
	if (intercom->state() != QProcess::NotRunning) intercom->terminate();
	if (player->state() != QProcess::NotRunning) player->terminate();
	memoblink->wait(1);
	memoblink->terminate();
	delete memoblink;
	delete player;
	delete memos;
	delete intercom;
	delete naplistener;
	delete dialogconfig;
	delete ui;
}

/**
 * Restart naplistener if it is not running.
 */
void MainWindow::restartNapListener() 
{
	QProcess killnaplistener;

	killnaplistener.start("stopnaplistener");
	killnaplistener.waitForFinished();
	naplistener->start("startnaplistener");
	naplistener->waitForStarted();
	ui->statusbar->showMessage("naplistener restarted");
}

/**
 * Start the Memos app. Record and listen to, share and delete 
 * memos that are stored by date. 
 */
void MainWindow::startIntercom(void)
{
	if (intercom->state() == QProcess::NotRunning) {
		intercom->start("intercom");
		intercom->waitForStarted();
		ui->statusbar->showMessage("intercom starting: searching for peers");
	} else {
		intercom->terminate();
		ui->statusbar->showMessage("intercom stopped");
	}
}

/**
 * Start the Intercomm app. Starts an intercomm session with another node.
 * May also respond to a request from another node to start an intercom session.
 */
void MainWindow::startMemos(void)
{
	if (memos->state() == QProcess::NotRunning) {
		memos->start("memos");
		memos->waitForStarted();
		ui->statusbar->showMessage("memos starting: searching for peers");
	} else {
		memos->terminate();
		ui->statusbar->showMessage("memos stopped");
	}
}

/**
 * Start the media Player app. Could be started by an event such as an end user
 * pluging in a usb drive.
 */
void MainWindow::startPlayer(void)
{
	if (player->state() == QProcess::NotRunning) {
		player->start("player");
		player->waitForStarted();
		ui->statusbar->showMessage("player started");
	} else {
		player->terminate();
		ui->statusbar->showMessage("player stopped");
	}
}

/**
 * Show the configuration dialog window.
 * This explains how to configure the system via a web browser and shows
 * system configuration.
 */
void MainWindow::showDialogConfig(void)
{
	dialogconfig = new DialogConfig();
	dialogconfig->showMaximized();
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
