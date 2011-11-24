#include "PlaylistWindow.h"
#include "ui_gui_PlaylistCreate.h"
#include <QCloseEvent>

extern "C" {
    #include "buttondrv.h"
}

PlaylistWindow::PlaylistWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlaylistWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->backButton, SIGNAL(clicked(bool)), this, SLOT(close()));
}

PlaylistWindow::~PlaylistWindow()
{
    delete ui;
}

void PlaylistWindow::setButtonThread (ButtonThread* thread) {
    buttonThread = thread;
    QObject::connect(buttonThread, SIGNAL(buttonsChanged(int)), this, SLOT(setButtons(int)));
}

void PlaylistWindow::setButtons(int btnMask)
{
    if (btnMask > 0) {
        if (btnMask & (1<<BTN_BIT_UP)) {
            int currentRow = ui->listWidget->currentRow ();
            if (currentRow > 0) {
                ui->listWidget->setCurrentRow (currentRow - 1);
            }
        }
        if (btnMask & (1<<BTN_BIT_DOWN)) {
            int currentRow = ui->listWidget->currentRow ();
            if (currentRow < ui->listWidget->count () - 1) {
                ui->listWidget->setCurrentRow (currentRow + 1);
            }
        }
/*
        if (btnMask & (1<<BTN_BIT_LEFT))
            str += "Left ";
        if (btnMask & (1<<BTN_BIT_RIGHT))
            str += "Right ";*/
    }
}

void PlaylistWindow::closeEvent(QCloseEvent *event)
{
    QObject::disconnect(buttonThread, SIGNAL(buttonsChanged(int)), this, SLOT(setButtons(int)));
    QObject::connect(buttonThread, SIGNAL(buttonsChanged(int)), parentWidget (), SLOT(setButtons(int)));
    event->accept ();
}
