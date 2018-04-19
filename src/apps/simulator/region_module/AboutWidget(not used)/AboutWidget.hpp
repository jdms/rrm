/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */


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
