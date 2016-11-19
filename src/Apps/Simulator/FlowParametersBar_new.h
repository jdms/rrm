#ifndef FLOWPARAMETERSBAR_H
#define FLOWPARAMETERSBAR_H

#include <iostream>

#include <QWidget>
#include <QFileDialog>
#include <QMouseEvent>

#include "ui_flowparametersbar.h"
#include "FormPropertyValues.h"



#include "ui_FormPropertyValues.h"

#include <Eigen/Dense>

class FlowParametersBar_new : public QWidget, private Ui::FormPropertyValues
{
    Q_OBJECT

    public:

		explicit FlowParametersBar_new (QWidget *parent = 0);

        void createDialogs();

        void setPropertyAreaParameters( const int np, const std::vector< double >& positions , const std::vector< double >& perm, const std::vector< double >& poros, const std::vector< double >& visc );
        void getPropertyAreaParameters( int& np, std::vector< double >& values , std::vector< double >& perm, std::vector< double >& poros, std::vector< double >& visc );

        void setWellParameter( const int nw,  const std::vector< unsigned int >& type, const std::vector< double >& value, const std::vector< int >& sign );
        void getWellParameter( int& nw,  std::vector< unsigned int >& type, std::vector< double >& value, std::vector< int >& sign );

        void clear();

		void mousePressEvent(QMouseEvent *event);
	public slots:
		void set_region_point(double x, double y, double z);
    private slots:

        void on_btn_inputparameters_clicked();
        void on_btn_loadparameterfile_clicked();

        void on_btn_regionproperties_clicked();
        void on_btn_wellsvalues_clicked();

        void on_btb_acceptparameters_accepted();
        void on_btb_acceptparameters_rejected();

		/// Regions
		void updateRegionWidget(const int index);
		void createRegions();

		/// Wells
		void updateWellsWidget(const int index);
		void createWells();

    signals:



        void readParameterFile( const std::string& input_file );
        void closeBar();
        void closeBarandAccept();

	

    private:

		FormPropertyValues* dg_regionsproperties;
		FormPropertyValues* dg_wells;



		/// New Gui -->
		void getPropertyAreaParameters_new(int& np, std::vector< double >& positions, std::vector< double >& perm, std::vector< double >& poros, std::vector< double >& visc);
		void loadRegions (const int np, const std::vector< double >& positions, const std::vector< double >& perm, const std::vector< double >& poros, const std::vector< double >& visc);
		void getWellParameter_new(int& nw, std::vector< unsigned int >& type, std::vector< double >& value, std::vector< int >& sign);
		void loadWells(const int nw, const std::vector< unsigned int >& type, const std::vector< double >& value, const std::vector< int >& sign);

		std::map< int, Eigen::Vector3d > positions_values;
		std::map< int, double > perm_values;
		std::map< int, double > poros_values;
		std::map< int, double > visc_values;

		std::map< int, unsigned int > well_types;
		std::map< int, double >		  well_pressure;
		std::map< int, int >		  well_signs;
		std::size_t					  number_of_wells_;

		void new_gui_clear();

		/// Using new flowlib model, those variable whould be:  
		/// "Property Name 1" - 0 dimension scalar , property["permiability"] = 1.0;
		/// "Property Name 2" - 0 dimension scalar , property["porosity"    ] = 1.0;
		/// "Property Name 3" - 0 dimension scalar , property["viscosity"   ] = 1.0;
		/// "Property Name 4" - 1 dimension vector , property["psoition"    ] = {1.0, 1.0, 1.0};

		std::vector<double> permiability_; // size = n
		std::vector<double> porosity_;     // size = n
		std::vector<double> viscosity_;    // size = n
		std::vector<double> positions_;    // size = 3*n
		std::size_t			number_of_regions_;
};

#endif // FLOWPARAMETERSBAR_H
