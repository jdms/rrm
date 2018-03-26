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
#include <QtWidgets/QCheckBox>
#include <QPixmap>
#include <Eigen/Dense>


#include "ui_region_widget_form.h"

#include "../widgets/spanslider/qxtspanslider.h"
#include "../core/widgets/color_picker.h"

namespace RRM
{

        class RegionWidget : public QWidget
        {
                 Q_OBJECT

                public:

                         RegionWidget ( QWidget * parent );
                        virtual ~RegionWidget ( ) = default;

						void getRegionData(int& _number_of_regions,
							std::map<int, std::pair<double, double> >& _x_permeability_values,
							std::map<int, std::pair<double, double> >& _y_permeability_values,
							std::map<int, std::pair<double, double> >& _z_permeability_values,
							std::map<int, std::pair<double, double> >& _porosity_values_,
							std::vector<double>& _saturations_values, /// Water Saturation
							std::vector<double>& _pct_values,		  /// Threshold Pressure
							std::vector<double>& _sort_factor_values, /// Lambda
							std::vector<double>& _siw_values		  /// Connate Water Saturation 
							);

                        void setRegionData( const std::map< int,  std::vector< int > >& region_colors );
                        void updateRegionColor( int _index, int red, int green, int blue );

                        int getNumberOfRegion() const;

						void setMultiPhaseByRegionSaturation();
						void setSinglePhase();
						void setMultiPhaseByDensity();
    
						int reversePermeability(bool _is_reversed,int _value);
						int resersePorosity(bool _is_reversed, int _value) ;

                public slots:
                       
						void clear();
						void reset();

                signals:
                        void numberOfRegions(int _number_of_regions);
                        void getRegions();
                private:

						struct Permeability_GUI
						{
							QxtSpanSlider* qxt_span_slider_;
							QSlider*     slider_;
							QLabel* gradient_Label_;
							QDoubleSpinBox* doubleSpinbBox_low_;
							QLabel* label_top_;
							QDoubleSpinBox* doubleSpinbBox_high_;
							QLabel* label_bottom_;
							std::tuple<int, int, int, int> position_in_the_grid_;
							/// FIXME January 2018
							QPushButton*      inverted_;
							// Values in the GUI
							double range_low_value_;
							double range_high_value_;
							// Initial values in the GUI
							double default_low_value_;
							double default_high_value_;
							/// Data
							std::map< int, std::pair<double, double> > gradient_values_;
							std::map< int, bool> is_inverted_;							
						};

						struct singleDoubleInput_GUI
						{
							QLabel*			label_;
							QDoubleSpinBox* doubleSpinBox_;
							QSlider*        slider_;
							std::map< int, double > values_;
							// Values in the GUI
							double range_low_value_;
							double range_high_value_;
							double range_size_;
							// Initial values in the GUI
							double default_value_;

							std::tuple<int, int, int, int> position_in_the_grid_;
						};

                        // Designer form
                        Ui::RegionWidgetForm * ui_;

                        // Create  the default widget stage
                        void setupWidget();
                        void createConnections();
                        void updateRegionsWidget(int _index);
                        void createRegions( const std::map< int,  std::vector< int > >& region_colors  );

                        int number_of_regions_;             

						///@FIXME January 2018 --------------------------------
						///----------------------------------------------------
						/// Alternative approach	
						std::vector<Permeability_GUI> permeability_GUI_;
						///----------------------------------------------------

                        QxtSpanSlider* qxt_span_slider_porosity_;
                        QSlider*     slider_porosity_;
							QLabel* gradient_porosity_Label_;
                        QDoubleSpinBox* doubleSpinbBox_low_porosity_;
							QLabel*	label_bottom_porosity_;
                        QDoubleSpinBox* doubleSpinbBox_high_porosity_;
							QLabel*	label_top_porosity_;
						// Values in the GUI
						double range_low_porosity_;
						double range_high_porosity_;
						// Initial values in the GUI
						double default_low_porosity_;
						double default_high_porosity_;

                        std::map< int, std::pair<double, double> > porosity_gradient_values_;
						std::map< int, bool> is_inverted_porosity_gradient_values_;
                        std::tuple<int, int, int, int> porosity_position_;

						bool is_reversed_porosity_;
						QPushButton*      inverted_porosity_;
		
						///@FIXME January 2018 --------------------------------
						///----------------------------------------------------
						/// Alternative approach	
						std::vector<singleDoubleInput_GUI> single_double_input_GUI_;
						///----------------------------------------------------

                        QLabel* lb_region_color;
                        std::map< int, QColor > regions_colors;

						std::map< int, std::vector< int > > region_colors_;

						/// @FIXEME Janudary 2018
						bool is_isotropic_permeability_;
						bool is_constant_;

        };

} /* namespace RRM */

#endif /* _REGIONWIDGET_HPP_ */
