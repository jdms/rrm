/*
 * WellWidget.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: felipe
 */

#include "region_widget.hpp"

#include <iostream>

namespace RRM
{
        RegionWidget::RegionWidget ( QWidget * _parent)  : QWidget (_parent)
        {
                this->ui_ = new Ui::RegionWidgetForm;
                this->ui_->setupUi(this);

                this->setupWidget();
                this->createConnections();


                /// @FIXME September 2017
                this->doubleSpinbBox_low_porosity_->setSuffix(" (--)");
                this->doubleSpinbBox_high_porosity_->setSuffix(" (--)");

				//doubleSpinBox_Region_Water_Saturation_->setSuffix(" (--)");

                ///@September
                emit this->ui_->radioButton_Linear_->toggled(true);

                ui_->spinBox_Number_of_Regions_->setVisible( false );
                ui_->pushButton->setVisible( false );

				this->inverted_porosity_->setToolTip("Check it to increase values top down");

				this->is_isotropic_permeability_ = true;
				ui_->checkBox_Isotropic_Permeability_->setChecked(this->is_isotropic_permeability_);

        }

        void RegionWidget::setupWidget()
        {
            this->number_of_regions_ = 0;

			/// Grid positions at the double slider inputs
			int i = 0;
			int j = 0;

			/// @FIXME January 2018 -- @TODO Create functors to the slider values
			permeability_GUI_.resize(3);
			std::vector<QString> permeability_labels = { "Permeability X", "Permeability Y", "Permeability Z" };

			for (std::size_t index = 0; index < permeability_GUI_.size(); index++)
			{
				/// @FIXME September  PERMEABILITY			
				permeability_GUI_[index].slider_ = new QSlider(Qt::Orientation::Horizontal);
				permeability_GUI_[index].slider_->setVisible(false);
				permeability_GUI_[index].qxt_span_slider_ = new QxtSpanSlider(Qt::Orientation::Horizontal);
				permeability_GUI_[index].qxt_span_slider_->setHandleMovementMode(QxtSpanSlider::HandleMovementMode::NoCrossing);
				permeability_GUI_[index].qxt_span_slider_->setInvertedAppearance(false);

				permeability_GUI_[index].gradient_Label_ = new QLabel(tr(permeability_labels[index].toStdString().c_str()));
				permeability_GUI_[index].doubleSpinbBox_low_ = new QDoubleSpinBox();
				permeability_GUI_[index].label_bottom_ = new QLabel(tr("Bottom"));
				permeability_GUI_[index].doubleSpinbBox_high_ = new QDoubleSpinBox();
				permeability_GUI_[index].label_top_ = new QLabel(tr("Top"));
				permeability_GUI_[index].inverted_ = new QPushButton("Inverted");
				permeability_GUI_[index].inverted_->setCheckable(true);

				this->ui_->gridLayout_Region_Attributes_->addWidget(permeability_GUI_[index].gradient_Label_, i, j);

				this->ui_->gridLayout_Region_Attributes_->addWidget(permeability_GUI_[index].doubleSpinbBox_high_, i, j + 1);
				this->ui_->gridLayout_Region_Attributes_->addWidget(permeability_GUI_[index].label_top_, i, j + 2);

				this->ui_->gridLayout_Region_Attributes_->addWidget(permeability_GUI_[index].qxt_span_slider_, i + 1, j + 2, 1, 8);
				this->ui_->gridLayout_Region_Attributes_->addWidget(permeability_GUI_[index].inverted_, i + 1, j + 9);

				this->ui_->gridLayout_Region_Attributes_->addWidget(permeability_GUI_[index].doubleSpinbBox_low_, i, j + 8);
				this->ui_->gridLayout_Region_Attributes_->addWidget(permeability_GUI_[index].label_bottom_, i, j + 9);

				permeability_GUI_[index].position_in_the_grid_ = std::make_tuple<int, int, int, int>(i + 1, j + 1, 1, 8);

				permeability_GUI_[index].range_low_value_ = 0.001;
				permeability_GUI_[index].range_high_value_ = 10000.0;
				permeability_GUI_[index].default_low_value_ = 150.0;
				permeability_GUI_[index].default_high_value_ = 250.0;

				permeability_GUI_[index].doubleSpinbBox_low_->setMinimum(permeability_GUI_[index].range_low_value_);
				permeability_GUI_[index].doubleSpinbBox_low_->setMaximum(permeability_GUI_[index].range_high_value_);
				permeability_GUI_[index].doubleSpinbBox_low_->setDecimals(3);
				permeability_GUI_[index].doubleSpinbBox_high_->setMinimum(permeability_GUI_[index].range_low_value_);
				permeability_GUI_[index].doubleSpinbBox_high_->setMaximum(permeability_GUI_[index].range_high_value_);
				permeability_GUI_[index].doubleSpinbBox_high_->setDecimals(3);

				permeability_GUI_[index].qxt_span_slider_->setMinimum(1);
				permeability_GUI_[index].qxt_span_slider_->setMaximum(100);
				permeability_GUI_[index].slider_->setMinimum(1);
				permeability_GUI_[index].slider_->setMaximum(100);
				/// Next Position in the Grid
				i += 2;
			}

			/// @FIXME September   POROSITY
			slider_porosity_ = new QSlider(Qt::Orientation::Horizontal);
			slider_porosity_->setVisible(false);
			qxt_span_slider_porosity_ = new QxtSpanSlider(Qt::Orientation::Horizontal);
			qxt_span_slider_porosity_->setHandleMovementMode(QxtSpanSlider::HandleMovementMode::NoCrossing);
			this->is_reversed_porosity_ = false;
			qxt_span_slider_porosity_->setInvertedAppearance(this->is_reversed_porosity_);

			gradient_porosity_Label_ = new QLabel(tr("Porosity"));
			doubleSpinbBox_low_porosity_ = new QDoubleSpinBox();
			label_bottom_porosity_ = new QLabel(tr("Bottom"));
			doubleSpinbBox_high_porosity_ = new QDoubleSpinBox();
			label_top_porosity_ = new QLabel(tr("Top"));
			inverted_porosity_ = new QPushButton("Inverted");
			inverted_porosity_->setCheckable(true);

			this->ui_->gridLayout_Region_Attributes_->addWidget(gradient_porosity_Label_, i, j);

			this->ui_->gridLayout_Region_Attributes_->addWidget(doubleSpinbBox_high_porosity_, i, j + 1);
			this->ui_->gridLayout_Region_Attributes_->addWidget(label_top_porosity_, i, j + 2);

			this->ui_->gridLayout_Region_Attributes_->addWidget(qxt_span_slider_porosity_, i + 1, j + 2, 1, 8);
			this->ui_->gridLayout_Region_Attributes_->addWidget(inverted_porosity_, i + 1, j + 9);

			this->ui_->gridLayout_Region_Attributes_->addWidget(doubleSpinbBox_low_porosity_, i, j + 8);
			this->ui_->gridLayout_Region_Attributes_->addWidget(label_bottom_porosity_, i, j + 9);

			this->porosity_position_ = std::make_tuple<int, int, int, int>(i + 1, j + 1, 1, 8);

			/// Accepted Range Values
			range_low_porosity_ = 0.01;
			range_high_porosity_ = 0.99;
			/// Default Values
			default_low_porosity_ = 0.28;
			default_high_porosity_ = 0.32;

			doubleSpinbBox_low_porosity_->setMinimum(range_low_porosity_);
			doubleSpinbBox_low_porosity_->setMaximum(range_high_porosity_);
			doubleSpinbBox_low_porosity_->setDecimals(2);
			doubleSpinbBox_high_porosity_->setMinimum(range_low_porosity_);
			doubleSpinbBox_high_porosity_->setMaximum(range_high_porosity_);
			doubleSpinbBox_high_porosity_->setDecimals(2);

			qxt_span_slider_porosity_->setMinimum(0);
			qxt_span_slider_porosity_->setMaximum(100);
			slider_porosity_->setMinimum(0);
			slider_porosity_->setMaximum(100);

			/// @FIXME January 2018 -- @TODO Create functors to the slider values
			single_double_input_GUI_.resize(4);
			std::vector<QString> single_double_input_labels = { "Water Saturation", "Threshold Pressure", "Sort Factor","Connate Water Saturation" };
			std::vector<double>  single_double_input_low_values = { 0.0, 0.0, 0.1, 0.0 };
			std::vector<double>  single_double_input_high_values = { 1.0, 100.0, 10.0, 1.0 };
			std::vector<double>  single_double_input_default_values = { 0.0, 0.0, 2.0, 0.0 };

			/// Next Position in the Grid
			i += 2;

			for (std::size_t index = 0; index < single_double_input_GUI_.size(); index++)
			{				
				single_double_input_GUI_[index].label_		   = new QLabel(tr(single_double_input_labels[index].toStdString().c_str()));
				single_double_input_GUI_[index].doubleSpinBox_ = new QDoubleSpinBox();
				single_double_input_GUI_[index].slider_		   = new QSlider(Qt::Orientation::Horizontal);
				
				this->ui_->gridLayout_Region_Attributes_->addWidget(single_double_input_GUI_[index].label_, i, 0);
				this->ui_->gridLayout_Region_Attributes_->addWidget(single_double_input_GUI_[index].doubleSpinBox_, i, j + 1);
				this->ui_->gridLayout_Region_Attributes_->addWidget(single_double_input_GUI_[index].slider_, i, j + 2, 1, 8);

				single_double_input_GUI_[index].position_in_the_grid_ = std::make_tuple<int, int, int, int>(i + 0, j + 2, 1, 8);

				// Values in the GUI
				single_double_input_GUI_[index].range_low_value_  = single_double_input_low_values[index];
				single_double_input_GUI_[index].range_high_value_ = single_double_input_high_values[index];
				single_double_input_GUI_[index].range_size_		  = single_double_input_high_values[index] - single_double_input_low_values[index];
				// Initial values in the GUI
				single_double_input_GUI_[index].default_value_	  = single_double_input_default_values[index];


				single_double_input_GUI_[index].slider_->setMinimum(0);
				single_double_input_GUI_[index].slider_->setMaximum(101);
				single_double_input_GUI_[index].doubleSpinBox_->setMinimum(single_double_input_low_values[index]);
				single_double_input_GUI_[index].doubleSpinBox_->setMaximum(single_double_input_high_values[index]);

				bool is_enable = true;

				single_double_input_GUI_[index].slider_->setEnabled(is_enable);
				single_double_input_GUI_[index].label_->setEnabled(is_enable);
				single_double_input_GUI_[index].doubleSpinBox_->setEnabled(is_enable);

				i += 1;
			}

            lb_region_color = new QLabel();
            lb_region_color->setMaximumSize( 16, 16 );
            lb_region_color->setFrameShape( QFrame::Box );
            lb_region_color->setLineWidth( 1 );


            ui_->horizontalLayout_Region_Number->addWidget( lb_region_color );
            ui_->spinBox_Number_of_Regions_->setVisible( false );
            ui_->pushButton->setVisible( false );

        }

        int RegionWidget::getNumberOfRegion() const
        {
            return this->number_of_regions_;
        }

        void RegionWidget::clear()
        {
			this->reset();
        }

		void RegionWidget::reset()
		{
		
			porosity_gradient_values_.clear();

			for (std::size_t index = 0; index < single_double_input_GUI_.size(); index++)
			{
				single_double_input_GUI_[index].values_.clear();
			}

			//// @FIXME January 2018
			for (std::size_t index = 0; index < permeability_GUI_.size(); index++)
			{
				permeability_GUI_[index].gradient_values_.clear();
				permeability_GUI_[index].is_inverted_.clear();
			}


			this->createRegions(this->region_colors_);
		}

		void RegionWidget::getRegionData(int& _number_of_regions,
			std::map<int, std::pair<double, double> >& _x_permeability_values,
			std::map<int, std::pair<double, double> >& _y_permeability_values,
			std::map<int, std::pair<double, double> >& _z_permeability_values,
			std::map<int, std::pair<double, double> >& _porosity_values,
			std::vector<double>& _saturations_values, /// Water Saturation
			std::vector<double>& _pct_values,		  /// Threshold Pressure
			std::vector<double>& _sort_factor_values, /// Lambda
			std::vector<double>& _siw_values		  /// Connate Water Saturation 
			)
        {
            /// Ensure number of regions
            _number_of_regions = this->number_of_regions_;         
			/// Ensure the size of the vectors
			_pct_values.resize(_number_of_regions);
			_siw_values.resize(_number_of_regions);
			_sort_factor_values.resize(_number_of_regions);
			_saturations_values.resize(_number_of_regions);

            for (int it = 0; it < this->number_of_regions_; it++)
            {
				/// @FIXME January HArd coded names
				/// { "Water Saturation", "Threshold Pressure", "Sort Factor","Connate Water Saturation" };
				_pct_values[it] = single_double_input_GUI_[1].values_[it];
				_siw_values[it] = single_double_input_GUI_[3].values_[it];
				_sort_factor_values[it] = single_double_input_GUI_[2].values_[it];
				_saturations_values[it] = single_double_input_GUI_[0].values_[it];

				if (ui_->radioButton_Linear_->isChecked())
				{
					_porosity_values[it].first = this->porosity_gradient_values_.at(it).first;
					_porosity_values[it].second = this->porosity_gradient_values_.at(it).first;

					_x_permeability_values[it].first	= permeability_GUI_[0].gradient_values_.at(it).first;
					_x_permeability_values[it].second	= permeability_GUI_[0].gradient_values_.at(it).first;
					_y_permeability_values[it].first	= permeability_GUI_[1].gradient_values_.at(it).first;
					_y_permeability_values[it].second	= permeability_GUI_[1].gradient_values_.at(it).first;
					_z_permeability_values[it].first	= permeability_GUI_[2].gradient_values_.at(it).first;
					_z_permeability_values[it].second	= permeability_GUI_[2].gradient_values_.at(it).first;
				}
				else
				{
					_porosity_values[it] = this->porosity_gradient_values_.at(it);
					_x_permeability_values[it] = permeability_GUI_[0].gradient_values_.at(it);
					_y_permeability_values[it] = permeability_GUI_[1].gradient_values_.at(it);
					_z_permeability_values[it] = permeability_GUI_[2].gradient_values_.at(it);
				}

            }

        }

        void RegionWidget::setRegionData( const std::map< int,  std::vector< int > >& region_colors )
        {
            ui_->spinBox_Number_of_Regions_->setValue( region_colors.size() );
			this->region_colors_ = region_colors;
			//createRegions(this->region_colors_/*_number_of_region, colors_*/);
			this->reset();
        }

        void RegionWidget::updateRegionColor( int _index, int red, int green, int blue )
        {
            regions_colors[ _index ] =  QColor( red, green, blue );
            QPixmap pixmap_ = QPixmap( 15, 15 );
            pixmap_.fill( regions_colors[ _index ] );
            lb_region_color->setPixmap( pixmap_ );
            lb_region_color->update();
        }
        /// Create Qt Signal-Slot connection
        void RegionWidget::createConnections()
        {
                //@see http://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
                /// New GUI ---->
                //// REGION
//                connect(ui_->spinBox_Number_of_Regions_, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &RegionWidget::createRegions);
                connect(ui_->comboBox_Region_, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, [=](){ updateRegionsWidget(ui_->comboBox_Region_->currentIndex()); });

                connect( ui_->pushButton, &QPushButton::clicked, this, [=](){ emit getRegions(); } );

				//// @FIXME January 2018
				for (std::size_t index = 0; index < single_double_input_GUI_.size(); index++)
				{
					//@see http://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
					/// 0 to 10101
					/// Saturation
					connect(single_double_input_GUI_[index].slider_, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), this, [=]()
					{
						/// All slider have range into the range:  0-100
						double ex = static_cast<double>( (0.01 * single_double_input_GUI_[index].slider_->value())	* single_double_input_GUI_[index].range_size_);

						single_double_input_GUI_[index].doubleSpinBox_->setValue(ex);

						single_double_input_GUI_[index].values_[ui_->comboBox_Region_->currentIndex()] = ex;

					});

					/// FIXME September
					connect(single_double_input_GUI_[index].doubleSpinBox_, &QDoubleSpinBox::editingFinished, this, [=]()
					{

						double ex = single_double_input_GUI_[index].doubleSpinBox_->value();

						int i = static_cast<int>(100.0*(ex/single_double_input_GUI_[index].range_size_));

						single_double_input_GUI_[index].slider_->blockSignals(true);
						single_double_input_GUI_[index].slider_->setValue(i);
						single_double_input_GUI_[index].slider_->blockSignals(false);

						single_double_input_GUI_[index].values_[ui_->comboBox_Region_->currentIndex()] = ex;

					});
				}


                //// @FIXME September Porosity

                connect(qxt_span_slider_porosity_, &QxtSpanSlider::lowerValueChanged, this, [=]()
                {
					double ex = 0.01 + ((range_high_porosity_ - range_low_porosity_)*0.01)*static_cast<double>(resersePorosity(is_inverted_porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()], qxt_span_slider_porosity_->lowerValue()));
                    doubleSpinbBox_low_porosity_->setValue(ex);

                    porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = ex;
                });

                    connect(doubleSpinbBox_low_porosity_, &QDoubleSpinBox::editingFinished, this, [=]()
                    {


                        double ex = doubleSpinbBox_low_porosity_->value();
						int i = static_cast<int>((ex - 0.01) / ((range_high_porosity_ - range_low_porosity_)*0.01));

						qxt_span_slider_porosity_->blockSignals(true);
						qxt_span_slider_porosity_->setLowerPosition(i);
						qxt_span_slider_porosity_->blockSignals(false);
                       

                        ///@September
						slider_porosity_->blockSignals(true);
                        slider_porosity_->setValue(i);
						slider_porosity_->blockSignals(false);

                        porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = doubleSpinbBox_low_porosity_->value();
                    });

                    connect(qxt_span_slider_porosity_, &QxtSpanSlider::upperValueChanged, this, [=]()
                {
					double ex = 0.01 + ((range_high_porosity_ - range_low_porosity_)*0.01)*static_cast<double>(resersePorosity(is_inverted_porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()], qxt_span_slider_porosity_->upperValue()));
                    doubleSpinbBox_high_porosity_->setValue(ex);
                    porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = ex;
                });

                    connect(doubleSpinbBox_high_porosity_, &QDoubleSpinBox::editingFinished, this, [=]()
                    {


                        double ex = doubleSpinbBox_high_porosity_->value();
						int i = static_cast<int>((ex - 0.01) / ((range_high_porosity_ - range_low_porosity_)*0.01));


						qxt_span_slider_porosity_->blockSignals(true);
						qxt_span_slider_porosity_->setUpperPosition(i);
						qxt_span_slider_porosity_->blockSignals(false);
                  
                        porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = doubleSpinbBox_high_porosity_->value();
                    });

                connect(this->ui_->radioButton_Linear_, &QRadioButton::toggled, this, [=](bool _checked)
                {
					this->is_constant_ = _checked;

                    if (_checked == true)
                    {
                        ///@September
                        /// Exchange the QSlider and QxtSpanSlider - Porosity
                        this->ui_->gridLayout_Region_Attributes_->removeWidget(qxt_span_slider_porosity_);
                        this->ui_->gridLayout_Region_Attributes_->addWidget(slider_porosity_, std::get<0>(porosity_position_), std::get<1>(porosity_position_), std::get<2>(porosity_position_), std::get<3>(porosity_position_));
                        slider_porosity_->setVisible(true);
                        qxt_span_slider_porosity_->setVisible(false);

                        this->doubleSpinbBox_high_porosity_->setEnabled(false);

						//// @FIXME January 2018
						for (std::size_t index = 0; index < permeability_GUI_.size(); index++)
						{
							/// Exchange the QSlider and QxtSpanSlider - Permeability
							this->ui_->gridLayout_Region_Attributes_->removeWidget(permeability_GUI_[index].qxt_span_slider_);
							this->ui_->gridLayout_Region_Attributes_->addWidget(permeability_GUI_[index].slider_, 
								std::get<0>(permeability_GUI_[index].position_in_the_grid_),
								std::get<1>(permeability_GUI_[index].position_in_the_grid_),
								std::get<2>(permeability_GUI_[index].position_in_the_grid_),
								std::get<3>(permeability_GUI_[index].position_in_the_grid_));
							permeability_GUI_[index].slider_->setVisible(true);
							permeability_GUI_[index].qxt_span_slider_->setVisible(false);

							permeability_GUI_[index].doubleSpinbBox_high_->setEnabled(false);

						}
                    }
                    else
                    {
                        ///@September
                        /// Exchange the QSlider and QxtSpanSlider
                        this->ui_->gridLayout_Region_Attributes_->removeWidget(slider_porosity_);
                        this->ui_->gridLayout_Region_Attributes_->addWidget(qxt_span_slider_porosity_, std::get<0>(porosity_position_), std::get<1>(porosity_position_), std::get<2>(porosity_position_), std::get<3>(porosity_position_));
                        slider_porosity_->setVisible(false);
                        qxt_span_slider_porosity_->setVisible(true);

                        this->doubleSpinbBox_high_porosity_->setEnabled(true);

						//// @FIXME January 2018
						for (std::size_t index = 0; index < permeability_GUI_.size(); index++)
						{
							/// Exchange the QSlider and QxtSpanSlider - Permeability
							this->ui_->gridLayout_Region_Attributes_->removeWidget(permeability_GUI_[index].slider_);
							this->ui_->gridLayout_Region_Attributes_->addWidget(permeability_GUI_[index].qxt_span_slider_, 
								std::get<0>(permeability_GUI_[index].position_in_the_grid_),
								std::get<1>(permeability_GUI_[index].position_in_the_grid_),
								std::get<2>(permeability_GUI_[index].position_in_the_grid_),
								std::get<3>(permeability_GUI_[index].position_in_the_grid_));
							permeability_GUI_[index].slider_->setVisible(false);
							permeability_GUI_[index].qxt_span_slider_->setVisible(true);

							permeability_GUI_[index].doubleSpinbBox_high_->setEnabled(true && !is_isotropic_permeability_);
						}
                    }		
                });


                //// @FIXME September Permeability
				connect(slider_porosity_, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), this, [=]()
                {
					double ex = 0.01 + ((range_high_porosity_ - range_low_porosity_)*0.01)*static_cast<double>(slider_porosity_->value());
                    doubleSpinbBox_low_porosity_->setValue(ex);

					//std::cout << "Step CONSTANT " << slider_porosity_->value() << ex << std::endl;

                    porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = ex;
                    porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = ex;
                });

				/// FIXME January 2018
				
				connect(this->inverted_porosity_, &QPushButton::toggled, this, [=](bool _is_checked)
                {
                        this->is_reversed_porosity_ = _is_checked;
						qxt_span_slider_porosity_->setInvertedAppearance(this->is_reversed_porosity_);
                        double tmp = 0.0;
                        tmp = this->doubleSpinbBox_high_porosity_->value();
                        this->doubleSpinbBox_high_porosity_->setValue(this->doubleSpinbBox_low_porosity_->value());
                        this->doubleSpinbBox_low_porosity_->setValue(tmp);

						porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = this->doubleSpinbBox_low_porosity_->value();
						porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = this->doubleSpinbBox_high_porosity_->value();
						is_inverted_porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()] = _is_checked;

						if (_is_checked)
						{
							this->inverted_porosity_->setToolTip("Check it to increase values bottom up");
						}
						else
						{
							this->inverted_porosity_->setToolTip("Check it to increase values top down");
						}

                });


				//// @FIXME January 2018
				for (std::size_t index = 0; index < permeability_GUI_.size(); index++)
				{

					//// @FIXME January XYZ Permeability
					connect(permeability_GUI_[index].qxt_span_slider_, &QxtSpanSlider::lowerValueChanged, this, [=]()
					{
						double ex = -3.0 + (0.07)*static_cast<double>(reversePermeability(permeability_GUI_[index].is_inverted_[ui_->comboBox_Region_->currentIndex()], permeability_GUI_[index].qxt_span_slider_->lowerValue()));
						ex = std::pow(10, ex);
						//std::cout << "Step LINEAR" << qxt_span_slider_permeability_->lowerValue() << std::endl;

						permeability_GUI_[index].doubleSpinbBox_low_->setValue(ex);
						permeability_GUI_[index].gradient_values_[ui_->comboBox_Region_->currentIndex()].first = ex;
					});

					connect(permeability_GUI_[index].doubleSpinbBox_low_, &QDoubleSpinBox::editingFinished, this, [=]()
					{
						double p = permeability_GUI_[index].doubleSpinbBox_low_->value();
						double ex = std::log10(p);
						int i = static_cast<int>((ex + 3.0) / 0.07);

						/// @see https://stackoverflow.com/questions/4146140/qslider-value-changed-signal

						permeability_GUI_[index].qxt_span_slider_->blockSignals(true);
						permeability_GUI_[index].qxt_span_slider_->setLowerPosition(i);
						permeability_GUI_[index].qxt_span_slider_->blockSignals(false);
						///@September
						/// @see https://stackoverflow.com/questions/4146140/qslider-value-changed-signal
						permeability_GUI_[index].slider_->blockSignals(true);
						permeability_GUI_[index].slider_->setValue(i);
						permeability_GUI_[index].slider_->blockSignals(false);

						permeability_GUI_[index].gradient_values_[ui_->comboBox_Region_->currentIndex()].first = permeability_GUI_[index].doubleSpinbBox_low_->value();
					});

					connect(permeability_GUI_[index].qxt_span_slider_, &QxtSpanSlider::upperValueChanged, this, [=]()
					{
						double ex = -3.0 + (0.07)*static_cast<double>(reversePermeability(permeability_GUI_[index].is_inverted_[ui_->comboBox_Region_->currentIndex()], permeability_GUI_[index].qxt_span_slider_->upperValue()));
						ex = std::pow(10, ex);
						//std::cout << "Step upper" << qxt_span_slider_permeability_->upperValue() << std::endl;

						permeability_GUI_[index].doubleSpinbBox_high_->setValue(ex);
						permeability_GUI_[index].gradient_values_[ui_->comboBox_Region_->currentIndex()].second = ex;
					});

					connect(permeability_GUI_[index].doubleSpinbBox_high_, &QDoubleSpinBox::editingFinished, this, [=]()
					{

						double p = permeability_GUI_[index].doubleSpinbBox_high_->value();
						double ex = std::log10(p);
						int i = static_cast<int>((ex + 3.0) / 0.07);

						/// @see https://stackoverflow.com/questions/4146140/qslider-value-changed-signal
						permeability_GUI_[index].qxt_span_slider_->blockSignals(true);
						permeability_GUI_[index].qxt_span_slider_->setUpperPosition(i);
						permeability_GUI_[index].qxt_span_slider_->blockSignals(false);

						permeability_GUI_[index].gradient_values_[ui_->comboBox_Region_->currentIndex()].second = permeability_GUI_[index].doubleSpinbBox_high_->value();

					});

					//// @FIXME January XYZ Permeability
					connect(permeability_GUI_[index].slider_, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), this, [=]()
					{
						double ex = -3.0 + (0.07)*static_cast<double>(permeability_GUI_[index].slider_->value());
						ex = std::pow(10, ex);

						permeability_GUI_[index].doubleSpinbBox_low_->setValue(ex);

						permeability_GUI_[index].gradient_values_[ui_->comboBox_Region_->currentIndex()].first = ex;
						permeability_GUI_[index].gradient_values_[ui_->comboBox_Region_->currentIndex()].second = ex;
					});

					connect(permeability_GUI_[index].inverted_, &QPushButton::toggled, this, [=](bool _is_checked)
					{
						permeability_GUI_[index].qxt_span_slider_->setInvertedAppearance(_is_checked);
						double tmp = 0.0;
						tmp = permeability_GUI_[index].doubleSpinbBox_high_->value();
						permeability_GUI_[index].doubleSpinbBox_high_->setValue(permeability_GUI_[index].doubleSpinbBox_low_->value());
						permeability_GUI_[index].doubleSpinbBox_low_->setValue(tmp);


						permeability_GUI_[index].gradient_values_[ui_->comboBox_Region_->currentIndex()].first = permeability_GUI_[index].doubleSpinbBox_low_->value();
						permeability_GUI_[index].gradient_values_[ui_->comboBox_Region_->currentIndex()].second = permeability_GUI_[index].doubleSpinbBox_high_->value();
						permeability_GUI_[index].is_inverted_[ui_->comboBox_Region_->currentIndex()] = _is_checked;

						if (_is_checked)
						{
							permeability_GUI_[index].inverted_->setToolTip("Check it to increase values bottom up");
						}
						else
						{
							permeability_GUI_[index].inverted_->setToolTip("Check it to increase values top down");
						}
					});
				}


				/// Last Feature from January 2018 release
				connect(ui_->checkBox_Isotropic_Permeability_, &QCheckBox::toggled, this, [=](bool _is_checked)
				{
					/// I like to write code very clear in the names and actions
					is_isotropic_permeability_ = _is_checked;
					if (is_isotropic_permeability_ == true)
					{
						permeability_GUI_[0].gradient_Label_->setText(tr("Permeabilty XYZ"));
						permeability_GUI_[0].doubleSpinbBox_high_->setEnabled(!is_constant_);

						for (std::size_t index = 1; index < permeability_GUI_.size(); index++)
						{
							/// @FIXME September  PERMEABILITY			
							permeability_GUI_[index].slider_->setEnabled(!is_isotropic_permeability_);
							permeability_GUI_[index].qxt_span_slider_->setEnabled(!is_isotropic_permeability_);
							permeability_GUI_[index].qxt_span_slider_->setEnabled(!is_isotropic_permeability_);
							permeability_GUI_[index].qxt_span_slider_->setEnabled(!is_isotropic_permeability_);

							permeability_GUI_[index].gradient_Label_->setEnabled(!is_isotropic_permeability_);
							permeability_GUI_[index].doubleSpinbBox_low_->setEnabled(!is_isotropic_permeability_);
							permeability_GUI_[index].label_bottom_->setEnabled(!is_isotropic_permeability_);

							permeability_GUI_[index].doubleSpinbBox_high_->setEnabled(!is_isotropic_permeability_  && !is_constant_);

							permeability_GUI_[index].label_top_->setEnabled(!is_isotropic_permeability_);
							permeability_GUI_[index].inverted_->setEnabled(!is_isotropic_permeability_);
							permeability_GUI_[index].inverted_->setEnabled(!is_isotropic_permeability_);
						}


					}
					else
					{
						permeability_GUI_[0].gradient_Label_->setText(tr("Permeabilty X"));

						for (std::size_t index = 1; index < permeability_GUI_.size(); index++)
						{
							/// @FIXME September  PERMEABILITY			
							permeability_GUI_[index].slider_->setEnabled(!is_isotropic_permeability_);
							permeability_GUI_[index].qxt_span_slider_->setEnabled(!is_isotropic_permeability_);
							permeability_GUI_[index].qxt_span_slider_->setEnabled(!is_isotropic_permeability_);
							permeability_GUI_[index].qxt_span_slider_->setEnabled(!is_isotropic_permeability_);

							permeability_GUI_[index].gradient_Label_->setEnabled(!is_isotropic_permeability_);
							permeability_GUI_[index].doubleSpinbBox_low_->setEnabled(!is_isotropic_permeability_);
							permeability_GUI_[index].label_bottom_->setEnabled(!is_isotropic_permeability_);

							permeability_GUI_[index].doubleSpinbBox_high_->setEnabled(!is_isotropic_permeability_ && !is_constant_);

							permeability_GUI_[index].label_top_->setEnabled(!is_isotropic_permeability_);
							permeability_GUI_[index].inverted_->setEnabled(!is_isotropic_permeability_);
							permeability_GUI_[index].inverted_->setEnabled(!is_isotropic_permeability_);
						}
					}

				});

        }

        /// @FIXME There is a region ZERO that is represented as 1 ....
        /// Update text box with the current information
        void RegionWidget::updateRegionsWidget(int _index)
        {

            /// @FIXME Me September
			inverted_porosity_->blockSignals(true);
			inverted_porosity_->setChecked(is_inverted_porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()]);
			inverted_porosity_->blockSignals(false);
			qxt_span_slider_porosity_->setInvertedAppearance(is_inverted_porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()]);

            doubleSpinbBox_low_porosity_->setValue(porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].first);
            emit doubleSpinbBox_low_porosity_->editingFinished();
            doubleSpinbBox_high_porosity_->setValue(porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].second);
            emit doubleSpinbBox_high_porosity_->editingFinished();

			/// @FIXME Me September
			//doubleSpinBox_Region_Water_Saturation_->setValue(saturation_values[ui_->comboBox_Region_->currentIndex()]);
			//emit doubleSpinBox_Region_Water_Saturation_->editingFinished();
			for (std::size_t index = 0; index < single_double_input_GUI_.size(); index++)
			{
				single_double_input_GUI_[index].doubleSpinBox_->setValue(single_double_input_GUI_[index].values_[ui_->comboBox_Region_->currentIndex()]);
				emit single_double_input_GUI_[index].doubleSpinBox_->editingFinished();
			}

			//// @FIXME January 2018
			for (std::size_t index = 0; index < permeability_GUI_.size(); index++)
			{
				permeability_GUI_[index].inverted_->blockSignals(true);
				permeability_GUI_[index].inverted_->setChecked(permeability_GUI_[index].is_inverted_[ui_->comboBox_Region_->currentIndex()]);
				permeability_GUI_[index].inverted_->blockSignals(false);
				permeability_GUI_[index].qxt_span_slider_->setInvertedAppearance(permeability_GUI_[index].is_inverted_[ui_->comboBox_Region_->currentIndex()]);

				permeability_GUI_[index].doubleSpinbBox_low_->setValue(permeability_GUI_[index].gradient_values_[ui_->comboBox_Region_->currentIndex()].first);
				emit permeability_GUI_[index].doubleSpinbBox_low_->editingFinished();
				permeability_GUI_[index].doubleSpinbBox_high_->setValue(permeability_GUI_[index].gradient_values_[ui_->comboBox_Region_->currentIndex()].second);
				emit permeability_GUI_[index].doubleSpinbBox_high_->editingFinished();
			}

            QPixmap pixmap_ = QPixmap( 15, 15 );
            pixmap_.fill( regions_colors[ _index ] );
            lb_region_color->setPixmap( pixmap_ );
            lb_region_color->update();


        }

        void RegionWidget::createRegions( const std::map< int,  std::vector< int > >& region_colors /*int _number_of_well, const std::vector< float >& colors_*/ )
        {
                QStringList stringList;

				inverted_porosity_->setChecked(false);

                this->number_of_regions_ = static_cast< int >( region_colors.size() );

                for( auto it: region_colors )
                {
                    int region_index_ = it.first;
					stringList.push_back("Region " + QString::number(region_index_));

                    int r = it.second[ 0 ];
                    int g = it.second[ 1 ];
                    int b = it.second[ 2 ];

					regions_colors[region_index_] = QColor(r, g, b);

					if (porosity_gradient_values_.count(region_index_) == 0)
                    {
                        /// @FIXME Me September
                        doubleSpinbBox_low_porosity_->setValue(this->default_low_porosity_);
                        emit doubleSpinbBox_low_porosity_->editingFinished();
                        doubleSpinbBox_high_porosity_->setValue(this->default_high_porosity_);
                        emit doubleSpinbBox_high_porosity_->editingFinished();

						porosity_gradient_values_[region_index_].first = default_low_porosity_;
						porosity_gradient_values_[region_index_].second = default_high_porosity_;
						is_inverted_porosity_gradient_values_[region_index_] = false;

						//saturation_values[index_] = this->default_water_saturation_;
						//doubleSpinBox_Region_Water_Saturation_->setValue(this->default_water_saturation_);
						//emit doubleSpinBox_Region_Water_Saturation_->editingFinished();

						//// @FIXME January 2018
						for (std::size_t index = 0; index < single_double_input_GUI_.size(); index++)
						{
							single_double_input_GUI_[index].values_[region_index_] = single_double_input_GUI_[index].default_value_;
							single_double_input_GUI_[index].doubleSpinBox_->setValue(single_double_input_GUI_[index].values_[region_index_]);
							emit single_double_input_GUI_[index].doubleSpinBox_->editingFinished();
						}

						//// @FIXME January 2018
						for (std::size_t index = 0; index < permeability_GUI_.size(); index++)
						{
							permeability_GUI_[index].doubleSpinbBox_low_->setValue(permeability_GUI_[index].default_low_value_);
							emit permeability_GUI_[index].doubleSpinbBox_low_->editingFinished();
							permeability_GUI_[index].doubleSpinbBox_high_->setValue(permeability_GUI_[index].default_high_value_);
							emit permeability_GUI_[index].doubleSpinbBox_high_->editingFinished();

							permeability_GUI_[index].gradient_values_[region_index_].first = permeability_GUI_[index].default_low_value_;
							permeability_GUI_[index].gradient_values_[region_index_].second = permeability_GUI_[index].default_high_value_;
							permeability_GUI_[index].is_inverted_[region_index_] = false;
						}

                    }

                }

                ui_->comboBox_Region_->clear();
                ui_->comboBox_Region_->insertItems(0, stringList);

                // Set the Current position to the last one added
                ui_->comboBox_Region_->setCurrentIndex(ui_->comboBox_Region_->count() - 1);
                /// Update the GUI using the current region information
                this->updateRegionsWidget(ui_->comboBox_Region_->currentIndex());

                emit numberOfRegions(this->number_of_regions_);
        }

		void RegionWidget::setByRegionSaturation(bool _option)
		{
			//slider_Water_Saturation_->setEnabled(_option);
			//label_Water_Saturation_->setEnabled(_option);
			//doubleSpinBox_Region_Water_Saturation_->setEnabled(_option);
		}

		int RegionWidget::reversePermeability(bool _is_reversed, int _value)
		{
			int result = _value;

			if (_is_reversed == true)
			{
				result = 101 - _value;
			}
			else
			{
				result = _value;
			}

			return result;
		}

		int RegionWidget::resersePorosity(bool _is_reversed, int _value)
		{
			int result = _value;

			if (_is_reversed == true)
			{
				result = 101 - _value;
			}
			else
			{
				result = _value;
			}

			return result;
		}

} /* namespace RRM */
