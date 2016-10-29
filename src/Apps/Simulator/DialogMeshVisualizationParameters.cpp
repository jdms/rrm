#include "DialogMeshVisualizationParameters.h"
#include "ui_DialogMeshVisualizationParameters.h"

DialogMeshVisualizationParameters::DialogMeshVisualizationParameters(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setLayout( vl_trianglelayout );
}

DialogMeshVisualizationParameters::~DialogMeshVisualizationParameters()
{
}

void DialogMeshVisualizationParameters::on_buttonBox_accepted()
{

    std::string cmd = edt_cmdtriangle->text().toStdString();

    float value = edt_value->text().toFloat();

    std::string method;

    if( rd_numberpartition->isChecked() )
        method = "PARTITION";
    else
        method = "EDGELENGHT";

    emit sendVisualizationParameters( cmd, method, value );

    closeBar();
}


void DialogMeshVisualizationParameters::setVisualizationParameters( const std::string& trianglecmd, const std::string& resolutiontype, const int& npartitionedge, const int&lenghtedge )
{
    edt_cmdtriangle->setText( trianglecmd.c_str() );

    if( resolutiontype.compare( "PARTITION" ) == 0 ){
        edt_value->setText( QString( "%1" ).arg( npartitionedge ) );
        rd_numberpartition->setChecked( true );
    }
    else if( resolutiontype.compare( "EDGELENGHT" ) == 0 ){
        edt_value->setText( QString( "%1" ).arg( lenghtedge ) );
        rd_lenghtedge->setChecked( true );
    }

}

void DialogMeshVisualizationParameters::on_buttonBox_rejected()
{
    closeBar();
}
