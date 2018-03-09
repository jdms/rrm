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

        viscosity_values_.resize(1);
        bo_values_.resize(1);
        oildensity_values_.resize(1);

        /// Single Phase Method
        phase_method_.first = 1;
        phase_method_.second = 1;

        this->ui_ = new Ui::FluidWidgetForm;
        this->ui_->setupUi(this);

        this->setupWidget();
        this->createConnections();
  //      ui_->radioButton_Singlephase_->setChecked(true);

		this->default_viscosity_value_  = 1.0;
		//ui_->doubleSpinBox_Region_Viscosity_->setValue(default_viscosity_value_);		

		this->deafaul_bo_value_		  = 1.0;
		//ui_->doubleSpinBox_Fluid_Bo_->setValue(this->deafaul_bo_value_);

		this->default_oildensity_value_ = 10.0;
		//ui_->doubleSpinBox_Fluid_OilDensity_->setValue(this->default_oildensity_value_);

  //      viscosity_values_[0] = ui_->doubleSpinBox_Region_Viscosity_->value();
  //      bo_values_[0] = ui_->doubleSpinBox_Fluid_Bo_->value();
  //      oildensity_values_[0] = ui_->doubleSpinBox_Fluid_OilDensity_->value();

		this->reset();

    }

	void FluidWidget::reset()
	{
		ui_->radioButton_Singlephase_->setChecked(true);

		ui_->doubleSpinBox_Region_Viscosity_->setValue(default_viscosity_value_);
		ui_->doubleSpinBox_Fluid_Bo_->setValue(this->deafaul_bo_value_);
		ui_->doubleSpinBox_Fluid_OilDensity_->setValue(this->default_oildensity_value_);

		viscosity_values_[0] = ui_->doubleSpinBox_Region_Viscosity_->value();
		bo_values_[0] = ui_->doubleSpinBox_Fluid_Bo_->value();
		oildensity_values_[0] = ui_->doubleSpinBox_Fluid_OilDensity_->value();
	}

    void FluidWidget::clear()
    {
		ui_->radioButton_Singlephase_->setChecked(true);
		
		ui_->doubleSpinBox_Region_Viscosity_->setValue(default_viscosity_value_);			
		ui_->doubleSpinBox_Fluid_Bo_->setValue(this->deafaul_bo_value_);	
		ui_->doubleSpinBox_Fluid_OilDensity_->setValue(this->default_oildensity_value_);

		viscosity_values_[0] = ui_->doubleSpinBox_Region_Viscosity_->value();
		bo_values_[0] = ui_->doubleSpinBox_Fluid_Bo_->value();
		oildensity_values_[0] = ui_->doubleSpinBox_Fluid_OilDensity_->value();

    }

    void FluidWidget::setupWidget()
    {

        phase_methods_names_.resize(2);
        phase_methods_names_[0] = "By Water Saturation per Region";
        phase_methods_names_[1] = "By Oil Density";

        ui_->comboBox_PhaseMethods_->addItem(phase_methods_names_[0]);
        ui_->comboBox_PhaseMethods_->addItem(phase_methods_names_[1]);
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

        connect(ui_->horizontalSlider_Bo_,static_cast<void (QSlider::*)(int)>(&QSlider::sliderMoved), this, [=]( int value )
        {
            bo_values_[0] = static_cast<double>( value );
            ui_->doubleSpinBox_Fluid_Bo_->setValue( bo_values_[0] );
        });

        connect( ui_->doubleSpinBox_Fluid_Bo_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                 this, [=]( double value )
        {
            bo_values_[0] = value;
            ui_->horizontalSlider_Bo_->setValue(static_cast<int>( bo_values_[0] ) );
        });

        connect(ui_->horizontalSlider_Fluid_OilDensity_,static_cast<void (QSlider::*)(int)>(&QSlider::sliderMoved), this, [=]( int value )
        {
            oildensity_values_[0] = static_cast<double>( value );
            ui_->doubleSpinBox_Fluid_OilDensity_->setValue( oildensity_values_[0] );
        });

        connect( ui_->doubleSpinBox_Fluid_OilDensity_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                 this, [=]( double value )
        {
            oildensity_values_[0] = value;
            ui_->horizontalSlider_Fluid_OilDensity_->setValue(static_cast<int>( oildensity_values_[0] ) );
        });

        /// @FIXME September
        connect( ui_->radioButton_Singlephase_, &QRadioButton::toggled, [=]( bool status_ )
        {
            ui_->comboBox_PhaseMethods_->setEnabled(false);
            ui_->label_Fluid_OilDensity_->setEnabled( false );
            ui_->doubleSpinBox_Fluid_OilDensity_->setEnabled( false );
            ui_->horizontalSlider_Fluid_OilDensity_->setEnabled( false );

            emit setSinglePhase();

            phase_method_.first = 1;

        } );

        connect(ui_->radioButton_Multiphase_, &QRadioButton::clicked, this, [=]( bool status_ )
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
                ui_->label_Fluid_OilDensity_->setEnabled( false );
                ui_->doubleSpinBox_Fluid_OilDensity_->setEnabled( false );
                ui_->horizontalSlider_Fluid_OilDensity_->setEnabled( false );

                std::cout << "By Water Saturation per Region" << std::endl;
                emit setSaturationPerRegion();
                phase_method_.second = 1;
            }
            /// By API Grabity
            else
            {
                ui_->label_Fluid_OilDensity_->setEnabled( true );
                ui_->doubleSpinBox_Fluid_OilDensity_->setEnabled( true );
                ui_->horizontalSlider_Fluid_OilDensity_->setEnabled( true );

				std::cout << "By API Gravity" << std::endl;
                emit setAPIGravity();
                phase_method_.second = 2;
            }
        } );



//        connect( ui_->radioButton_Singlephase_, &QRadioButton::toggled, [=](){ emit singlePhase(); } );
//        connect( ui_->radioButton_Multiphase_, &QRadioButton::toggled, [=](){ emit multiPhase(); } );


    }

    void FluidWidget::getFluidData(std::vector<double>& _viscosity_values, std::vector< double >& _bo_values,
                                   std::vector< double >& _oildensity_values, std::pair<int, int>& _phase_method )
    {
        _viscosity_values.clear();
        _viscosity_values = this->viscosity_values_;

        _bo_values.clear();
        _bo_values = this->bo_values_;

        _oildensity_values.clear();
        _oildensity_values = this->oildensity_values_;
		        
        _phase_method = this->phase_method_;
    }

    void FluidWidget::setFluidData(const std::vector<double>& _viscosity_values, const std::vector<double>& _bo_values)
    {
    }



}    /* namespace RRM */
