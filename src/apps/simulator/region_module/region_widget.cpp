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
                this->doubleSpinbBox_low_permeability_->setSuffix(" (mD)");
                this->doubleSpinbBox_high_permeability_->setSuffix(" (mD)");
                this->doubleSpinbBox_low_porosity_->setSuffix(" (--)");
                this->doubleSpinbBox_high_porosity_->setSuffix(" (--)");

				doubleSpinBox_Region_Water_Saturation_->setSuffix(" (--)");

                ///@September
                emit this->ui_->radioButton_Linear_->toggled(true);

                ui_->spinBox_Number_of_Regions_->setVisible( false );
                ui_->pushButton->setVisible( false );

				this->inverted_permeability_->setToolTip("Check it to increase values bottom up");
				this->inverted_porosity_->setToolTip("Check it to increase values top down");

        }

        void RegionWidget::setupWidget()
        {
            this->number_of_regions_ = 0;


            /// @FIXME September   POROSITY
            slider_porosity_ = new QSlider(Qt::Orientation::Horizontal);
            slider_porosity_->setVisible(false);
            qxt_span_slider_porosity_ = new QxtSpanSlider(Qt::Orientation::Horizontal);
            qxt_span_slider_porosity_->setHandleMovementMode(QxtSpanSlider::HandleMovementMode::NoCrossing);
			this->is_reversed_porosity_ = false;
			qxt_span_slider_porosity_->setInvertedAppearance(this->is_reversed_porosity_);

            gradient_porosity_Label_ = new QLabel(tr("Porosity"));
            doubleSpinbBox_low_porosity_ = new QDoubleSpinBox();
            doubleSpinbBox_high_porosity_ = new QDoubleSpinBox();
			inverted_porosity_ = new QCheckBox();

            this->ui_->gridLayout_Region_Attributes_->addWidget(gradient_porosity_Label_, 0, 0);
            this->ui_->gridLayout_Region_Attributes_->addWidget(doubleSpinbBox_low_porosity_, 0, 1);
            this->ui_->gridLayout_Region_Attributes_->addWidget(qxt_span_slider_porosity_, 1, 2, 1, 8);
			this->ui_->gridLayout_Region_Attributes_->addWidget(inverted_porosity_, 1, 9);
            this->ui_->gridLayout_Region_Attributes_->addWidget(doubleSpinbBox_high_porosity_, 0, 8);
            this->porosity_position_ = std::make_tuple<int, int, int, int>(1, 1, 1, 8);


			low_porosity_ = 0.01;
			high_porosity_ = 0.99;
			default_low_porosity_ = 0.28;
			defaul_high_porosity_ = 0.32;

			doubleSpinbBox_low_porosity_->setMinimum(low_porosity_);
			doubleSpinbBox_low_porosity_->setMaximum(high_porosity_);
			doubleSpinbBox_low_porosity_->setDecimals(2);
			doubleSpinbBox_high_porosity_->setMinimum(low_porosity_);
			doubleSpinbBox_high_porosity_->setMaximum(high_porosity_);
			doubleSpinbBox_high_porosity_->setDecimals(2);

			qxt_span_slider_porosity_->setMinimum(0);
			qxt_span_slider_porosity_->setMaximum(100);
			slider_porosity_->setMinimum(0);
			slider_porosity_->setMaximum(100);

            /// @FIXME September  PERMEABILITY
            slider_permeability_ = new QSlider(Qt::Orientation::Horizontal);
            slider_permeability_->setVisible(false);
            qxt_span_slider_permeability_ = new QxtSpanSlider(Qt::Orientation::Horizontal);
            qxt_span_slider_permeability_->setHandleMovementMode(QxtSpanSlider::HandleMovementMode::NoCrossing);
			this->is_reversed_permeability_ = false;
			qxt_span_slider_permeability_->setInvertedAppearance(this->is_reversed_permeability_);

            gradient_permeability_Label_ = new QLabel(tr("Permeability"));
            doubleSpinbBox_low_permeability_ = new QDoubleSpinBox();
            doubleSpinbBox_high_permeability_ = new QDoubleSpinBox();
			inverted_permeability_ = new QCheckBox();

            this->ui_->gridLayout_Region_Attributes_->addWidget(gradient_permeability_Label_, 2, 0);
            this->ui_->gridLayout_Region_Attributes_->addWidget(doubleSpinbBox_low_permeability_, 2, 1);
            //this->ui_->gridLayout_Region_Attributes_->addWidget(qxt_span_slider_permeability_, 1, 2, 1, 5);
			this->ui_->gridLayout_Region_Attributes_->addWidget(qxt_span_slider_permeability_, 3, 1, 1, 8);
			this->ui_->gridLayout_Region_Attributes_->addWidget(inverted_permeability_, 3, 9);
            this->ui_->gridLayout_Region_Attributes_->addWidget(doubleSpinbBox_high_permeability_, 2, 8);
            this->permeability_position_ = std::make_tuple<int, int, int, int>(3, 1, 1, 8);

			low_permeability_ = 0.001;
			high_permeability_ = 10000.0;
			default_low_permeability_ = 150.0;
			default_high_permeability_ = 250.0;

			doubleSpinbBox_low_permeability_->setMinimum(low_permeability_);
			doubleSpinbBox_low_permeability_->setMaximum(high_permeability_);
            doubleSpinbBox_low_permeability_->setDecimals(3);
			doubleSpinbBox_high_permeability_->setMinimum(low_permeability_);
			doubleSpinbBox_high_permeability_->setMaximum(high_permeability_);
            doubleSpinbBox_high_permeability_->setDecimals(3);

            qxt_span_slider_permeability_->setMinimum(1);
            qxt_span_slider_permeability_->setMaximum(100);
            slider_permeability_->setMinimum(1);
            slider_permeability_->setMaximum(100);
			/// @FIXME January 2018

			/// @FIXME September	 Water Saturation
			slider_Water_Saturation_ = new QSlider(Qt::Orientation::Horizontal);
			label_Water_Saturation_ = new QLabel(tr("Water Saturation"));
			doubleSpinBox_Region_Water_Saturation_ = new QDoubleSpinBox();
			
			this->ui_->gridLayout_Region_Attributes_->addWidget(label_Water_Saturation_, 4, 0);
			this->ui_->gridLayout_Region_Attributes_->addWidget(doubleSpinBox_Region_Water_Saturation_, 4, 1);
			this->ui_->gridLayout_Region_Attributes_->addWidget(slider_Water_Saturation_, 4, 2, 1, 8);
			
			water_saturation_position_ = std::make_tuple<int, int, int, int>(4, 2, 1, 8);

			// Values in the GUI
			low_water_saturation_  = 0.0;
			high_water_saturation_ = 1.0;
			// Initial values in the GUI
			defaul_low_water_saturation_  = 0.0;
			defaul_high_water_saturation_ = 1.0;

			slider_Water_Saturation_->setMinimum(0);
			slider_Water_Saturation_->setMaximum(100);
			doubleSpinBox_Region_Water_Saturation_->setMinimum(low_water_saturation_);
			doubleSpinBox_Region_Water_Saturation_->setMaximum(high_water_saturation_);

			slider_Water_Saturation_->setEnabled(false);
			label_Water_Saturation_->setEnabled(false);
            doubleSpinBox_Region_Water_Saturation_->setEnabled(false);

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
			permeability_gradient_values_.clear();
			porosity_gradient_values_.clear();
			saturation_values.clear();

			permeability_values.clear();
			porosity_values.clear();

			this->createRegions(this->region_colors_);
		}


        void RegionWidget::getRegionData(int& _number_of_regions,
                                         std::vector<double>& _positions,
                                         std::vector<double>& _permeability_values,
                                         std::vector<double>& _porosity_values,
                                         std::vector<double >& _staturation_values,
                                         std::map<int, std::pair<double, double> >& _porosity_gratdients,
                                         std::map<int, std::pair<double, double> >& _permeability_gratdients
                                         )
        {
            /// Ensure number of regions
            _number_of_regions = this->number_of_regions_;

            _positions.resize(3 * this->number_of_regions_);

            _permeability_values.resize(this->number_of_regions_);
            _porosity_values.resize(this->number_of_regions_);
            _staturation_values.resize(this->number_of_regions_);

            for (int it = 0; it < this->number_of_regions_; it++)
            {
                _positions[it * 3 + 0] = positions_values[it].x();
                _positions[it * 3 + 1] = positions_values[it].y();
                _positions[it * 3 + 2] = positions_values[it].z();

                _permeability_values[it]	= permeability_values[it];
                _porosity_values[it]		= porosity_values[it];
                _staturation_values[it]     = saturation_values[it];

				if (ui_->radioButton_Linear_->isChecked())
				{
					_porosity_gratdients[it].first = this->porosity_gradient_values_.at(it).first;
					_porosity_gratdients[it].second = this->porosity_gradient_values_.at(it).first;

					_permeability_gratdients[it].first = this->permeability_gradient_values_.at(it).first;
					_permeability_gratdients[it].second = this->permeability_gradient_values_.at(it).first;
				}
				else
				{
					_porosity_gratdients[it] = this->porosity_gradient_values_.at(it);
					_permeability_gratdients[it] = this->permeability_gradient_values_.at(it);
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

                //@see http://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
                /// 0 to 10101
                /// Saturation
				connect(slider_Water_Saturation_, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), this, [=]()
                {

                    double ex = static_cast<double>(0.01*slider_Water_Saturation_->value());

					doubleSpinBox_Region_Water_Saturation_->setValue(ex);

                    saturation_values[ui_->comboBox_Region_->currentIndex()] = ex;

                });

                /// FIXME September
				connect(doubleSpinBox_Region_Water_Saturation_, &QDoubleSpinBox::editingFinished, this, [=]()
                {

                    double ex = doubleSpinBox_Region_Water_Saturation_->value();

                    int i = static_cast<int>( 100.0*ex);

					slider_Water_Saturation_->setValue(i);

                    saturation_values[ui_->comboBox_Region_->currentIndex()] = ex;

                });


                //// @FIXME September Porosity

                connect(qxt_span_slider_porosity_, &QxtSpanSlider::lowerValueChanged, this, [=]()
                {
					double ex = 0.01 + ( (high_porosity_ - low_porosity_)*0.01 )*static_cast<double>(resersePorosity(this->is_reversed_porosity_, qxt_span_slider_porosity_->lowerValue()));
                    doubleSpinbBox_low_porosity_->setValue(ex);

                    porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = ex;
                });

                    connect(doubleSpinbBox_low_porosity_, &QDoubleSpinBox::editingFinished, this, [=]()
                    {
                        double ex = doubleSpinbBox_low_porosity_->value();
						int i = static_cast<int>((ex - 0.01) / ((high_porosity_ - low_porosity_)*0.01));
                        qxt_span_slider_porosity_->setLowerPosition(i);

                        ///@September
                        slider_porosity_->setValue(i);

                        porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = doubleSpinbBox_low_porosity_->value();
                    });

                    connect(qxt_span_slider_porosity_, &QxtSpanSlider::upperValueChanged, this, [=]()
                {
					double ex = 0.01 + ((high_porosity_ - low_porosity_)*0.01)*static_cast<double>(resersePorosity(this->is_reversed_porosity_, qxt_span_slider_porosity_->upperValue()));
                    doubleSpinbBox_high_porosity_->setValue(ex);
                    porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = ex;
                });

                    connect(doubleSpinbBox_high_porosity_, &QDoubleSpinBox::editingFinished, this, [=]()
                    {
                        double ex = doubleSpinbBox_high_porosity_->value();
						int i = static_cast<int>((ex - 0.01) / ((high_porosity_ - low_porosity_)*0.01));
                        qxt_span_slider_porosity_->setUpperPosition(i);
                        porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = doubleSpinbBox_high_porosity_->value();
                    });

                //// @FIXME September Permeability
					connect(qxt_span_slider_permeability_, &QxtSpanSlider::lowerValueChanged, this, [=]()
                {
					double ex = -3.0 + (0.07)*static_cast<double>(reversePermeability(this->is_reversed_permeability_, qxt_span_slider_permeability_->lowerValue()));
                    ex = std::pow(10, ex);
                    //std::cout << "Step LINEAR" << qxt_span_slider_permeability_->lowerValue() << std::endl;

                    doubleSpinbBox_low_permeability_->setValue(ex);
                    permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = ex;
                });

                    connect(doubleSpinbBox_low_permeability_, &QDoubleSpinBox::editingFinished, this, [=]()
                    {
           
						if (this->is_reversed_permeability_)
						{
							if (doubleSpinbBox_low_permeability_->value() < doubleSpinbBox_high_permeability_->value())
							{
								doubleSpinbBox_low_permeability_->setValue(doubleSpinbBox_high_permeability_->value());
							}											 
						}
						else
						{
							if (doubleSpinbBox_low_permeability_->value() > doubleSpinbBox_high_permeability_->value())
							{
								doubleSpinbBox_low_permeability_->setValue(doubleSpinbBox_high_permeability_->value());
							}
						}

						double p = doubleSpinbBox_low_permeability_->value();
						double ex = std::log10(p);
						int i = static_cast<int>((ex + 3.0) / 0.07);

                        /// @see https://stackoverflow.com/questions/4146140/qslider-value-changed-signal

                        qxt_span_slider_permeability_->blockSignals(true);
						qxt_span_slider_permeability_->setLowerPosition(i);
                        qxt_span_slider_permeability_->blockSignals(false);
                        ///@September
                        /// @see https://stackoverflow.com/questions/4146140/qslider-value-changed-signal
                        slider_permeability_->blockSignals(true);
                        slider_permeability_->setValue(i);
                        slider_permeability_->blockSignals(false);

                        permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = doubleSpinbBox_low_permeability_->value();
                    });

					connect(qxt_span_slider_permeability_, &QxtSpanSlider::upperValueChanged, this, [=]()
                {
					double ex = -3.0 + (0.07)*static_cast<double>(reversePermeability(is_reversed_permeability_, qxt_span_slider_permeability_->upperValue()));
                    ex= std::pow(10, ex);
                    //std::cout << "Step upper" << qxt_span_slider_permeability_->upperValue() << std::endl;

                    doubleSpinbBox_high_permeability_->setValue(ex);
                    permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = ex;
                });

                    connect(doubleSpinbBox_high_permeability_, &QDoubleSpinBox::editingFinished, this, [=]()
                    {

						if (this->is_reversed_permeability_)
						{
							if (doubleSpinbBox_high_permeability_->value() > doubleSpinbBox_low_permeability_->value())
							{
								doubleSpinbBox_high_permeability_->setValue(doubleSpinbBox_low_permeability_->value());
							}
						}
						else
						{
							if (doubleSpinbBox_high_permeability_->value() < doubleSpinbBox_low_permeability_->value())
							{
								doubleSpinbBox_high_permeability_->setValue(doubleSpinbBox_low_permeability_->value());
							}
						}

                        double p = doubleSpinbBox_high_permeability_->value();
                        double ex = std::log10(p);
                        int i = static_cast<int>((ex + 3.0) / 0.07);

                        /// @see https://stackoverflow.com/questions/4146140/qslider-value-changed-signal
                        qxt_span_slider_permeability_->blockSignals(true);
						qxt_span_slider_permeability_->setUpperPosition(i);
                        qxt_span_slider_permeability_->blockSignals(false);

                        permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = doubleSpinbBox_high_permeability_->value();

                    });



                connect(this->ui_->radioButton_Linear_, &QRadioButton::toggled, this, [=](bool _checked)
                {
                    if (_checked == true)
                    {
                        ///@September
                        /// Exchange the QSlider and QxtSpanSlider - Porosity
                        this->ui_->gridLayout_Region_Attributes_->removeWidget(qxt_span_slider_porosity_);
                        this->ui_->gridLayout_Region_Attributes_->addWidget(slider_porosity_, std::get<0>(porosity_position_), std::get<1>(porosity_position_), std::get<2>(porosity_position_), std::get<3>(porosity_position_));
                        slider_porosity_->setVisible(true);
                        qxt_span_slider_porosity_->setVisible(false);

                        this->doubleSpinbBox_high_porosity_->setEnabled(false);

                        /// Exchange the QSlider and QxtSpanSlider - Permeability
                        this->ui_->gridLayout_Region_Attributes_->removeWidget(qxt_span_slider_permeability_);
                        this->ui_->gridLayout_Region_Attributes_->addWidget(slider_permeability_, std::get<0>(permeability_position_), std::get<1>(permeability_position_), std::get<2>(permeability_position_), std::get<3>(permeability_position_));
                        slider_permeability_->setVisible(true);
                        qxt_span_slider_permeability_->setVisible(false);

                        this->doubleSpinbBox_high_permeability_->setEnabled(false);

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

                        /// Exchange the QSlider and QxtSpanSlider - Permeability
                        this->ui_->gridLayout_Region_Attributes_->removeWidget(slider_permeability_);
                        this->ui_->gridLayout_Region_Attributes_->addWidget(qxt_span_slider_permeability_, std::get<0>(permeability_position_), std::get<1>(permeability_position_), std::get<2>(permeability_position_), std::get<3>(permeability_position_));
                        slider_permeability_->setVisible(false);
                        qxt_span_slider_permeability_->setVisible(true);

                        this->doubleSpinbBox_high_permeability_->setEnabled(true);
                    }
                });


                //// @FIXME September Permeability
				connect(slider_permeability_, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), this, [=]()
                {
                    double ex = -3.0 + (0.07)*static_cast<double>(slider_permeability_->value());
                    ex = std::pow(10, ex);

					//std::cout << "Step CONSTANT " << slider_permeability_->value() << ex << std::endl;

                    doubleSpinbBox_low_permeability_->setValue(ex);

                    permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = ex;
                    permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = ex;
                });

                //// @FIXME September Permeability
				connect(slider_porosity_, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), this, [=]()
                {
					double ex = 0.01 + ((high_porosity_ - low_porosity_)*0.01)*static_cast<double>(slider_porosity_->value());
                    doubleSpinbBox_low_porosity_->setValue(ex);

					//std::cout << "Step CONSTANT " << slider_porosity_->value() << ex << std::endl;

                    porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = ex;
                    porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = ex;
                });

				/// FIXME January 2018
				connect(this->inverted_permeability_, &QCheckBox::toggled, this, [=](bool _is_checked)
				{
					this->is_reversed_permeability_ = _is_checked;
					qxt_span_slider_permeability_->setInvertedAppearance(this->is_reversed_permeability_);
					double tmp = 0.0;
					tmp = this->doubleSpinbBox_high_permeability_->value();
					this->doubleSpinbBox_high_permeability_->setValue(this->doubleSpinbBox_low_permeability_->value());
					this->doubleSpinbBox_low_permeability_->setValue(tmp);

					if (_is_checked)
					{
						this->inverted_permeability_->setToolTip("Check it to increase values bottom up");
					}
					else
					{
						this->inverted_permeability_->setToolTip("Check it to increase values top down");
					}
				});
				
                connect(this->inverted_porosity_, &QCheckBox::toggled, this, [=](bool _is_checked)
                {
                        this->is_reversed_porosity_ = _is_checked;
						qxt_span_slider_porosity_->setInvertedAppearance(this->is_reversed_porosity_);
                        double tmp = 0.0;
                        tmp = this->doubleSpinbBox_high_porosity_->value();
                        this->doubleSpinbBox_high_porosity_->setValue(this->doubleSpinbBox_low_porosity_->value());
                        this->doubleSpinbBox_low_porosity_->setValue(tmp);

						if (_is_checked)
						{
							this->inverted_porosity_->setToolTip("Check it to increase values bottom up");
						}
						else
						{
							this->inverted_porosity_->setToolTip("Check it to increase values top down");
						}

                });

        }

        /// @FIXME There is a region ZERO that is represented as 1 ....
        /// Update text box with the current information
        void RegionWidget::updateRegionsWidget(int _index)
        {

            /// @FIXME Me September
            doubleSpinbBox_low_porosity_->setValue(porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].first);
            emit doubleSpinbBox_low_porosity_->editingFinished();
            doubleSpinbBox_high_porosity_->setValue(porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].second);
            emit doubleSpinbBox_high_porosity_->editingFinished();

            doubleSpinbBox_low_permeability_->setValue(permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].first);
            emit doubleSpinbBox_low_permeability_->editingFinished();
            doubleSpinbBox_high_permeability_->setValue(permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].second);
            emit doubleSpinbBox_high_permeability_->editingFinished();

			/// @FIXME Me September
			doubleSpinBox_Region_Water_Saturation_->setValue(saturation_values[ui_->comboBox_Region_->currentIndex()]);
			emit doubleSpinBox_Region_Water_Saturation_->editingFinished();


            QPixmap pixmap_ = QPixmap( 15, 15 );
            pixmap_.fill( regions_colors[ _index ] );
            lb_region_color->setPixmap( pixmap_ );
            lb_region_color->update();


        }

        void RegionWidget::createRegions( const std::map< int,  std::vector< int > >& region_colors /*int _number_of_well, const std::vector< float >& colors_*/ )
        {
                QStringList stringList;

                this->number_of_regions_ = static_cast< int >( region_colors.size() );

                for( auto it: region_colors )
                {
                    int index_ = it.first;
                    stringList.push_back("Region " + QString::number( index_ ) );

                    int r = it.second[ 0 ];
                    int g = it.second[ 1 ];
                    int b = it.second[ 2 ];


//                    QPixmap pixmap_ = QPixmap( 15, 15 );
//                    pixmap_.fill( QColor( r, g, b ) );
//                    lb_region_color->setPixmap( pixmap_ );
//                    lb_region_color->update();

                    regions_colors[ index_ ] =  QColor( r, g, b );


					if (permeability_gradient_values_.count(index_) == 0)
                    {

                        positions_values[index_].x() = 0.0;
                        positions_values[index_].y() = 0.0;
                        positions_values[index_].z() = 0.0;

                        /// @FIXME Me September
                        doubleSpinbBox_low_porosity_->setValue(0.28);
                        emit doubleSpinbBox_low_porosity_->editingFinished();
                        doubleSpinbBox_high_porosity_->setValue(0.32);
                        emit doubleSpinbBox_high_porosity_->editingFinished();

                        porosity_gradient_values_[index_].first = 0.28;
                        porosity_gradient_values_[index_].second = 0.32;

                        doubleSpinbBox_low_permeability_->setValue(150.0);
                        emit doubleSpinbBox_low_permeability_->editingFinished();
                        doubleSpinbBox_high_permeability_->setValue(250.0);
                        emit doubleSpinbBox_high_permeability_->editingFinished();

                        permeability_gradient_values_[index_].first = 150.0;
                        permeability_gradient_values_[index_].second = 250.0;

                        saturation_values[index_] = 0.0;
                        doubleSpinBox_Region_Water_Saturation_->setValue(0.0);
						emit doubleSpinBox_Region_Water_Saturation_->editingFinished();

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

        void RegionWidget::setRegionDepth(float _depth)
        {
            /// Coordinate System for FlowDiagnostic is  <x, z, y>
            /// @FIXEME Be Careful about the coordinate system

            this->depth_ = _depth;

            for (auto index : positions_values)
            {
                positions_values[index.first][1] = this->depth_;
            }

            this->updateRegionsWidget(ui_->comboBox_Region_->currentIndex());
        }

        void RegionWidget::updateRegionPosition(const std::map< int, Eigen::Vector3f >& _positions)
        {
                /// Coordinate System for FlowDiagnostic is  <x, z, y>
                /// @FIXEME Be Careful about the coordinate system
                for (auto index : _positions)
                {
                    //
                    positions_values[index.first][0] = index.second.x();
                    // Z
                    positions_values[index.first][2] = index.second.y();
                    // Y
                    positions_values[index.first][1] = index.second.z()/*this->depth_*/;

                    std::cout << "region " << index.first << ": " << index.second.x() << ", "
                              << index.second.y() << ", " << index.second.z() << std::endl << std::flush;

                }

                this->updateRegionsWidget(ui_->comboBox_Region_->currentIndex());
        }


		void RegionWidget::setByRegionSaturation(bool _option)
		{
			slider_Water_Saturation_->setEnabled(_option);
			label_Water_Saturation_->setEnabled(_option);
			doubleSpinBox_Region_Water_Saturation_->setEnabled(_option);
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
