#ifndef FLOWPARAMETERSBAR_H
#define FLOWPARAMETERSBAR_H

#include <iostream>

#include <QWidget>

#include "ui_FlowParametersBar.h"
#include "FormPropertyValues.h"


class FlowParametersBar : public QWidget,
                          private Ui::FlowParametersBar
{
    Q_OBJECT

    public:
        explicit FlowParametersBar( QWidget *parent = 0 );

        void createDialogs();

        void setTetgenCommand( std::string cmd );
        void setPropertyAreaParameters( int np, std::vector< double > values );


        void setSurfaceBoundariesParameter(int nbound, std::vector< double > vbound );
        void setWellParameter( int nw, std::vector< double > vwell );
        void setTofBoundaryParameter( int ntfbound, std::vector< int > vtfbound );
        void setTracerBoundaryParameter(int ntrbound, std::vector< int > vtrbound );

    private slots:

        void on_btn_permeabilityvalues_clicked();
        void on_btn_tfboundaryvalues_clicked();
        void on_btn_trboundaryvalues_clicked();
        void on_btb_acceptparameters_accepted();

        void on_btb_acceptparameters_rejected();


        void on_btn_boundariesvalues_clicked();

        void on_btn_wellsvalues_clicked();

signals:

        void sendToleranceValues( const float& , const float& );
        void sendTetgenCommand( std::string& );
        void sendPropertyArea( const int& n, const std::vector< double >& );
        void sendBoundariesValues( int n, std::vector< double >& );
        void sendWellsValues( int n, std::vector< double >& );


        void sendTOFBoundaryValues( int n, std::vector< double >& );
        void sendTrBoundaryValues( int n, std::vector< double >& );

        void readSurface();
        void rebuildTetrahedricalVolume();

        void closeBar();


private:

    FormPropertyValues* dg_permeability;
    FormPropertyValues* dg_boundaries;
    FormPropertyValues* dg_wells;
    FormPropertyValues* dg_tofboundary;
    FormPropertyValues* dg_trboundary;

};

#endif // FLOWPARAMETERSBAR_H
