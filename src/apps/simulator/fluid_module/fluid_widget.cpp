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

    }

	void FluidWidget::reset()
	{
		for (std::size_t index = 0; index < single_double_input_GUI_fluid_.size(); index++)
		{
			single_double_input_GUI_fluid_[index].doubleSpinBox_->setValue(single_double_input_GUI_fluid_[index].default_value_);
			emit single_double_input_GUI_fluid_[index].doubleSpinBox_->editingFinished();
		}
	
		phase_method_.first = 1;
		phase_method_.second = 1;

		fluid_phase_type_ = FluidPhaseType::SinglePhase;
		multi_phase_type_ = MultiPhaseType::WaterSaturation;

		ui_->radioButton_Singlephase_->setChecked(true);
		ui_->comboBox_PhaseMethods_->setCurrentIndex(int(multi_phase_type_));
				
	}

	bool FluidWidget::read(const QJsonObject& fluid_data)
	{
		if (fluid_data.contains("fluid_phase_type_") && fluid_data["fluid_phase_type_"].isDouble())
		{
			fluid_phase_type_ = FluidPhaseType(int(fluid_data["fluid_phase_type_"].toDouble()));	
		}

		if (fluid_data.contains("multi_phase_type_") && fluid_data["multi_phase_type_"].isDouble())
		{
			multi_phase_type_ = MultiPhaseType(int(fluid_data["multi_phase_type_"].toDouble()));
		}
		
		QVector<QString> attributes({ "oil_viscosity", "oil_density", "bo", "water_viscosity", "water_density", "bw", "fwl" });

		for (std::size_t i = 0; i < attributes.size(); i++)
		{
			if (fluid_data.contains(attributes[i]) && fluid_data[attributes[i]].isDouble())
			{
				single_double_input_GUI_fluid_[i].value_ = fluid_data[attributes[i]].toDouble();
			}
			else
			{
				qWarning("No Data");
			}
		}

		if (fluid_phase_type_ == FluidPhaseType::SinglePhase)
		{
			ui_->radioButton_Singlephase_->setChecked(true);	
		}
		else
		{
			ui_->radioButton_Multiphase_->setChecked(true);
		}

		updateWidgetData();

		return true;
	}

	bool FluidWidget::write(QJsonObject& fluid_data)
	{
		fluid_data["fluid_phase_type_"] = fluid_phase_type_;
		fluid_data["multi_phase_type_"] = multi_phase_type_;

		fluid_data["oil_viscosity"]   = single_double_input_GUI_fluid_[0].value_;
		fluid_data["oil_density"]     = single_double_input_GUI_fluid_[1].value_;
		fluid_data["bo"]			  = single_double_input_GUI_fluid_[2].value_;
		fluid_data["water_viscosity"] = single_double_input_GUI_fluid_[3].value_;
		fluid_data["water_density"]   = single_double_input_GUI_fluid_[4].value_;
		fluid_data["bw"]			  = single_double_input_GUI_fluid_[5].value_;
		fluid_data["fwl"]			  = single_double_input_GUI_fluid_[6].value_;

		return true;
	}

    void FluidWidget::clear()
    {		

    }

	void FluidWidget::updateWidgetData()
	{
		for (std::size_t index = 0; index < single_double_input_GUI_fluid_.size(); index++)
		{
			std::cout << single_double_input_GUI_fluid_[index].value_ << std::endl;
			single_double_input_GUI_fluid_[index].doubleSpinBox_->setValue(single_double_input_GUI_fluid_[index].value_);		
			emit single_double_input_GUI_fluid_[index].doubleSpinBox_->editingFinished();
		}
		
	}
	

    void FluidWidget::setupWidget()
    {
        phase_methods_names_.resize(2);
        phase_methods_names_[0] = "By Water Saturation per Region";
        phase_methods_names_[1] = "By Density";

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

			fluid_phase_type_ = FluidPhaseType::SinglePhase;

			emit setSinglePhase();

			phase_method_.first = 1;

			/// { "Oil Viscosity", "Oil Density", "Bo", "Water Viscosity", "Water Density", "Bw", "FWL" };
			single_double_input_GUI_fluid_[1].label_->setEnabled(false);
			single_double_input_GUI_fluid_[1].doubleSpinBox_->setEnabled(false);
			single_double_input_GUI_fluid_[1].slider_->setEnabled(false);

			for (std::size_t index = 3; index < single_double_input_GUI_fluid_.size(); index++)
			{
				single_double_input_GUI_fluid_[index].label_->setEnabled(false);
				single_double_input_GUI_fluid_[index].doubleSpinBox_->setEnabled(false);
				single_double_input_GUI_fluid_[index].slider_->setEnabled(false);
			}

		});

		connect(ui_->radioButton_Multiphase_, &QRadioButton::toggled, this, [=](bool status_)
		{
			ui_->comboBox_PhaseMethods_->setEnabled(true);

			fluid_phase_type_ = FluidPhaseType::MultiPhase;					

			emit ui_->comboBox_PhaseMethods_->currentIndexChanged(int(multi_phase_type_));
		
			emit setMultiPhase();

			phase_method_.first = 2;

		});
		
		connect(ui_->comboBox_PhaseMethods_, QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int index)
		{
			multi_phase_type_ = MultiPhaseType(index);
			/// still dont know why I need to implicit set current index
			ui_->comboBox_PhaseMethods_->setCurrentIndex(index);
			
			/// @FIXME September
			//"By Water Saturation per Region"
			if (multi_phase_type_ == MultiPhaseType::WaterSaturation)
			{
				//std::cout << "By Water Saturation per Region" << std::endl;
				emit setSaturationPerRegion();		

				phase_method_.second = 1;

				/// { "Oil Viscosity", "Oil Density", "Bo", "Water Viscosity", "Water Density", "Bw", "FWL" };
				////                0,             1,    2,                 3,               4,    5,    6 
				std::vector<std::size_t> indices = { 1, 4, 6 };
				
				for (std::size_t index = 0; index < single_double_input_GUI_fluid_.size(); index++)
				{
					single_double_input_GUI_fluid_[index].label_->setEnabled(true);
					single_double_input_GUI_fluid_[index].doubleSpinBox_->setEnabled(true);
					single_double_input_GUI_fluid_[index].slider_->setEnabled(true);
				}

				for (auto index : indices)
				{
					single_double_input_GUI_fluid_[index].label_->setEnabled(false);
					single_double_input_GUI_fluid_[index].doubleSpinBox_->setEnabled(false);
					single_double_input_GUI_fluid_[index].slider_->setEnabled(false);
				}


			}
			/// By API Grabity
			else
			{
				//std::cout << "By Density" << std::endl;
				emit setAPIGravity();							

				phase_method_.second = 2;

				/// { "Oil Viscosity", "Oil Density", "Bo", "Water Viscosity", "Water Density", "Bw", "FWL" };
				////                0,             1,    2,                 3,               4,    5,    6 

				for (std::size_t index = 0; index < single_double_input_GUI_fluid_.size(); index++)
				{
					single_double_input_GUI_fluid_[index].label_->setEnabled(true);
					single_double_input_GUI_fluid_[index].doubleSpinBox_->setEnabled(true);
					single_double_input_GUI_fluid_[index].slider_->setEnabled(true);
				}

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
