#include "MainWindow.h"

MainWindow::MainWindow( QWidget *parent) : QMainWindow( parent )
{
    createWindow();
}

void MainWindow::createWindow()
{
     this->setMinimumSize( 800, 600 );
}
