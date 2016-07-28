#ifndef FLOWPARAMETERSBAR_H
#define FLOWPARAMETERSBAR_H

#include <iostream>

#include <QWidget>
#include <QFileDialog>

#include "ui_flowparametersbar.h"
#include "FormPropertyValues.h"


class FlowParametersBar : public QWidget,
                          private Ui::FlowParametersBar
{
    Q_OBJECT

    public:
        explicit FlowParametersBar( QWidget *parent = 0 );

        void createDialogs();

       void setPropertyAreaParameters( int np, std::vector< double > values );
        void setSurfaceBoundariesParameter(int nbound, std::vector< double > vbound );
        void setWellParameter( int nw, std::vector< double > vwell );
        void setTofBoundaryParameter( int ntfbound, std::vector< int > vtfbound );
        void setTracerBoundaryParameter(int ntrbound, std::vector< int > vtrbound );

        void clear();


    private slots:

        void on_btn_permeabilityvalues_clicked();
        void on_btn_tfboundaryvalues_clicked();
        void on_btn_trboundaryvalues_clicked();
        void on_btb_acceptparameters_accepted();
        void on_btb_acceptparameters_rejected();
        void on_btn_boundariesvalues_clicked();
        void on_btn_wellsvalues_clicked();
        void on_btn_inputparameters_clicked();
        void on_btn_loadparameterfile_clicked();

signals:


        void sendPropertyArea( const int& n, const std::vector< double >& );
        void sendBoundariesValues( int n, std::vector< double >& );
        void sendWellsValues( int n, std::vector< double >& );
        void sendTOFBoundaryValues( int n, std::vector< double >& );
        void sendTrBoundaryValues( int n, std::vector< double >& );

        void readParameterFile( const std::string& input_file );
        void closeBar();
        void closeBarandAccept();

private:

    FormPropertyValues* dg_permeability;
    FormPropertyValues* dg_boundaries;
    FormPropertyValues* dg_wells;
    FormPropertyValues* dg_tofboundary;
    FormPropertyValues* dg_trboundary;

};

#endif // FLOWPARAMETERSBAR_H
