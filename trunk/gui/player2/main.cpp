/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983 
 */
#include <QtGui/QApplication>
#include "player.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;

	if (IS_EM2440) w.showMaximized();
	else w.showNormal();

	return a.exec();
}

