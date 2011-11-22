/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * references:
 * http://www.qtforum.org/article/3079/howto-start-an-external-program-from-a-qt-application.html
 * http://doc.qt.nokia.com/latest/qprocess.html
 *
 */

#include <QProcess>
#include <QStringList>
#include "launcher.h"
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
	naplistener(new QProcess())
{
	ui->setupUi(this);
	restartNapListener();

	// Setup connections:
	QObject::connect(ui->btnPlayer, SIGNAL(clicked()), this, SLOT(startPlayer()));
	QObject::connect(ui->btnMemos, SIGNAL(clicked()), this, SLOT(startMemos()));
	QObject::connect(ui->btnIntercom, SIGNAL(clicked()), this, SLOT(startIntercom()));
	QObject::connect(ui->actionNapListener, SIGNAL(triggered()), this, SLOT(restartNapListener()));
	QStringList killcmd;
}

/**
 * Launcher destructor. Cleans up ui. 
 * Shuts down thread that listens for intercomm connections.
 */
MainWindow::~MainWindow()
{
	if (naplistener->state() != QProcess::NotRunning) naplistener->terminate();
	if (memos->state() != QProcess::NotRunning) memos->terminate();
	if (intercom->state() != QProcess::NotRunning) intercom->terminate();
	if (player->state() != QProcess::NotRunning) player->terminate();
	delete ui;
}

/**
 * Restart naplistener if it is not running.
 */
void MainWindow::restartNapListener() 
{
	QProcess killnaplistener;
	QString napcmd("/bin/naplistener >> /var/log/naplistener 2>&1");

	if (naplistener->state() == QProcess::NotRunning) {
		killnaplistener.start("killall naplistener");
		killnaplistener.waitForFinished();
		naplistener->start(napcmd);
	} else {
		naplistener->terminate();
		naplistener->waitForFinished();
		naplistener->start(napcmd);
	}
}

/**
 * Start the Memos app. Record and listen to, share and delete 
 * memos that are stored by date. 
 */
void MainWindow::startIntercom(void)
{
	if (intercom->state() == QProcess::NotRunning) {
		intercom->start("/bin/intercom");
		ui->btnIntercom->setText("Intercom started");
	} else {
		intercom->terminate();
		ui->btnIntercom->setText("Intercom");
	}
}

/**
 * Start the Intercomm app. Starts an intercomm session with another node.
 * May also respond to a request from another node to start an intercom session.
 */
void MainWindow::startMemos(void)
{
	if (memos->state() == QProcess::NotRunning) {
		memos->start("/bin/memos");
		ui->btnMemos->setText("Memos started");
	} else {
		memos->terminate();
		ui->btnMemos->setText("Memos");
	}
}

/**
 * Start the media Player app. Could be started by an event such as an end user
 * pluging in a usb drive.
 */
void MainWindow::startPlayer(void)
{
	if (player->state() == QProcess::NotRunning) {
		player->start("/bin/player");
		ui->btnPlayer->setText("Player started");
	} else {
		player->terminate();
		ui->btnPlayer->setText("Player");
	}
}
