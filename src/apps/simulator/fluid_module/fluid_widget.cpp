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
        /// Single Phase Method
        phase_method_.first = 1;
        phase_method_.second = 1;

        this->ui_ = new Ui::FluidWidgetForm;
        this->ui_->setupUi(this);

        this->setupWidget();
        this->createConnections();
		this->reset();

		ui_->radioButton_Singlephase_->setChecked(true);


    }

	void FluidWidget::reset()
	{
		for (std::size_t index = 0; index < single_double_input_GUI_fluid_.size(); index++)
		{
			single_double_input_GUI_fluid_[index].doubleSpinBox_->setValue(single_double_input_GUI_fluid_[index].default_value_);
			emit single_double_input_GUI_fluid_[index].doubleSpinBox_->editingFinished();
		}
	}

    void FluidWidget::clear()
    {		

    }

    void FluidWidget::setupWidget()
    {
        phase_methods_names_.resize(2);
        phase_methods_names_[0] = "By Water Saturation per Region";
        phase_methods_names_[1] = "By Oil Density";

        ui_->comboBox_PhaseMethods_->addItem(phase_methods_names_[0]);
        ui_->comboBox_PhaseMethods_->addItem(phase_methods_names_[1]);

		/// Grid positions at the double slider inputs
		int i = 0;
		int j = 0;
		/// Alternative Approach
		/// @FIXME January 2018 -- @TODO Create functors to update slider values:  std::function ?
		single_double_input_GUI_fluid_.resize(7);
		std::vector<QString> single_double_input_labels				= { "Oil Viscosity", "Oil Density", "Bo", "Water Viscosity", "Water Density", "Bw", "FWL" };
		std::vector<QString> single_double_input_units				= { " (cP)", " (kg/m3)", " (--)", " (cP)", " (kg/m3)", " (--)", " (m)" };
		std::vector<double>  single_double_input_low_values		    = { 0.01,  10.0,    1.0, 0.01,  10.0,   1.0, 0.0};
		std::vector<double>  single_double_input_high_values		= { 100.0, 2000.0,  5.0, 100.0, 2000.0, 5.0, 1000.0};
		std::vector<double>  single_double_input_default_values		= { 1.0,   1000.0,  1.0, 1.0,   1000.0, 1.0, 0.0};

		for (std::size_t index = 0; index < single_double_input_GUI_fluid_.size(); index++)
		{
			single_double_input_GUI_fluid_[index].label_ = new QLabel(tr(single_double_input_labels[index].toStdString().c_str()));
			single_double_input_GUI_fluid_[index].doubleSpinBox_ = new QDoubleSpinBox();
			single_double_input_GUI_fluid_[index].slider_ = new QSlider(Qt::Orientation::Horizontal);

			this->ui_->gridLayout_->addWidget(single_double_input_GUI_fluid_[index].label_, i, 0);
			this->ui_->gridLayout_->addWidget(single_double_input_GUI_fluid_[index].doubleSpinBox_, i, j + 1);
			this->ui_->gridLayout_->addWidget(single_double_input_GUI_fluid_[index].slider_, i, j + 2, 1, 8);

			single_double_input_GUI_fluid_[index].position_in_the_grid_ = std::make_tuple<int, int, int, int>(i + 0, j + 2, 1, 8);

			// Values in the GUI
			single_double_input_GUI_fluid_[index].range_low_value_ = single_double_input_low_values[index];
			single_double_input_GUI_fluid_[index].range_high_value_ = single_double_input_high_values[index];
			single_double_input_GUI_fluid_[index].range_size_ = single_double_input_high_values[index] - single_double_input_low_values[index];
			// Initial values in the GUI
			single_double_input_GUI_fluid_[index].default_value_ = single_double_input_default_values[index];

			single_double_input_GUI_fluid_[index].slider_->setMinimum(0);
			single_double_input_GUI_fluid_[index].slider_->setMaximum(102);
			single_double_input_GUI_fluid_[index].doubleSpinBox_->setSuffix(tr(single_double_input_units[index].toStdString().c_str()));
			single_double_input_GUI_fluid_[index].doubleSpinBox_->setMinimum(single_double_input_low_values[index]);
			single_double_input_GUI_fluid_[index].doubleSpinBox_->setMaximum(single_double_input_high_values[index]);

			bool is_enable = true;

			single_double_input_GUI_fluid_[index].slider_->setEnabled(is_enable);
			single_double_input_GUI_fluid_[index].label_->setEnabled(is_enable);
			single_double_input_GUI_fluid_[index].doubleSpinBox_->setEnabled(is_enable);

			i += 1;
		}
    }

    /// Create Qt Signal-Slot connection
	void FluidWidget::createConnections()
	{
		//@see http://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5

		for (std::size_t index = 0; index < single_double_input_GUI_fluid_.size(); index++)
		{
			connect(single_double_input_GUI_fluid_[index].slider_, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), this, [=]()
			{
				/// All slider have range into the range:  0-100
				double ex = static_cast<double>(0.01 * single_double_input_GUI_fluid_[index].slider_->value() * single_double_input_GUI_fluid_[index].range_size_);

				single_double_input_GUI_fluid_[index].doubleSpinBox_->setValue(ex);

				single_double_input_GUI_fluid_[index].value_ = ex;

			});

				connect(single_double_input_GUI_fluid_[index].doubleSpinBox_, &QDoubleSpinBox::editingFinished, this, [=]()
				{
					double ex = single_double_input_GUI_fluid_[index].doubleSpinBox_->value();

					int i = static_cast<int>(100.0 * (ex / single_double_input_GUI_fluid_[index].range_size_) );

					single_double_input_GUI_fluid_[index].slider_->blockSignals(true);
					single_double_input_GUI_fluid_[index].slider_->setValue(i);
					single_double_input_GUI_fluid_[index].slider_->blockSignals(false);

					single_double_input_GUI_fluid_[index].value_ = ex;
				});
		}


		/// @FIXME September
		connect(ui_->radioButton_Singlephase_, &QRadioButton::toggled, [=](bool status_)
		{
			ui_->comboBox_PhaseMethods_->setEnabled(false);

			emit setSinglePhase();

			phase_method_.first = 1;

		});

		connect(ui_->radioButton_Multiphase_, &QRadioButton::clicked, this, [=](bool status_)
		{
			ui_->comboBox_PhaseMethods_->setEnabled(true);

			emit ui_->comboBox_PhaseMethods_->currentIndexChanged(phase_methods_names_[ui_->comboBox_PhaseMethods_->currentIndex()]);

			emit setMultiPhase();

			phase_method_.first = 2;

		});

		connect(ui_->comboBox_PhaseMethods_, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
			[=](const QString &text){

			/// @FIXME September
			//"By Water Saturation per Region"
			if (text.compare(phase_methods_names_[0]) == 0)
			{
				//std::cout << "By Water Saturation per Region" << std::endl;
				emit setSaturationPerRegion();
				phase_method_.second = 1;
			}
			/// By API Grabity
			else
			{
				//std::cout << "By API Gravity" << std::endl;
				emit setAPIGravity();
				phase_method_.second = 2;
			}
		});


	}

    void FluidWidget::getFluidData(
		double& _oil_viscosity,
		double& _oil_density,
		double& _bo,
		double& _water_viscosity,
		double& _water_density,
		double& _bw,
		double& _fwl,
		std::pair<int, int>& _phase_method)
    {		        
		//{ "Oil Viscosity", "Oil Density", "Bo", "Water Viscosity", "Water Density", "Bw", "FwL" };

		_oil_viscosity		= single_double_input_GUI_fluid_[0].value_;
		_oil_density		= single_double_input_GUI_fluid_[1].value_;
		_bo					= single_double_input_GUI_fluid_[2].value_;
		_water_viscosity    = single_double_input_GUI_fluid_[3].value_;
		_water_density		= single_double_input_GUI_fluid_[4].value_;
		_bw					= single_double_input_GUI_fluid_[5].value_;
		_fwl				= single_double_input_GUI_fluid_[6].value_;

        _phase_method = this->phase_method_;
    }

    void FluidWidget::setFluidData()
    {
    }

}    /* namespace RRM */
