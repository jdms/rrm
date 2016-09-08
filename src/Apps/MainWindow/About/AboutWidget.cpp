#include "AboutWidget.hpp"

AboutWidget::AboutWidget ( QWidget *parent ) : QDialog ( parent )
{
	// TODO Auto-generated constructor stub
	ui = new Ui::AboutDialog;
	ui->setupUi ( this );
	setWindowFlags(Qt::Dialog);
}

AboutWidget::~AboutWidget ( )
{
	// TODO Auto-generated destructor stub
	delete ui;
}

