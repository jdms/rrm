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
                                           std::vector<double>& _positions,
                                           std::vector<double>& _permeability_values,
                                           std::vector<double>& _porosity_values,
                                           std::vector<double >& _staturation_values,
                                           std::map<int, std::pair<double,double> >& _porosity_gratdients,
                                           std::map<int, std::pair<double, double> >& _permeability_gratdients
                                           );

                        void setRegionData( const std::map< int,  std::vector< int > >& region_colors );
                        void updateRegionColor( int _index, int red, int green, int blue );

                        int getNumberOfRegion() const;

						void setByRegionSaturation(bool option);
    
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
							/// FIXME January 2018
							bool is_reversed_;
							QPushButton*      inverted_;
							// Values in the GUI
							double low_value_;
							double high_value_;
							// Initial values in the GUI
							double default_low_value_;
							double default_high_value_;

							std::map< int, std::pair<double, double> > gradient_values_;
							std::map< int, bool> is_inverted_;
							std::tuple<int, int, int, int> position_;
						};

                        // Designer form
                        Ui::RegionWidgetForm * ui_;

                        // Create  the default widget stage
                        void setupWidget();
                        void createConnections();
                        void updateRegionsWidget(int _index);
                        void createRegions( const std::map< int,  std::vector< int > >& region_colors  );

                        int number_of_regions_;             
                        
                        ///@FIXME September 2017
                        QxtSpanSlider* qxt_span_slider_permeability_;
                        QSlider*     slider_permeability_;
							QLabel* gradient_permeability_Label_;
                        QDoubleSpinBox* doubleSpinbBox_low_permeability_;
							QLabel* label_top_pearmeability_;
                        QDoubleSpinBox* doubleSpinbBox_high_permeability_;
							QLabel* label_bottom_pearmeability_;
							/// FIXME January 2018
							bool is_reversed_permeability_;
							QPushButton*      inverted_permeability_;
						// Values in the GUI
						double low_permeability_;
						double high_permeability_;
						// Initial values in the GUI
						double default_low_permeability_;
						double default_high_permeability_;

                        std::map< int, std::pair<double,double> > permeability_gradient_values_;
						std::map< int, bool> is_inverted_permeability_gradient_values_;
                        std::tuple<int, int, int, int> permeability_position_;

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
						double low_porosity_;
						double high_porosity_;
						// Initial values in the GUI
						double default_low_porosity_;
						double defaul_high_porosity_;

                        std::map< int, std::pair<double, double> > porosity_gradient_values_;
						std::map< int, bool> is_inverted_porosity_gradient_values_;
                        std::tuple<int, int, int, int> porosity_position_;
		
						// Water Saturation
						QSlider*        slider_Water_Saturation_;
							QLabel*			label_Water_Saturation_;
						QDoubleSpinBox* doubleSpinBox_Region_Water_Saturation_;
						std::map< int, double > saturation_values;
						// Values in the GUI
						double low_water_saturation_;
						double high_water_saturation_;
						// Initial values in the GUI
						double defaul_low_water_saturation_;
						double defaul_high_water_saturation_;						

						std::tuple<int, int, int, int> water_saturation_position_;

                        QLabel* lb_region_color;
                        std::map< int, QColor > regions_colors;


						bool is_reversed_porosity_;
						QPushButton*      inverted_porosity_;

						std::map< int, std::vector< int > > region_colors_;

        };

} /* namespace RRM */

#endif /* _REGIONWIDGET_HPP_ */
