/*
 * FluidWidget.hpp
 *
 *  Created on: Apr 16, 2017
 *      Author: felipe
 */

#ifndef _FLUIDNWIDGET_HPP_
#define _FLUIDNWIDGET_HPP_

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QSlider>
#include <Eigen/Dense>

#include "ui_fluid_widget_form.h"

namespace RRM
{

    class FluidWidget: public QWidget
    {
            Q_OBJECT

        public:

            FluidWidget(QWidget * parent);
            virtual ~FluidWidget() = default;

			/// @FIXEME September
			/// @FIXEME January 2018 as in the spreadsheet:
			/// https://docs.google.com/spreadsheets/d/1z6rmsu_2ruVyM9qjEFfWeH6kiu33lBI-XltAuMoPd30/edit#gid=0

            void getFluidData (         		
				double& _oil_viscosity,
				double& _oil_density,
				double& _bo,
				double& _water_viscosity,
				double& _water_density,
				double& _bw,
				double& _fwl,
				std::pair<int, int>& _phase_method);

            /// Userless for now
            void setFluidData();

            void clear();
			void reset();

            public slots:

            signals:
                void setSaturationPerRegion();
                void setAPIGravity();
				void setSinglePhase();
				void setMultiPhase();

            private:

			struct singleDoubleInput_GUI_Fluid
			{
				QLabel*			label_;
				QDoubleSpinBox* doubleSpinBox_;
				QSlider*        slider_;			
			
				std::map< int, double > values_;
				double value_;
				// Values in the GUI
				double range_low_value_;
				double range_high_value_;
				double range_size_;
				// Initial values in the GUI
				double default_value_;

				std::tuple<int, int, int, int> position_in_the_grid_;
			};

			/// Alternative approach	
			std::vector<singleDoubleInput_GUI_Fluid> single_double_input_GUI_fluid_;
            // Designer form
            Ui::FluidWidgetForm * ui_;

            // Create  the default widget stage

            std::vector<QString> phase_methods_names_;

            void setupWidget();
            void createConnections();

            /// @FIXME September
            //// 1 for single phase
            //// 2 for multiphase
            ///        1 for water saturation per region
            ///        2 API gravity
            std::pair<int, int> phase_method_;
    };

} /* namespace RRM */

#endif /* _FLUIDNWIDGET_HPP_ */
