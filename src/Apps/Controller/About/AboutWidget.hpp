#ifndef ABOUTWIDGET_HPP_
#define ABOUTWIDGET_HPP_


/**
 * @file AboutWidget.h
 * @author Felipe Moura de Carvalho
 * @date $Date: 2005/04/14 14:16:20
 * @brief RRM's Main GUI.
 */

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
