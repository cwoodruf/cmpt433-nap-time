/**
 * Alternate version of the player app that includes 
 * finding songs on devices
 */
#include "player.h"
#include "ui_player.h"
#include <QDebug>
#include <QProcess>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	madplay(new QProcess),
	stopsong(new QProcess)
{
	isPlay = false;
	currIndex = prevIndex = -1;
	currDir = QString();

	ui->setupUi(this);
	displayListSelector();

	connect(ui->listSelector, SIGNAL(currentRowChanged(int)), this, SLOT(currentSongChanged(int)));
	connect(ui->buttonUp, SIGNAL(pressed()), this, SLOT(prevItem()));
	connect(ui->buttonDown, SIGNAL(pressed()), this, SLOT(nextItem()));
	connect(ui->buttonPlayStop, SIGNAL(clicked(bool)), this, SLOT(playStop()));
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
 * initially this would be a list of devices
 */
void MainWindow::displayListSelector() 
{
}

/**
 * slot to handle "down" gui button press
 * go to next item in list 
 * or go back to top if you hit the end
 */
void MainWindow::nextItem() 
{
	int next;
	if (currIndex == -1) return;
	next = ui->listSelector->currentRow();
	if (next >= ui->listSelector->count() - 1) {
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
	if (currIndex == -1) return;
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
	if (isPlay) playStop();
	prevItem = currItem;
	currItem = row;
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
	// no list yet...
	if (currIndex == -1) {
		return;
	}
	if (isPlay == true) {
		madplay->terminate();
		madplay->waitForFinished();
		stopsong->start("stopsong");
		isPlay = false;
	} else {
// todo: handle the external nodes on the network?
		if (currDir.equals("")) {
			return;
		} 

		item = ui->listSelector->currentItem();
		path = QString(currDir+"/"+item->text());
		file = QFileInfo(path);

		if (file.isDir()) {
			currDir = path;
			displayListSelector();
		} else {
			isPlay = true;
			madplay->start("playsong", QStringList() << path);
			if (!madplay->waitForStarted())
				qDebug() << "cannot start madplay process";
		}
	}
}

