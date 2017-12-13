#ifndef _ABOUTWIDGET_HPP_
#define _ABOUTWIDGET_HPP_

#include <QtWidgets/QDialog>

#include "ui_AboutWidget.h"

class AboutWidget: public QDialog
{
		Q_OBJECT
	public:
		AboutWidget ( QWidget *parent = 0 );
		virtual ~AboutWidget ( );
	private:

		Ui::AboutDialog * ui;

};

#endif /* _ABOUTWIDGET_HPP_ */
