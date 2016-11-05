#include "FormPropertyValues.h"

FormPropertyValues::FormPropertyValues( QWidget *parent )
{
    type = PROPERTY_TYPE::REGIONS;
    gl_layout = new QGridLayout();

    createButton();
}


FormPropertyValues::FormPropertyValues( PROPERTY_TYPE t, QWidget *parent )
{

    type = t;
    gl_layout = new QGridLayout();
    createButton();

}


void FormPropertyValues::createButton()
{

    btb_buttons = new QDialogButtonBox( QDialogButtonBox::Ok|QDialogButtonBox::Cancel );

    connect( btb_buttons, SIGNAL( accepted() ), this, SLOT( on_accept_values() ) );
    connect( btb_buttons, SIGNAL( rejected() ), this, SLOT( on_reject_values() ) );
}


void FormPropertyValues::reset()
{

    this->close();

    if( gl_layout->isEmpty() == false )
    {
        while(!gl_layout->isEmpty()) {
            QWidget *w = gl_layout->takeAt(0)->widget();
            delete w;
        }

        delete gl_layout;
        gl_layout = new QGridLayout();

        createButton();

    }


    vector_positions_values.clear();
    vector_perm_values.clear();
    vector_poros_values.clear();
    vector_visc_values.clear();

    vector_type_values.clear();
    vector_valuewell_values.clear();
    vector_sign_values.clear();


    number_of_regions = 0;
    number_of_wells = 0;


}



void FormPropertyValues::createRegionPropertiesForm( const int np )
{

    number_of_regions = np;

    vector_positions_values.clear();
    vector_perm_values.clear();
    vector_poros_values.clear();
    vector_visc_values.clear();

	vector_positions_values.resize(3 * np);
	vector_perm_values.resize(np);
	vector_poros_values.resize(np);
	vector_visc_values.resize(np);


	for (int i = 0; i < np; i++)
	{
		vector_positions_values[3 * i + 0] = new QLineEdit();
		vector_positions_values[3 * i + 1] = new QLineEdit();
		vector_positions_values[3 * i + 2] = new QLineEdit();
		vector_perm_values[i]  = new QLineEdit();
		vector_poros_values[i] = new QLineEdit();
		vector_visc_values[i]  = new QLineEdit();
	}


}


void FormPropertyValues::createRegionPropertiesForm( const int np, const std::vector< double >& positions , const std::vector< double >& perm, const std::vector< double >& poros, const std::vector< double >& visc )
{

    createRegionPropertiesForm( np );


    for( int i = 0; i < np; ++i )
    {
        QString value0 = QString( "%1" ).arg( positions[ 3*i ] );
        vector_positions_values[ 3*i ]->setText( value0 );

        QString value1 = QString( "%1" ).arg( positions[ 3*i + 1 ] );
        vector_positions_values[ 3*i + 2 ]->setText( value1 );

        QString value2 = QString( "%1" ).arg( positions[ 3*i + 2 ] );
        vector_positions_values[ 3*i + 1 ]->setText( value2 );


        QString perm_str = QString( "%1" ).arg( perm[ i ] );
        vector_perm_values[ i ]->setText( perm_str );

        QString poros_str = QString( "%1" ).arg( poros[ i ] );
        vector_poros_values[ i ]->setText( poros_str );

        QString visc_str = QString( "%1" ).arg( visc[ i ] );
        vector_visc_values[ i ]->setText( visc_str );
    }




}




void FormPropertyValues::createWellForm( const int nw )
{

    number_of_wells = nw;

    vector_type_values.clear();
    vector_valuewell_values.clear();
    vector_sign_values.clear();

	vector_type_values.resize(nw);
	vector_valuewell_values.resize(nw);
	vector_sign_values.resize(nw);

	for (int i = 0; i < nw; i++)
	{
		vector_type_values[i] = new QLineEdit();
		vector_valuewell_values[i] = new QLineEdit();
		vector_sign_values[i] = new QLineEdit();
	}
	    
}


void FormPropertyValues::createWellForm( const int nw, const std::vector< unsigned int >& type, const std::vector< double >& value, const std::vector< int >& sign )
{

    createWellForm( nw );

    for( int i = 0; i < nw; ++i )
    {

        QString type_str = QString( "%1" ).arg( type[ i ] );
        vector_type_values[ i ]->setText( type_str );

        QString value_str = QString( "%1" ).arg( value[ i ] );
        vector_valuewell_values[ i ]->setText( value_str );

        QString sign_str = QString( "%1" ).arg( sign[ i ] );
        vector_sign_values[ i ]->setText( sign_str );

    }


}





void FormPropertyValues::viewRegionPropertiesForm()
{

    if( gl_layout->isEmpty() == false )
    {
        while(!gl_layout->isEmpty()) {
            QWidget *w = gl_layout->takeAt(0)->widget();
            gl_layout->removeWidget( w );
        }

        delete gl_layout;
        gl_layout = new QGridLayout();

        createButton();

    }


    QFrame* line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    int nrows = (int)number_of_regions/2 + 4 ;

    for( int i = 0; i < number_of_regions; ++i )
    {
        int row = (int)i/2;
        int col = 15*( i%2 );

        QString name( "Region" );
        name = name + QString( " %1: " ).arg( i );

        QLabel* lb_name = new QLabel( name );

        gl_layout->addWidget( lb_name, row, col );

        gl_layout->addWidget( new QLabel( "Permeability (mD)"), row, col + 1 );
        gl_layout->addWidget( vector_perm_values[ i ], row, col + 2 );

        gl_layout->addWidget( new QLabel( "Porosity"), row, col + 3 );
        gl_layout->addWidget( vector_poros_values[ i ], row, col + 4 );

        gl_layout->addWidget( new QLabel( "Viscosity (cP)"), row, col + 5 );
        gl_layout->addWidget( vector_visc_values[ i ], row, col + 6 );

        gl_layout->addWidget( new QLabel( "X(m)"), row, col + 7 );
        gl_layout->addWidget( vector_positions_values[ 3*i ], row, col + 8 );

        gl_layout->addWidget( new QLabel( "Y(m)"), row, col + 9 );
        gl_layout->addWidget( vector_positions_values[ 3*i + 1 ], row, col + 10 );

        gl_layout->addWidget( new QLabel( "Z(m)"), row, col + 11 );
        gl_layout->addWidget( vector_positions_values[ 3*i + 2 ], row, col + 12 );

        if( col == 0 ){
            gl_layout->addWidget( new QLabel( "   "), row, col + 13 );
            gl_layout->addWidget( new QLabel( "   "), row, col + 14 );
        }

    }

    gl_layout->addWidget( line, nrows, 0 , 1, 27 );

    if( number_of_regions > 1 )
        gl_layout->addWidget( btb_buttons, nrows + 1, 24 , 1, 4 );
    else
        gl_layout->addWidget( btb_buttons, nrows + 1, 9 , 1, 4 );

    this->setLayout( gl_layout );
    this->show();


}


void FormPropertyValues::viewWellForm()
{

    if( gl_layout->isEmpty() == false )
    {
        while(!gl_layout->isEmpty()) {
            QWidget *w = gl_layout->takeAt(0)->widget();
            gl_layout->removeWidget( w );
        }

        delete gl_layout;
        gl_layout = new QGridLayout();

        createButton();

    }


    QFrame* line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    int nrows = (int)number_of_wells/2 + 4 ;

    for( int i = 0; i < number_of_wells; ++i )
    {
        int row = (int)i/2;
        int col = 9*( i%2 );

        QString name( "Well" );
        name = name + QString( " %1: " ).arg( i );

        QLabel* lb_name = new QLabel( name );

        gl_layout->addWidget( lb_name, row, col );

        gl_layout->addWidget( new QLabel( "Type"), row, col + 1 );
        gl_layout->addWidget( vector_type_values[ i ], row, col + 2 );

        gl_layout->addWidget( new QLabel( "Value (Bar)"), row, col + 3 );
        gl_layout->addWidget( vector_valuewell_values[ i ], row, col + 4 );

        gl_layout->addWidget( new QLabel( "Sign"), row, col + 5 );
        gl_layout->addWidget( vector_sign_values[ i ], row, col + 6 );


        if( col == 0 ){
            gl_layout->addWidget( new QLabel( "   "), row, col + 7 );
            gl_layout->addWidget( new QLabel( "   "), row, col + 8 );
        }

    }


    gl_layout->addWidget( line, nrows, 0 , 1, 17 );


    if( number_of_wells > 1 )
        gl_layout->addWidget( btb_buttons, nrows + 1, 12 , 1, 4 );
    else
        gl_layout->addWidget( btb_buttons, nrows + 1, 4 , 1, 4 );

    this->setLayout( gl_layout );
    this->show();
}




std::vector< double > FormPropertyValues::getRegionPositions()
{

    std::vector< double > positions;

    for( int i = 0; i < number_of_regions; ++i )
    {
        positions.push_back( vector_positions_values[ 3*i ]->text().toDouble() );
        positions.push_back( vector_positions_values[ 3*i + 1 ]->text().toDouble() );
        positions.push_back( vector_positions_values[ 3*i + 2 ]->text().toDouble() );
    }
    return positions;

}


std::vector< double > FormPropertyValues::getRegionPermeability()
{
    std::vector< double > permeability;

    for( int i = 0; i < number_of_regions; ++i )
    {
        permeability.push_back( vector_perm_values[ i ]->text().toDouble() );
    }
    return permeability;
}


std::vector< double > FormPropertyValues::getRegionPorosity()
{
    std::vector< double > porosity;

    for( int i = 0; i < number_of_regions; ++i )
    {
        porosity.push_back( vector_poros_values[ i ]->text().toDouble() );
    }
    return porosity;
}


std::vector< double > FormPropertyValues::getRegionViscosity()
{
    std::vector< double > viscosity;

    for( int i = 0; i < number_of_regions; ++i )
    {
        viscosity.push_back( vector_visc_values[ i ]->text().toDouble() );
    }
    return viscosity;
}




std::vector< unsigned int > FormPropertyValues::getWellTypes()
{

    std::vector< unsigned int > types;

    for( int i = 0; i < number_of_wells; ++i )
    {
        types.push_back( vector_type_values[ i ]->text().toUInt() );
    }

    return types;

}


std::vector< double > FormPropertyValues::getWellValues()
{
    std::vector< double > values;

    for( int i = 0; i < number_of_wells; ++i )
    {
        values.push_back( vector_valuewell_values[ i ]->text().toDouble() );
    }

    return values;
}


std::vector< int > FormPropertyValues::getWellSigns()
{
    std::vector< int > signs;

    for( int i = 0; i < number_of_wells; ++i )
    {
        signs.push_back( vector_sign_values[ i ]->text().toInt() );
    }

    return signs;
}



void FormPropertyValues::on_accept_values()
{

    this->close();

}


void FormPropertyValues::on_reject_values()
{
    reset();
    this->close();
}
