#include "AboutWidget.hpp"

AboutWidget::AboutWidget( QWidget *parent ) : QDialog ( parent )
{

	ui = new Ui::AboutDialog;
	ui->setupUi ( this );
	setWindowFlags(Qt::Dialog);
}

AboutWidget::~AboutWidget( ) {
	delete ui;
}

