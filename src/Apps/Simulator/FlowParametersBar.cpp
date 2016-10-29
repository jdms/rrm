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
    btn_regionproperties->setIcon(QIcon(":/images/icons/pencil--plus.png"));
    btn_wellsvalues->setIcon(QIcon(":/images/icons/pencil--plus.png"));


    dg_regionsproperties = new FormPropertyValues( FormPropertyValues::PROPERTY_TYPE::REGIONS, this );
    dg_wells = new FormPropertyValues( FormPropertyValues::PROPERTY_TYPE::WELL, this );


    connect( sp_noregions, SIGNAL( valueChanged(int) ), dg_regionsproperties, SLOT( reset() ) );
    connect( sp_nowells, SIGNAL( valueChanged(int) ), dg_wells, SLOT( reset() ) );

}



void FlowParametersBar::setPropertyAreaParameters( const int np, const std::vector< double >& positions , const std::vector< double >& perm, const std::vector< double >& poros, const std::vector< double >& visc )
{
    sp_noregions->setValue( np );
    dg_regionsproperties->createRegionPropertiesForm( np, positions, perm, poros, visc );
}


void FlowParametersBar::getPropertyAreaParameters( int& np, std::vector< double >& values , std::vector< double >& perm, std::vector< double >& poros, std::vector< double >& visc )
{
    np = dg_regionsproperties->getNumberofRegions();
    values = dg_regionsproperties->getRegionPositions();
    perm = dg_regionsproperties->getRegionPermeability();
    poros = dg_regionsproperties->getRegionPorosity();
    visc = dg_regionsproperties->getRegionViscosity();
}





void FlowParametersBar::setWellParameter( const int nw,  const std::vector< unsigned int >& type, const std::vector< double >& value, const std::vector< int >& sign )
{
    sp_nowells->setValue( nw );
    dg_wells->createWellForm( nw,  type, value, sign );
}


void FlowParametersBar::getWellParameter( int& nw,  std::vector< unsigned int >& type, std::vector< double >& value, std::vector< int >& sign )
{
    nw = dg_wells->getNumberofWells();
    type = dg_wells->getWellTypes();
    value = dg_wells->getWellValues();
    sign = dg_wells->getWellSigns();
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





void FlowParametersBar::on_btn_regionproperties_clicked()
{
    int value_sp = sp_noregions->value();
    if( value_sp == 0 ) return;

    int noregions = dg_regionsproperties->getNumberofRegions();
    if( noregions == 0 )
    {
        dg_regionsproperties->createRegionPropertiesForm( value_sp );
    }

    dg_regionsproperties->viewRegionPropertiesForm();
}



void FlowParametersBar::on_btn_wellsvalues_clicked()
{
    int value_sp = sp_nowells->value();
    if( value_sp == 0 ) return;

    int nowellsources = dg_wells->getNumberofWells();
    if( nowellsources == 0 )
    {
        dg_wells->createWellForm( value_sp );
    }

    dg_wells->viewWellForm();
}





void FlowParametersBar::on_btb_acceptparameters_accepted()
{

    closeBarandAccept();

}


void FlowParametersBar::on_btb_acceptparameters_rejected()
{
    closeBar();
}






void FlowParametersBar::clear()
{
    edt_inputparameters->clear();

    sp_noregions->setValue( 0 );
    sp_nowells->setValue( 0 );

    dg_regionsproperties->reset();
    dg_wells->reset();


}
