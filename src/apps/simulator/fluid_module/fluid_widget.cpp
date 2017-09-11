/*
 * WellWidget.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: felipe
 */

#include "fluid_widget.hpp"

#include <iostream>

namespace RRM
{
    FluidWidget::FluidWidget(QWidget * _parent) :
            QWidget(_parent)
    {
        this->ui_ = new Ui::FluidWidgetForm;
        this->ui_->setupUi(this);

        this->setupWidget();
        this->createConnections();

		viscosity_values_.resize(1);

		viscosity_values_[0] = ui_->doubleSpinBox_Region_Viscosity_->value();
    }

    void FluidWidget::clear()
    {
    }

    void FluidWidget::setupWidget()
    {

    }


    /// Create Qt Signal-Slot connection
    void FluidWidget::createConnections()
    {
        //@see http://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5

        /// New GUI ---->
        //// REGION

        //@see http://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5

        connect(ui_->horizontalSlider_Viscosity_,static_cast<void (QSlider::*)(int)>(&QSlider::sliderMoved), this, [=]()
        {
            double ex = 0.1 + (0.1)*static_cast<double>(ui_->horizontalSlider_Viscosity_->value());
            ui_->doubleSpinBox_Region_Viscosity_->setValue(ex);
            /// FIXME Viscosity have to be the same for all model for now, util a better
            /// interface if design.
			viscosity_values_[0] = ex;			
        });


		connect(ui_->doubleSpinBox_Region_Viscosity_, &QDoubleSpinBox::editingFinished, this, [=]()
		{
			double ex = ui_->doubleSpinBox_Region_Viscosity_->value();

			int i = static_cast<int>((ex - 0.1) / 0.1);
			/// FIXME Viscosity have to be the same for all model for now, util a better
			/// interface if design.

			ui_->horizontalSlider_Viscosity_->setValue(i);

			viscosity_values_[0] = ex;
		});

    }

    void FluidWidget::getFluidData(std::vector<double>& _viscosity_values)
    {
		_viscosity_values.clear();
		_viscosity_values = this->viscosity_values_;
    }

    void FluidWidget::setFluidData(const std::vector<double>& _viscosity_values)
    {
    }



}    /* namespace RRM */
