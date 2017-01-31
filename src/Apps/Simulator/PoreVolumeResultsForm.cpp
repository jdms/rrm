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

#include "PoreVolumeResultsForm.h"

PoreVolumeResultsForm::PoreVolumeResultsForm( QWidget* parent )
{
    createWindow();
}

void PoreVolumeResultsForm::createWindow()
{
	
    tbw_results = new QTableWidget(this);
    QHBoxLayout* hb_results = new QHBoxLayout();
    hb_results->addWidget( tbw_results );
	
    this->setLayout( hb_results );

}

void PoreVolumeResultsForm::setResults( const std::vector< QColor >& colors, const std::vector< double >& values )
{

    int number_of_regions = colors.size();
    tbw_results->clear();
    tbw_results->setRowCount( number_of_regions );
    tbw_results->setColumnCount( 3 );

	
	
	for( int i = 0; i < number_of_regions; ++i )
	{

        tbw_results->setColumnWidth( 0, 20 );
        tbw_results->setItem( i, 0, new QTableWidgetItem);
        tbw_results->item( i, 0 )->setBackground( colors[ i ] );

		
        QLabel* lb_text = new QLabel( QString( "Region %1: ").arg( i ) );
        tbw_results->setCellWidget( i, 1, lb_text );
		
        QLabel* lb_value = new QLabel( QString( "%1" ).arg( values[ i ] ) );
        tbw_results->setCellWidget( i, 2,  lb_value );
		
		
	}


    this->show();

}


