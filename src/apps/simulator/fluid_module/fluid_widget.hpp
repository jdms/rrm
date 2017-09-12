/*
 * FluidWidget.hpp
 *
 *  Created on: Apr 16, 2017
 *      Author: felipe
 */

#ifndef _FLUIDNWIDGET_HPP_
#define _FLUIDNWIDGET_HPP_

#include <QtWidgets/QWidget>
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

            void getFluidData ( std::vector<double>& _viscosity_values,
                                std::vector< double >& _bo_values,
                                std::vector< double >& _oildensity_values,
                                std::vector<std::pair<int, int>>& _phase_method);
            /// Userless for now
            void setFluidData(const std::vector<double>& _viscosity_values, const std::vector<double>& _bo_values);

            void clear();

            public slots:

            signals:

                void setSaturationPerRegion();
                void setAPIGravity();
                void setSinglePhase();

            private:
            // Designer form
            Ui::FluidWidgetForm * ui_;

            // Create  the default widget stage

            std::vector<QString> phase_methods_names_;

            void setupWidget();
            void createConnections();

            std::vector<double> viscosity_values_;
            std::vector<double> bo_values_;
            std::vector<double> oildensity_values_;

            /// @FIXME September
            //// 1 for single phase
            //// 2 for multiphase
            ///        1 for water saturation per region
            ///        2 API gravity
            std::vector<std::pair<int, int>> phase_method_;
    };

} /* namespace RRM */

#endif /* _FLUIDNWIDGET_HPP_ */
