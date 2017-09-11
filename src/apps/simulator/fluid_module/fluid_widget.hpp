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

            void getFluidData(std::vector<double>& _viscosity_values);

            void setFluidData(const std::vector<double>& _viscosity_values);

            void clear();

            public slots:

            signals:

            private:
            // Designer form
            Ui::FluidWidgetForm * ui_;

            // Create  the default widget stage
            void setupWidget();
            void createConnections();

            std::vector<double> viscosity_values_;
    };

} /* namespace RRM */

#endif /* _FLUIDNWIDGET_HPP_ */
