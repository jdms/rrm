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
	
/*
    gdl_porevolume = new QGridLayout( this );
    this->setLayout( gdl_porevolume )*/;
}

void PoreVolumeResultsForm::setResults( const std::vector< QColor >& colors, const std::vector< double >& values )
{

	
//	int number_of_regions = values.size();
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


