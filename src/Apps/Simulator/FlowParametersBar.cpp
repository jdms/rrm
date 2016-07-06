#include "FlowParametersBar.h"
#include "ui_FlowParametersBar.h"

FlowParametersBar::FlowParametersBar(QWidget *parent) :
    QWidget(parent)
{
    setupUi( this );

    this->setLayout( vl_mainlayout );

    createDialogs();
}

void FlowParametersBar::createDialogs()
{

    btn_inputparameters->setIcon(QIcon(":/images/icons/folder.png"));
    btn_boundariesvalues->setIcon(QIcon(":/images/icons/pencil--plus.png"));
    btn_permeabilityvalues->setIcon(QIcon(":/images/icons/pencil--plus.png"));
    btn_tfboundaryvalues->setIcon(QIcon(":/images/icons/pencil--plus.png"));
    btn_trboundaryvalues->setIcon(QIcon(":/images/icons/pencil--plus.png"));
    btn_wellsvalues->setIcon(QIcon(":/images/icons/pencil--plus.png"));


    dg_permeability = new FormPropertyValues( FormPropertyValues::PROPERTY_TYPE::SINGLEVALUE, "Property Area", this );
    dg_boundaries = new FormPropertyValues( FormPropertyValues::PROPERTY_TYPE::BOUNDARY, "Surfaces Boundaries", this );
    dg_wells = new FormPropertyValues( FormPropertyValues::PROPERTY_TYPE::WELL, "Wells", this );
    dg_tofboundary = new FormPropertyValues( FormPropertyValues::PROPERTY_TYPE::TOFANDTRACER, "TOF Boundary", this );
    dg_trboundary = new FormPropertyValues( FormPropertyValues::PROPERTY_TYPE::TOFANDTRACER, "Tracer Boundary", this );

    connect( sp_nopermeabilityvalues, SIGNAL( valueChanged(int) ), dg_permeability, SLOT( reset() ) );
    connect( sp_noboundaries, SIGNAL( valueChanged(int) ), dg_boundaries, SLOT( reset() ) );
    connect( sp_nowells, SIGNAL( valueChanged(int) ), dg_wells, SLOT( reset() ) );
    connect( sp_notfboundaryvalues, SIGNAL( valueChanged(int) ), dg_tofboundary, SLOT( reset() ) );
    connect( sp_notrboundaryvalues, SIGNAL( valueChanged(int) ), dg_trboundary, SLOT( reset() ) );


    connect( dg_permeability, &FormPropertyValues::sendPropertyValues, this, [=]( int n, std::vector<double>& v ){ emit sendPropertyArea( n, v ); }  ) ;
    connect( dg_boundaries, &FormPropertyValues::sendPropertyValues, this, [=]( int n, std::vector<double>& v ){ emit sendBoundariesValues( n, v ); }  ) ;
    connect( dg_wells, &FormPropertyValues::sendPropertyValues, this, [=]( int n, std::vector<double>& v ){ emit sendWellsValues( n, v ); }  ) ;\
    connect( dg_tofboundary, &FormPropertyValues::sendPropertyValues, this, [=]( int n, vector<double>& v ){ emit sendTOFBoundaryValues( n, v ); }  ) ;
    connect( dg_trboundary, &FormPropertyValues::sendPropertyValues, this, [=]( int n, vector<double>& v ){ emit sendTrBoundaryValues( n, v ); }  ) ;


}



void FlowParametersBar::setPropertyAreaParameters( int np, std::vector< double > values )
{
    sp_nopermeabilityvalues->setValue( np );
    dg_permeability->setNumberofValues( np );
    dg_permeability->setValues( values );
    dg_permeability->create();
}


void FlowParametersBar::setSurfaceBoundariesParameter( int nbound, std::vector< double > vbound )
{
    sp_noboundaries->setValue( nbound );
    dg_boundaries->setNumberofValues( nbound );
    dg_boundaries->setValues( vbound );
    dg_boundaries->create();
}


void FlowParametersBar::setWellParameter( int nw, std::vector< double > vwell )
{
    sp_nowells->setValue( nw );
    dg_wells->setNumberofValues( nw );
    dg_wells->setValues( vwell );
    dg_wells->create();
}


void FlowParametersBar::setTofBoundaryParameter( int ntfbound, std::vector< int > vtfbound )
{
    sp_notfboundaryvalues->setValue( ntfbound );
    dg_tofboundary->setNumberofValues( ntfbound );
    dg_tofboundary->setValues( vtfbound );
    dg_tofboundary->create();
}


void FlowParametersBar::setTracerBoundaryParameter( int ntrbound, std::vector< int > vtrbound )
{
    sp_notrboundaryvalues->setValue( ntrbound );
    dg_trboundary->setNumberofValues( ntrbound );
    dg_trboundary->setValues( vtrbound );
    dg_trboundary->create();
}


void FlowParametersBar::on_btn_permeabilityvalues_clicked()
{
    int value_sp = sp_nopermeabilityvalues->value();
    if( value_sp == 0 ) return;

    int nopermeabilities = dg_permeability->getNumberofValues();
    if( nopermeabilities == 0 )
    {
        nopermeabilities = sp_nopermeabilityvalues->value();
        dg_permeability->setNumberofValues( nopermeabilities );
        dg_permeability->create();
    }

    dg_permeability->viewSingleValueForm();
}


void FlowParametersBar::on_btn_tfboundaryvalues_clicked()
{
    int value_sp = sp_notfboundaryvalues->value();
    if( value_sp == 0 ) return;

    int noboundaries = dg_tofboundary->getNumberofValues();
    if( noboundaries == 0 )
    {
        noboundaries = sp_notfboundaryvalues->value();
        dg_tofboundary->setNumberofValues( noboundaries );
        dg_tofboundary->create();
    }

    dg_tofboundary->viewTofandTracerForm();
}


void FlowParametersBar::on_btn_trboundaryvalues_clicked()
{
    int value_sp = sp_notrboundaryvalues->value();
    if( value_sp == 0 ) return;

    int noboundaries = dg_trboundary->getNumberofValues();
    if( noboundaries == 0 )
    {
        noboundaries = sp_notrboundaryvalues->value();
        dg_trboundary->setNumberofValues( noboundaries );
        dg_trboundary->create();
    }

    dg_trboundary->viewTofandTracerForm();
}


void FlowParametersBar::on_btb_acceptparameters_accepted()
{

    dg_permeability->sendValues();
    dg_boundaries->sendValues();
    dg_wells->sendValues();
    dg_tofboundary->sendValues();
    dg_trboundary->sendValues();

    closeBar();


}


void FlowParametersBar::on_btb_acceptparameters_rejected()
{
    closeBar();
}


void FlowParametersBar::on_btn_boundariesvalues_clicked()
{
    int value_sp = sp_noboundaries->value();
    if( value_sp == 0 ) return;

    int noboundaries = dg_boundaries->getNumberofValues();
    if( noboundaries == 0 )
    {
        noboundaries = value_sp;
        dg_boundaries->setNumberofValues( noboundaries );
        dg_boundaries->create();
    }

    dg_boundaries->viewBoundaryForm();

}


void FlowParametersBar::on_btn_wellsvalues_clicked()
{
    int value_sp = sp_nowells->value();
    if( value_sp == 0 ) return;

    int nowellsources = dg_wells->getNumberofValues();
    if( nowellsources == 0 )
    {
        nowellsources = value_sp;
        dg_wells->setNumberofValues( nowellsources );
        dg_wells->create();
    }

    dg_wells->viewWellForm();
}


void FlowParametersBar::on_btn_inputparameters_clicked()
{
    QString selected_format = "";
    QString filename = QFileDialog::getOpenFileName( this, tr( "Open File" ), "./inputs/",
                                                     ".ui files (*.ui)", &selected_format );
    if( filename.isEmpty() == true ) return;

    edt_inputparameters->setText( filename );
}


void FlowParametersBar::on_btn_loadparameterfile_clicked()
{
    if( edt_inputparameters->text().isEmpty() == true ) return;

    emit readParameterFile( edt_inputparameters->text().toStdString() );
}

void FlowParametersBar::clear()
{
    edt_inputparameters->clear();

    sp_noboundaries->setValue( 0 );
    sp_nopermeabilityvalues->setValue( 0 );
    sp_notfboundaryvalues->setValue( 0 );
    sp_notrboundaryvalues->setValue( 0 );
    sp_nowells->setValue( 0 );

    dg_permeability->reset();
    dg_boundaries->reset();
    dg_tofboundary->reset();
    dg_trboundary->reset();
    dg_wells->reset();


}
