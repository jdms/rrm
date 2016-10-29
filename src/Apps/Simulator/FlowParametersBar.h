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

        void setPropertyAreaParameters( const int np, const std::vector< double >& positions , const std::vector< double >& perm, const std::vector< double >& poros, const std::vector< double >& visc );
        void getPropertyAreaParameters( int& np, std::vector< double >& values , std::vector< double >& perm, std::vector< double >& poros, std::vector< double >& visc );

        void setWellParameter( const int nw,  const std::vector< unsigned int >& type, const std::vector< double >& value, const std::vector< int >& sign );
        void getWellParameter( int& nw,  std::vector< unsigned int >& type, std::vector< double >& value, std::vector< int >& sign );

        void clear();


    private slots:

        void on_btn_inputparameters_clicked();
        void on_btn_loadparameterfile_clicked();

        void on_btn_regionproperties_clicked();
        void on_btn_wellsvalues_clicked();

        void on_btb_acceptparameters_accepted();
        void on_btb_acceptparameters_rejected();


    signals:



        void readParameterFile( const std::string& input_file );
        void closeBar();
        void closeBarandAccept();


    private:

        FormPropertyValues* dg_regionsproperties;
        FormPropertyValues* dg_wells;

};

#endif // FLOWPARAMETERSBAR_H
