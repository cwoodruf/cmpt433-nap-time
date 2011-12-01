/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * Intercom application: respond to request for an intercom connection
 * this program is started from a web request
 *
 */

#include "intercom-confirm.h"
#include "ui_intercom-confirm.h"

extern "C" {
	#include <stdio.h>
}

/**
 * Intercom window constructor. 
 */
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	// Setup connections:
	QObject::connect(ui->btnConnect, SIGNAL(clicked()), this, SLOT(connectPeer()));
	QObject::connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(cancelPeer()));
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
 * Send confirmation to finish the connect process
 * the OK should get picked up by the calling application 
 */
void MainWindow::connectPeer(void)
{
	printf("OK");
	close();
}


/**
 * Send confirmation to finish the connect process
 */
void MainWindow::cancelPeer(void)
{
	printf("connection refused");
	close();
}

