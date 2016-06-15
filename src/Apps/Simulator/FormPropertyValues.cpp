#include "FormPropertyValues.h"

FormPropertyValues::FormPropertyValues( QWidget *parent )
{
    number_of_values = 0;
    type = PROPERTY_TYPE::SINGLEVALUE;
    gl_layout = new QGridLayout();
    hb_layout_toftracer = new QHBoxLayout();
    vb_layout_toftracer = new QVBoxLayout();

    createButton();
}


FormPropertyValues::FormPropertyValues( PROPERTY_TYPE t, std::string name, QWidget *parent )
{
    number_of_values = 0;
    type = t;
    name_of_property = name;
    gl_layout = new QGridLayout();

    hb_layout_toftracer = new QHBoxLayout();
    vb_layout_toftracer = new QVBoxLayout();

    createButton();
}


void FormPropertyValues::create()
{
    if( number_of_values == 0 ) return;

    if( type == PROPERTY_TYPE::SINGLEVALUE )
        createSingleValueForm();
    else if( type == PROPERTY_TYPE::WELL )
        createWellForm();
    else if( type == PROPERTY_TYPE::BOUNDARY )
        createBoundaryForm();
    else if( type == PROPERTY_TYPE::TOFANDTRACER )
        createTofandTracerForm();

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


    vector_edt_values.clear();
    vector_values.clear();
    number_of_values = 0;


}

void FormPropertyValues::setValues( vector<double> &v )
{

    if( vector_values.empty() == false )
        reset();

//    if( type == PROPERTY_TYPE::SINGLEVALUE )
//        number_of_values = (int) v.size()/6;

//    else if( type == PROPERTY_TYPE::BOUNDARY )
//    {
//        number_of_values = (int) v.size();

//    }
//    else if( type == PROPERTY_TYPE::WELL )
//        number_of_values = (int) v.size()/6;

//    else if( type == PROPERTY_TYPE::TOFANDTRACER )
//        number_of_values = (int) ( v.size() - 1 )/2;

    vector_values = v;

}


void FormPropertyValues::setValues( vector< int > &v )
{
    if( vector_values.empty() == false )
        reset();


//    if( type == PROPERTY_TYPE::SINGLEVALUE )
//        number_of_values = (int) v.size()/6;

//    else if( type == PROPERTY_TYPE::BOUNDARY )
//    {
//        number_of_values = (int) v.size();
//    }
//    else if( type == PROPERTY_TYPE::WELL )
//        number_of_values = (int) v.size()/6;

//    else if( type == PROPERTY_TYPE::TOFANDTRACER )
//        number_of_values = (int) ( v.size() - 1 )/2;


    std::size_t nvalues = v.size();
    vector_values.resize( nvalues );

    for( std::size_t it = 0; it < nvalues; ++it )
        vector_values[ it ] = (float) v[ it ];

}



void FormPropertyValues::createSingleValueForm()
{

    vector_edt_values.clear();
    vector_edt_values.resize( 6*number_of_values );


    if( vector_values.empty() == true )
    {
        for( int i = 0; i < number_of_values; ++i )
        {
            vector_edt_values[ 6*i ] = new QLineEdit;
            vector_edt_values[ 6*i + 1 ] = new QLineEdit;
            vector_edt_values[ 6*i + 2 ] = new QLineEdit;
            vector_edt_values[ 6*i + 3 ] = new QLineEdit;
            vector_edt_values[ 6*i + 4 ] = new QLineEdit;
            vector_edt_values[ 6*i + 5 ] = new QLineEdit;
        }
    }
    else if( vector_values.size() != vector_edt_values.size() )
        return;
    else
    {

        for( int i = 0, j = 0; i < number_of_values; ++i )
        {
            vector_edt_values[ 6*i ] = new QLineEdit;
            QString value0 = QString( "%1" ).arg( vector_values[ 6*i ] );
            vector_edt_values[ 6*i ]->setText( value0 );

            vector_edt_values[ 6*i + 1 ] = new QLineEdit;
            QString value1 = QString( "%1" ).arg( vector_values[ 6*i + 1 ] );
            vector_edt_values[ 6*i + 1 ]->setText( value1 );

            vector_edt_values[ 6*i + 2 ] = new QLineEdit;
            QString value2 = QString( "%1" ).arg( vector_values[ 6*i + 2 ] );
            vector_edt_values[ 6*i + 2 ]->setText( value2 );

            vector_edt_values[ 6*i + 3 ] = new QLineEdit;
            QString value3 = QString( "%1" ).arg( vector_values[ 6*i + 3 ] );
            vector_edt_values[ 6*i + 3 ]->setText( value3 );

            vector_edt_values[ 6*i + 4 ] = new QLineEdit;
            QString value4 = QString( "%1" ).arg( vector_values[ 6*i + 4 ] );
            vector_edt_values[ 6*i + 4 ]->setText( value4 );

            vector_edt_values[ 6*i + 5 ] = new QLineEdit;
            QString value5 = QString( "%1" ).arg( vector_values[ 6*i + 5 ] );
            vector_edt_values[ 6*i + 5 ]->setText( value5 );
        }

    }


}


void FormPropertyValues::createWellForm()
{

    vector_edt_values.clear();
    vector_edt_values.resize( 6*number_of_values );


    if( vector_values.empty() == true )
    {
        for( int i = 0; i < number_of_values; ++i )
        {
            vector_edt_values[ 6*i ] = new QLineEdit;
            vector_edt_values[ 6*i + 1 ] = new QLineEdit;
            vector_edt_values[ 6*i + 2 ] = new QLineEdit;
            vector_edt_values[ 6*i + 3 ] = new QLineEdit;
            vector_edt_values[ 6*i + 4 ] = new QLineEdit;
            vector_edt_values[ 6*i + 5 ] = new QLineEdit;

        }
    }
    else if( vector_values.size() != vector_edt_values.size() )
        return;
    else
    {
        for( int i = 0; i < number_of_values; ++i )
        {

            vector_edt_values[ 6*i ] = new QLineEdit;
            QString value0 = QString( "%1" ).arg( vector_values[ 6*i ] );
            vector_edt_values[ 6*i ]->setText( value0 );

            vector_edt_values[ 6*i + 1 ] = new QLineEdit;
            QString value1 = QString( "%1" ).arg( vector_values[ 6*i + 1 ] );
            vector_edt_values[ 6*i + 1 ]->setText( value1 );

            vector_edt_values[ 6*i + 2 ] = new QLineEdit;
            QString value2 = QString( "%1" ).arg( vector_values[ 6*i + 2 ] );
            vector_edt_values[ 6*i + 2 ]->setText( value2 );

            vector_edt_values[ 6*i + 3 ] = new QLineEdit;
            QString value3 = QString( "%1" ).arg( vector_values[ 6*i + 3 ] );
            vector_edt_values[ 6*i + 3 ]->setText( value3 );

            vector_edt_values[ 6*i + 4 ] = new QLineEdit;
            QString value4 = QString( "%1" ).arg( vector_values[ 6*i + 4 ] );
            vector_edt_values[ 6*i + 4 ]->setText( value4 );

            vector_edt_values[ 6*i + 5 ] = new QLineEdit;
            QString value5 = QString( "%1" ).arg( vector_values[ 6*i + 5 ] );
            vector_edt_values[ 6*i + 5 ]->setText( value5 );

        }
    }

}


void FormPropertyValues::createBoundaryForm()
{


    vector_edt_values.clear();
    vector_edt_values.resize( 4*number_of_values );


    if( vector_values.empty() == true )
    {

        for( int i = 0; i < number_of_values; ++i )
        {
            vector_edt_values[ 4*i ] = new QLineEdit;
            vector_edt_values[ 4*i + 1 ] = new QLineEdit;
            vector_edt_values[ 4*i + 2 ] = new QLineEdit;
            vector_edt_values[ 4*i + 3 ] = new QLineEdit;
        }

    }
//    else if( 4*vector_values.size() != vector_edt_values.size() )
//        return;
    else
    {

        for( int i = 0, j = 0; i < number_of_values; ++i )
        {
            vector_edt_values[ 4*i ] = new QLineEdit;
            vector_edt_values[ 4*i + 1 ] = new QLineEdit;
            vector_edt_values[ 4*i + 2 ] = new QLineEdit;
            vector_edt_values[ 4*i + 3 ] = new QLineEdit;


            vector_edt_values[ 4*i ]->setText( QString( "%1" ).arg( vector_values[ j ] ) );

            if( vector_values[ j ] == 1 )
            {
                vector_edt_values[ 4*i + 1 ]->setText( QString( "%1" ).arg( vector_values[ j + 1 ] ) );
                j += 2;
            }
            else if( vector_values[ j ] == 2 )
            {
                vector_edt_values[ 4*i + 1 ]->setText( QString( "%1" ).arg( vector_values[ j + 2 ] ) );
                vector_edt_values[ 4*i + 2 ]->setText( QString( "%1" ).arg( vector_values[ j + 3 ] ) );
                vector_edt_values[ 4*i + 3 ]->setText( QString( "%1" ).arg( vector_values[ j + 4 ] ) );
                j += 4;
            }
            else
                j++;

        }

    }

}


void FormPropertyValues::createTofandTracerForm()
{


    vector_edt_values.clear();
    vector_edt_values.resize( 2*number_of_values + 1 );


    if( vector_values.empty() == true )
    {

        vector_edt_values[ 0 ] = new QLineEdit;

        for( int i = 0; i < number_of_values; ++i )
        {
            vector_edt_values[ 2*i + 1 ] = new QLineEdit;
            vector_edt_values[ 2*i + 2 ] = new QLineEdit;

        }
    }
    else if( vector_values.size() != vector_edt_values.size() )
        return;
    else
    {

        vector_edt_values[ 0 ] = new QLineEdit;
        vector_edt_values[ 0 ]->setText( QString( "%1" ).arg( vector_values[ 0 ] ) );

        for( int i = 0; i < number_of_values; ++i )
        {

            vector_edt_values[ 2*i + 1 ] = new QLineEdit;
            vector_edt_values[ 2*i + 1 ]->setText( QString( "%1" ).arg( vector_values[ 2*i + 1 ] ) );
            vector_edt_values[ 2*i + 2 ] = new QLineEdit;
            vector_edt_values[ 2*i + 2 ]->setText( QString( "%1" ).arg( vector_values[ 2*i + 2] ) );

        }
    }

}


void FormPropertyValues::viewSingleValueForm()
{
    if( gl_layout->isEmpty() == false )
    {
        while(!gl_layout->isEmpty()) {
            QWidget *w = gl_layout->takeAt(0)->widget();
            delete w;
        }

        delete gl_layout;
        gl_layout = new QGridLayout();

        createButton();

        createSingleValueForm();
    }


    QFrame* line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    int nrows = (int)number_of_values/2 + 4 ;

    for( int i = 0; i < number_of_values; ++i )
    {
        int row = (int)i/2;
        int col = 15*( i%2 );

        QString name = name_of_property.c_str();
        name = name + QString( " %1: " ).arg( i );

        QLabel* lb_name = new QLabel( name );

        gl_layout->addWidget( lb_name, row, col );

        gl_layout->addWidget( new QLabel( "Permeability "), row, col + 1 );
        gl_layout->addWidget( vector_edt_values[ 6*i ], row, col + 2 );

        gl_layout->addWidget( new QLabel( "Porosity"), row, col + 3 );
        gl_layout->addWidget( vector_edt_values[ 6*i + 1 ], row, col + 4 );

        gl_layout->addWidget( new QLabel( "Viscosity "), row, col + 5 );
        gl_layout->addWidget( vector_edt_values[ 6*i + 2 ], row, col + 6 );

        gl_layout->addWidget( new QLabel( "X "), row, col + 7 );
        gl_layout->addWidget( vector_edt_values[ 6*i + 3 ], row, col + 8 );

        gl_layout->addWidget( new QLabel( "Y "), row, col + 9 );
        gl_layout->addWidget( vector_edt_values[ 6*i + 4 ], row, col + 10 );

        gl_layout->addWidget( new QLabel( "Z "), row, col + 11 );
        gl_layout->addWidget( vector_edt_values[ 6*i + 5 ], row, col + 12 );

        if( col == 0 ){
            gl_layout->addWidget( new QLabel( "   "), row, col + 13 );
            gl_layout->addWidget( new QLabel( "   "), row, col + 14 );
        }

    }

    gl_layout->addWidget( line, nrows, 0 , 1, 27 );

    if( number_of_values > 1 )
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
            delete w;
        }

        delete gl_layout;
        gl_layout = new QGridLayout();

        createButton();

        createWellForm();
    }


    QFrame* line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    int nrows = (int)number_of_values/2 + 4 ;

    for( int i = 0; i < number_of_values; ++i )
    {
        int row = (int)i/2;
        int col = 15*( i%2 );

        QString name = name_of_property.c_str();
        name = name + QString( " %1: " ).arg( i );

        QLabel* lb_name = new QLabel( name );

        gl_layout->addWidget( lb_name, row, col );

        gl_layout->addWidget( new QLabel( "Type "), row, col + 1 );
        gl_layout->addWidget( vector_edt_values[ 6*i ], row, col + 2 );

        gl_layout->addWidget( new QLabel( "Sign "), row, col + 3 );
        gl_layout->addWidget( vector_edt_values[ 6*i + 1 ], row, col + 4 );

        gl_layout->addWidget( new QLabel( "Value"), row, col + 5 );
        gl_layout->addWidget( vector_edt_values[ 6*i + 2 ], row, col + 6 );

        gl_layout->addWidget( new QLabel( "X "), row, col + 7 );
        gl_layout->addWidget( vector_edt_values[ 6*i + 3 ], row, col + 8 );

        gl_layout->addWidget( new QLabel( "Y "), row, col + 9 );
        gl_layout->addWidget( vector_edt_values[ 6*i + 4 ], row, col + 10 );

        gl_layout->addWidget( new QLabel( "Z "), row, col + 11 );
        gl_layout->addWidget( vector_edt_values[ 6*i + 5 ], row, col + 12 );

        if( col == 0 ){
            gl_layout->addWidget( new QLabel( "   "), row, col + 13 );
            gl_layout->addWidget( new QLabel( "   "), row, col + 14 );
        }

    }


    gl_layout->addWidget( line, nrows, 0 , 1, 29 );


    if( number_of_values > 1 )
        gl_layout->addWidget( btb_buttons, nrows + 1, 24 , 1, 4 );
    else
        gl_layout->addWidget( btb_buttons, nrows + 1, 9 , 1, 4 );

    this->setLayout( gl_layout );
    this->show();
}


void FormPropertyValues::viewBoundaryForm()
{

    if( gl_layout->isEmpty() == false )
    {

        while(!gl_layout->isEmpty()) {
            QWidget *w = gl_layout->takeAt(0)->widget();
            delete w;
        }

        delete gl_layout;
        gl_layout = new QGridLayout();

        createButton();

        createBoundaryForm();
    }

    int nrows = (int)number_of_values/2 + 4 ;

    QFrame* line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    for( int i = 0; i < number_of_values; ++i )
    {
        int row = (int)i/2;
        int col = 11*( i%2 );



        QString name = name_of_property.c_str();
        name = name + QString( " %1: " ).arg( i );

        QLabel* lb_name = new QLabel( name );


        gl_layout->addWidget( lb_name, row, col );

        gl_layout->addWidget( new QLabel( "Type "), row, col + 1 );
        gl_layout->addWidget( vector_edt_values[ 4*i ], row, col + 2 );

        gl_layout->addWidget( new QLabel( "UX"), row, col + 3 );
        gl_layout->addWidget( vector_edt_values[ 4*i + 1 ], row, col + 4 );

        gl_layout->addWidget( new QLabel( "UY"), row, col + 5 );
        gl_layout->addWidget( vector_edt_values[ 4*i + 2 ], row, col + 6 );

        gl_layout->addWidget( new QLabel( "UZ"), row, col + 7 );
        gl_layout->addWidget( vector_edt_values[ 4*i + 3 ], row, col + 8 );


        if( col == 0 ){
            gl_layout->addWidget( new QLabel( "   "), row, col + 9 );
            gl_layout->addWidget( new QLabel( "   "), row, col + 10 );
        }
    }



    gl_layout->addWidget( line, nrows, 0 , 1, 21 );

    if( number_of_values > 1 )
        gl_layout->addWidget( btb_buttons, nrows + 1, 16 , 1, 4 );
    else
        gl_layout->addWidget( btb_buttons, nrows + 1, 6 , 1, 4 );

    this->setLayout( gl_layout );

    this->show();
}



void FormPropertyValues::viewTofandTracerForm()
{
    if( vb_layout_toftracer->isEmpty() == false )
    {

        while(!gl_layout->isEmpty()) {
            QWidget *w = gl_layout->takeAt(0)->widget();
            delete w;
        }

        delete gl_layout;
        gl_layout = new QGridLayout();

        while(!hb_layout_toftracer->isEmpty()) {
            QWidget *w = hb_layout_toftracer->takeAt(0)->widget();
            delete w;
        }
        delete hb_layout_toftracer;
        hb_layout_toftracer = new QHBoxLayout();

        while(!vb_layout_toftracer->isEmpty()) {
            QWidget *w = vb_layout_toftracer->takeAt(0)->widget();
            delete w;
        }
        delete vb_layout_toftracer;
        vb_layout_toftracer = new QVBoxLayout();

        createButton();

        createTofandTracerForm();
    }


    int nrows = (int)number_of_values/2 + 4 ;

    vector_edt_values[ 0 ]->setMaximumWidth( 120 );


    hb_layout_toftracer->addWidget( new QLabel( "Sign") );
    hb_layout_toftracer->addWidget( vector_edt_values[ 0 ] );
    hb_layout_toftracer->addStretch();

    QFrame* line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    for( int i = 0; i < number_of_values; ++i )
    {
        int row = (int)i/2 ;
        int col = 7*( i%2 );


        QString name = name_of_property.c_str();
        name = name + QString( " %1: " ).arg( i );

        QLabel* lb_name = new QLabel( name );

        gl_layout->addWidget( lb_name, row, col );

        gl_layout->addWidget( new QLabel( "Type"), row, col + 1 );
        gl_layout->addWidget( vector_edt_values[ 2*i + 1 ], row, col + 2 );

        gl_layout->addWidget( new QLabel( "Mark"), row, col + 3 );
        gl_layout->addWidget( vector_edt_values[ 2*i + 2 ], row, col + 4 );

        if( col == 0 ){
            gl_layout->addWidget( new QLabel( "   "), row, col + 5 );
            gl_layout->addWidget( new QLabel( "   "), row, col + 6 );
        }
    }


    gl_layout->addWidget( line, nrows , 0 , 1, 13 );

    if( number_of_values > 1 )
        gl_layout->addWidget( btb_buttons, nrows + 1, 8 , 1, 4 );
    else
        gl_layout->addWidget( btb_buttons, nrows + 1, 2 , 1, 4 );



    QFrame* line1 = new QFrame();
    line1->setObjectName(QString::fromUtf8("line"));
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);

    vb_layout_toftracer->addLayout( hb_layout_toftracer );
    vb_layout_toftracer->addWidget( line1 );
    vb_layout_toftracer->addLayout( gl_layout );


    this->setLayout( vb_layout_toftracer );

    this->show();
}



vector< int > FormPropertyValues::getValues()
{
    std::size_t nvalues = vector_values.size();
    vector< int > v;
    v.resize( nvalues );

    for( std::size_t it = 0; it < nvalues; ++it )
        v[ it ] = (int) vector_values[ it ];

    return v;
}


void FormPropertyValues::on_accept_values()
{

    std::size_t total_number_values = vector_edt_values.size();
    vector_values.clear();
    vector_values.resize( total_number_values );


    for( std::size_t it = 0; it < total_number_values; ++it )
    {
        float value = vector_edt_values.at( it )->text().toFloat();
        vector_values[ it ] = value;
    }

    this->close();

}


void FormPropertyValues::on_reject_values()
{
    reset();
    this->close();
}
