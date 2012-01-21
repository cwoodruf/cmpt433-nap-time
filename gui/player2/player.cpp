/**
 * Alternate version of the player app that includes 
 * finding songs on devices
 */
#include "player.h"
#include "ui_player.h"
// for rand and srand functions
#include <QtGlobal>
#include <QDebug>
#include <QProcess>
#include <QFileInfo>
#include <QMessageBox>

extern "C" {
	#include <time.h>
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	madplay(new QProcess),
	stopsong(new QProcess)
{
	currSource = QString();

	ui->setupUi(this);
	displayListSelector();

	connect(ui->buttonUp, SIGNAL(pressed()), this, SLOT(prevItem()));
	connect(ui->buttonDown, SIGNAL(pressed()), this, SLOT(nextItem()));
	connect(ui->buttonPlayStop, SIGNAL(pressed()), this, SLOT(playStop()));
	connect(ui->actionRefreshSources, SIGNAL(triggered()), this, SLOT(refreshSources()));
	connect(ui->buttonBack, SIGNAL(pressed()), this, SLOT(refreshSources()));
	connect(ui->buttonShare, SIGNAL(pressed()), this, SLOT(shareMedia()));
	connect(ui->actionUnshare_Media, SIGNAL(triggered()), this, SLOT(unshareMedia()));
	connect(madplay, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(playNext(int,QProcess::ExitStatus)));
}

MainWindow::~MainWindow()
{
	if (madplay->state() != QProcess::NotRunning) madplay->terminate();
	if (stopsong->state() != QProcess::NotRunning) stopsong->terminate();
	delete madplay;
	delete stopsong;
	delete ui;
}

/**
 * fill our listSelector with something based on the value of currDir
 * if currDir is empty then get a list of devices
 * if not split off part of the path and search that path for media
 * use external script to do this as this is device dependent 
 */
void MainWindow::displayListSelector() 
{
	QProcess getrawlist;
	QString rawlist;

	// clear the list
	while (ui->listSelector->count()) {
		QListWidgetItem * item = ui->listSelector->takeItem(0);
		if (item) delete item;
	}

	if (currSource == "") {
		ui->labelTitle->setText("Sources");
		ui->buttonBack->setEnabled(false);
		ui->buttonPlayStop->setText("Open");
		getrawlist.start("getsources");
	} else {
		ui->labelTitle->setText("Source " + currSource);
		ui->buttonBack->setEnabled(true);
		ui->buttonPlayStop->setText("Play");
		getrawlist.start("getmusiclist",QStringList() << currSource);
	}
	getrawlist.waitForFinished(60000);
	rawlist = getrawlist.readAllStandardOutput();

	// then dump our current list in there
	ui->listSelector->setWordWrap(true);
	ui->listSelector->insertItems(0,rawlist.split("\n",QString::SkipEmptyParts));
	setItem(0);
}

/**
 * slot triggered by refresh sources menu item
 * go back to the start
 * will stop any music playing
 * and clear any saved info
 * and then display the sources list
 */
void MainWindow::refreshSources()
{
	if (isPlay) playStop();
	currSource = "";
	displayListSelector();
}

/**
 * slot triggered when madplay finishes on its own 
 * this way we can continuously play songs like a radio
 * checks menu action actionShuffle
 * checks menu action actionPlay_All
 */
void MainWindow::playNext(int madret, QProcess::ExitStatus madstatus) 
{
	ui->statusbar->clearMessage();
	if (madstatus == QProcess::CrashExit) {
		ui->statusbar->showMessage("player crashed with exit code "+madret);
		return;
	}
	// if we are playing stop that
	if (isPlay) isPlay = false;

	// if we are not in the context of a source dont' do anything
	if (currSource == "") return;

	// otherwise check what our behavior is and maybe play another song
	if (ui->actionPlay_All->isChecked()) {
		QListWidgetItem *item = ui->listSelector->currentItem();
		// using text to check as its more exact
		if (item->text() != playing) {
			setItem(ui->listSelector->currentRow()); 
		} else if (ui->actionShuffle->isChecked()) {
			randomItem();
		} else {
			nextItem();
		}
		playStop();
	}
}

/**
 * slot to handle choosing a random item from the list
 */
void MainWindow::randomItem()
{
	time_t t;
	qsrand(time(&t));
	setItem(qrand() % ui->listSelector->count());
}

/**
 * slot to handle "down" gui button press
 * go to next item in list 
 * or go back to top if you hit the end
 */
void MainWindow::nextItem() 
{
	int next;
	next = ui->listSelector->currentRow();
	if (next < 0 || next >= ui->listSelector->count() - 1) {
		next = 0;
	} else {
		next++;
	}
	setItem(next);
}

/**
 * slot to handle the "up" gui button press
 * go to previous item or to end of list if at top
 */
void MainWindow::prevItem() 
{
	int prev;
	prev = ui->listSelector->currentRow();
	if (prev <= 0) {
		prev =  ui->listSelector->count() - 1;
	} else {
		prev--;
	}
	setItem(prev);
}

/**
 * stop playing if we are playing a file
 * change listSelector to match correct item
 */
void MainWindow::setItem(int row) 
{
//	if (isPlay) playStop();
	ui->listSelector->setCurrentRow(row);
}

/**
 * slot to either play a song or stop / pause it
 * if we are on a song it will play the song if 
 * changes text of play/stop button to be opposite of current state 
 * if we are on a directory will go into directory and make list contents of
 * subdirectory
 *
 * since we are doing the naive thing of piping from wget to madplay
 * pausing might be difficult for streamed songs
 * possible that madplay can handle this?
 */
void MainWindow::playStop() 
{
	QListWidgetItem * item;
	QFileInfo file;
	QString path;

	ui->statusbar->clearMessage();
	if (isPlay == true) {
		madplay->terminate();
		madplay->waitForFinished();
		stopsong->start("stopsong");
		isPlay = false;
		ui->buttonPlayStop->setText("Play");
	} else {
		item = ui->listSelector->currentItem();
		if (currSource == "") {
			currSource = item->text();
			displayListSelector();
			ui->buttonPlayStop->setText("Play");
		} else {
			isPlay = true;
			playing = item->text();
			madplay->start("playsong", QStringList() << item->text());
			if (!madplay->waitForStarted()) {
				ui->statusbar->showMessage("error playing: "+madplay->readAllStandardError());
			} else {
				ui->statusbar->showMessage("playing "+item->text());
				ui->buttonPlayStop->setText("Stop");
			}
		}
	}
}

/**
 * shares a single song or all songs on a device
 * how this is done is defined by the sharemedia script
 * all we do is take whatever we selected and send it to the script
 */
void MainWindow::shareMedia()
{
	QListWidgetItem * item = ui->listSelector->currentItem();

	// item is a source - shares everything it can find so ask first
	if (currSource == "") {
		int res = QMessageBox::question(
			this,"Share Media", "Share all media on " + item->text() + "?",
			QMessageBox::Ok || QMessageBox::Cancel, 2
		);
		if (res == 1) { // Ok is the first button 
			share(item->text());
		}
	} else {
		share(item->text());
	}
}
/**
 * clears all shared media - which are all symbolic links
 */
void MainWindow::unshareMedia()
{
	QProcess unsharemedia;

	unsharemedia.start("unsharemedia");
	unsharemedia.waitForFinished();
	QMessageBox::information(
		this,"Share Media","All shared media has been cleared."
	);
}	

/**
 * private method that actually initiates the sharing of a media file or source
 */
void MainWindow::share(QString item)
{
	QProcess sharemedia;
	QString shareresult;

	sharemedia.start("sharemedia", QStringList() << item);
	sharemedia.waitForFinished();
	shareresult = sharemedia.readAllStandardOutput();
	QMessageBox::information(
		this,"Share Media",shareresult
	);
}

