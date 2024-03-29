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

#include "about_widget.hpp"
#include "version.h"

AboutWidget::AboutWidget ( QWidget *parent ) : QDialog ( parent )
{
	// TODO Auto-generated constructor stub
	ui = new Ui::AboutDialog;
	ui->setupUi ( this );
	auto about_text = ui->about_label->text();
	ui->about_label->setText(about_text.arg(RRM_VERSION_FULL));
	setWindowFlags(Qt::Dialog);
}

AboutWidget::~AboutWidget ( )
{
	// TODO Auto-generated destructor stub
	delete ui;
}


#include "moc_about_widget.cpp"
