#ifndef FLOWPARAMETERSBAR_H
#define FLOWPARAMETERSBAR_H

#include <iostream>

#include <QWidget>
#include <QFileDialog>
#include <QMouseEvent>

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

		int getNumberOfRegions();

        void clear();
		
	public slots:
		void setRegionPoints(const std::map<int,Eigen::Vector3f>& region_points);
		void setRegionDepth(const float _depth);
    private slots:

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
		void numberRegions(int number_of_regions);

    private:

		/// New Gui -->
		void getPropertyAreaParameters_new(int& np, std::vector< double >& positions, std::vector< double >& perm, std::vector< double >& poros, std::vector< double >& visc);
		void loadRegions (const int np, const std::vector< double >& positions, const std::vector< double >& perm, const std::vector< double >& poros, const std::vector< double >& visc);
		void getWellParameter_new(int& nw, std::vector< unsigned int >& type, std::vector< double >& value, std::vector< int >& sign);
		void loadWells(const int nw, const std::vector< unsigned int >& type, const std::vector< double >& value, const std::vector< int >& sign);

		std::map< int, Eigen::Vector3d > positions_values;
		std::map< int, double > permeability_values;
		std::map< int, double > porosity_values;
		std::map< int, double > viscosity_values;

		std::map< int, int > permeability_slider_values;
		std::map< int, int > porosity_slider_values;
		std::map< int, int > viscosity_slider_values;

		std::map< int, unsigned int > well_types;
		std::map< int, double >		  well_pressure;
		std::map< int, int >		  well_signs;
		std::size_t					  number_of_wells_;

		void newGUIClear();
		void newGUIReset();

		/// Using new flowlib model, those variable whould be:  
		/// "Property Name 1" - 0 dimension scalar , property["permiability"] = 1.0;
		/// "Property Name 2" - 0 dimension scalar , property["porosity"    ] = 1.0;
		/// "Property Name 3" - 0 dimension scalar , property["viscosity"   ] = 1.0;
		/// "Property Name 4" - 1 dimension vector , property["psoition"    ] = {1.0, 1.0, 1.0};

		std::vector<double> permiability_; // size = n
		std::vector<double> porosity_;     // size = n
		std::vector<double> viscosity_;    // size = n
		std::vector<double> positions_;    // size = 3*n
		int					number_of_regions_;

		/// Depth of region in 3D. This value is calculated in the Mesh.cpp int the function buildBoundingBox()
		float depth_;
};

#endif // FLOWPARAMETERSBAR_H
