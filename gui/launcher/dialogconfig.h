/**
 * cmpt433 nap time systems Cal Woodruff <cwoodruf@sfu.ca> 301013983
 * 
 * Defines the configuration dialog for the launcher window.
 * This explains how to use a web browser to configure the system
 * and lets the end user view other nap hosts via napping.
 */
#ifndef DIALOGCONFIG_H
#define DIALOGCONFIG_H

#include <QProcess>
#include <QDialog>
#include <QString>

namespace Ui {
	class DialogConfig;
}

class DialogConfig : public QDialog
{
	Q_OBJECT

public:
	explicit DialogConfig(QWidget *parent = 0);
	~DialogConfig();

public slots:
	void runNapping(void);
	void getNappingOutput(void);

private:
	Ui::DialogConfig *ui;
	QProcess *napping;
	void refreshConfig(QString msg = "");
};

#endif // DIALOGCONFIG_H

