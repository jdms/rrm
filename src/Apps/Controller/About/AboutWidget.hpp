#ifndef ABOUTWIDGET_HPP_
#define ABOUTWIDGET_HPP_

#include <QDialog>

#include "ui_AboutWidget.h"

class AboutWidget : public QDialog
{
		Q_OBJECT
	public:
		AboutWidget(QWidget *parent = 0);
		virtual ~AboutWidget();
	private:

		Ui::AboutDialog * ui;

};

#endif /* ABOUTWIDGET_HPP_ */
