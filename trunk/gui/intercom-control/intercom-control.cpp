/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * Intercom application: window that lets a receiver close the connection
 * this program is started from a web request
 *
 */

#include <QProcess>
#include "intercom-control.h"
#include "ui_intercom-control.h"

/**
 * Intercom window constructor. 
 */
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	// Setup connections:
	QObject::connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(closePeer()));
}

/**
 * Intercom window destructor.
 */
MainWindow::~MainWindow()
{
	delete ui;
}

/**
 * make the label message more informative
 */
void MainWindow::setNameHostIp(char *name, char *host, char *ip) 
{
	QString lbl = ui->lblConnect->text();
	lbl.append("\nfrom "+QString(name)+" ("+QString(host)+"/"+QString(ip)+")");
	ui->lblConnect->setText(lbl);
}

/**
 * Shut down the rtp applications
 */
void MainWindow::closePeer(void)
{
	QProcess naprtpstop;
	naprtpstop.start("naprtpstop");
	naprtpstop.waitForFinished();
	close();
}

