/*
 * WellWidget.hpp
 *
 *  Created on: Apr 16, 2017
 *      Author: felipe
 */

#ifndef _REGIONWIDGET_HPP_
#define _REGIONWIDGET_HPP_

#include <QtWidgets/QWidget>
#include <Eigen/Dense>


#include "ui_RegionWidgetForm.h"

namespace RRM
{

        class RegionWidget : public QWidget
        {
                 Q_OBJECT

                public:

                         RegionWidget ( QWidget * parent );
                        virtual ~RegionWidget ( ) = default;

						void getRegionData(int& _number_of_regions, 
										   std::vector<double>& _positions,
										   std::vector<double>& _permeability_values,
										   std::vector<double>& _porosity_values,
										   std::vector<double>& _viscosity_values
										   );

						void setRegionData(int& _number_of_regions,
							std::vector<double>& _positions,
							std::vector<double>& _permeability_values,
							std::vector<double>& _porosity_values,
							std::vector<double>& _viscosity_values
							);

						int getNumberOfRegion() const;

						void setRegionDepth(float _depth);

						void clear();

                public slots:
                        void updateRegionPosition(const std::map< int, Eigen::Vector3f >& _positions);
                signals:
                        void numberOfRegions(int _number_of_regions);
                private:
                        // Designer form
                        Ui::RegionWidgetForm * ui_;

                        // Create  the default widget stage
                        void setupWidget();
                        void createConnections();
                        void updateRegionsWidget(int _index);
                        void createRegions( int _number_of_regions);

                        int number_of_regions_;
						float depth_;
                        std::map< int, Eigen::Vector3f > positions_values;
                        std::map< int, double > permeability_values;
                        std::map< int, double > porosity_values;
                        std::map< int, double > viscosity_values;

                        std::map< int, int > permeability_slider_values;
                        std::map< int, int > porosity_slider_values;
                        std::map< int, int > viscosity_slider_values;
        };

} /* namespace RRM */

#endif /* _REGIONWIDGET_HPP_ */
