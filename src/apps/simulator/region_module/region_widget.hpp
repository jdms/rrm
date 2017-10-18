/*
 * WellWidget.hpp
 *
 *  Created on: Apr 16, 2017
 *      Author: felipe
 */

#ifndef _REGIONWIDGET_HPP_
#define _REGIONWIDGET_HPP_

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <Eigen/Dense>


#include "ui_region_widget_form.h"

#include "../widgets/spanslider/qxtspanslider.h"

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
                                           std::vector<double >& _staturation_values,
                                           std::map<int, std::pair<double,double> >& _porosity_gratdients,
                                           std::map<int, std::pair<double, double> >& _permeability_gratdients
                                           );

                        void setRegionData(const int _number_of_regions,
                            const std::vector<double>& _positions,
                            const std::vector<double>& _permeability_values,
                            const std::vector<double>& _porosity_values,
                            const std::vector<double >& _staturation_values
                            );

                        int getNumberOfRegion() const;

                        void setRegionDepth(float _depth);

						void setByRegionSaturation(bool option);
    
                        void clear();

                public slots:
                        void updateRegionPosition(const std::map< int, Eigen::Vector3f >& _positions);
                signals:
                        void numberOfRegions(int _number_of_regions);
                        void getRegions();
                private:
                        // Designer form
                        Ui::RegionWidgetForm * ui_;

                        // Create  the default widget stage
                        void setupWidget();
                        void createConnections();
                        void updateRegionsWidget(int _index);
                        void createRegions( int _number_of_regions);

                        int number_of_regions_;

                        /// @Deprecated soon ...
                        float depth_;

                        std::map< int, Eigen::Vector3f > positions_values;

                        /// Region Attributes
                        std::map< int, double > permeability_values;
                        std::map< int, double > porosity_values;
                        std::map< int, double > saturation_values;

                        
                        ///@FIXME September 2017
                        QxtSpanSlider* qxt_span_slider_permeability_;
                        QSlider*     slider_permeability_;
                        QLabel* gradient_permeability_Label_;
                        QDoubleSpinBox* doubleSpinbBox_low_permeability_;
                        QDoubleSpinBox* doubleSpinbBox_high_permeability_;

                        std::map< int, std::pair<double,double> > permeability_gradient_values_;
                        std::tuple<int, int, int, int> permeability_position_;

                        QxtSpanSlider* qxt_span_slider_porosity_;
                        QSlider*     slider_porosity_;
                        QLabel* gradient_porosity_Label_;
                        QDoubleSpinBox* doubleSpinbBox_low_porosity_;
                        QDoubleSpinBox* doubleSpinbBox_high_porosity_;

                        std::map< int, std::pair<double, double> > porosity_gradient_values_;
                        std::tuple<int, int, int, int> porosity_position_;
		
						// Water Saturation
						QSlider*        slider_Water_Saturation_;
						QLabel*			label_Water_Saturation_;
						QDoubleSpinBox* doubleSpinBox_Region_Water_Saturation_;

						std::tuple<int, int, int, int> water_saturation_position_;
        };

} /* namespace RRM */

#endif /* _REGIONWIDGET_HPP_ */
