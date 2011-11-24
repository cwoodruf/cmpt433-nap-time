/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * references:
 * http://www.qtforum.org/article/3079/howto-start-an-external-program-from-a-qt-application.html
 * http://doc.qt.nokia.com/latest/qprocess.html
 * 
 * network IP stuff
 * http://www.qtcentre.org/archive/index.php/t-14502.html
 * http://doc.trolltech.com/4.4/qnetworkinterface.html
 * http://doc.trolltech.com/4.4/qhostaddress.html
 * http://doc.trolltech.com/4.4/qnetworkaddressentry.html
 *
 */

#include <QProcess>
#include <QTextBrowser>
#include <QString>
#include <QByteArray>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QHostAddress>
#include <QList>
#include "dialogconfig.h"
#include "ui_dialogconfig.h"

/**
 * Launcher constructor. Sets up ui. Also starts thread that listens 
 * for intercomm connections. Should also check that naplistener is 
 * running and if not start it.
 */
DialogConfig::DialogConfig(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogConfig),
	napping(new QProcess)
{
	ui->setupUi(this);
	refreshConfig();

	// Set up connections:
	QObject::connect(ui->btnNapping, SIGNAL(clicked()), this, SLOT(runNapping()));
	QObject::connect(napping, SIGNAL(readyReadStandardOutput()), this, SLOT(getNappingOutput()));
}

DialogConfig::~DialogConfig()
{
	if (napping->state() != QProcess::NotRunning) napping->terminate();
	delete ui;
}

void DialogConfig::refreshConfig(QString msg) 
{
	QString html;
	QNetworkInterface iface;
	QList<QNetworkAddressEntry> entries;
	QHostAddress addr;
	QProcess getparams;

	if (msg.length() > 0) {
		html.append(msg + "<br>");
	}

	html.append("<h4>Configuration</h4>");
	iface = QNetworkInterface::interfaceFromName("eth0");
	entries = iface.addressEntries();
	addr = entries[0].ip();
	html.append("Configuration link:<br>http://"+addr.toString()+"/config.cgi");
	getparams.start("/bin/getparams");
	getparams.waitForFinished();
	html.append("<br>Parameters:<br><pre>"+getparams.readAllStandardOutput()+"</pre>");
	ui->textBrowserConfig->setHtml(html);
}

void DialogConfig::getNappingOutput(void) 
{
	QString out(napping->readAllStandardOutput());
	refreshConfig(out);
}

void DialogConfig::runNapping(void)
{
	if (napping->state() == QProcess::NotRunning) {
		napping->start("/bin/getpeers");
	} 
}


