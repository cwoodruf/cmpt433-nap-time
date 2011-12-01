/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983 
 * this should pop up a question window and alert an end user that an
 * intercom connection is being requested
 */
#include <QtGui/QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
	QString name, host, ip;
	int res;

	QApplication a(argc, argv);
	if (argc < 4) {
		printf("error: missing arguments should have name host and ip");
	} else {
		name = QString(argv[1]);
		host = QString(argv[2]);
		ip = QString(argv[3]);

		res = QMessageBox::question(&a, "Connection Request", 
			name+"("+host+"/"+ip+") wants to speak to you. Click Ok to accept."
			QMessageBox::Cancel|QMessageBox::Ok);
		if (res == QMessageBox::Ok) {
			printf("OK");
		}
	}
	printf("connection refused");
	return a.exec();
}

