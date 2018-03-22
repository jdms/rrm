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

			/// Grid positions at the double slider inputs
			int i = 0;
			int j = 0;

            /// @FIXME September  PERMEABILITY			
            slider_permeability_ = new QSlider(Qt::Orientation::Horizontal);
            slider_permeability_->setVisible(false);
            qxt_span_slider_permeability_ = new QxtSpanSlider(Qt::Orientation::Horizontal);
            qxt_span_slider_permeability_->setHandleMovementMode(QxtSpanSlider::HandleMovementMode::NoCrossing);
			this->is_reversed_permeability_ = false;
			qxt_span_slider_permeability_->setInvertedAppearance(this->is_reversed_permeability_);

            gradient_permeability_Label_ = new QLabel(tr("Permeability"));
            doubleSpinbBox_low_permeability_ = new QDoubleSpinBox();
				label_bottom_pearmeability_ = new QLabel(tr("Bottom"));
            doubleSpinbBox_high_permeability_ = new QDoubleSpinBox();
				label_top_pearmeability_ = new QLabel(tr("Top"));
			inverted_permeability_ = new QPushButton("Inverted");
			inverted_permeability_->setCheckable(true);

			this->ui_->gridLayout_Region_Attributes_->addWidget(gradient_permeability_Label_, i, j);

				this->ui_->gridLayout_Region_Attributes_->addWidget(doubleSpinbBox_high_permeability_, i, j + 1);
				this->ui_->gridLayout_Region_Attributes_->addWidget(label_top_pearmeability_, i, j + 2);

				this->ui_->gridLayout_Region_Attributes_->addWidget(qxt_span_slider_permeability_, i + 1, j + 2, 1, 8);
				this->ui_->gridLayout_Region_Attributes_->addWidget(inverted_permeability_, i + 1, j + 9);

				this->ui_->gridLayout_Region_Attributes_->addWidget(doubleSpinbBox_low_permeability_, i, j + 8);
				this->ui_->gridLayout_Region_Attributes_->addWidget(label_bottom_pearmeability_, i, j + 9);

			this->permeability_position_ = std::make_tuple<int, int, int, int>(i + 1, j + 1, 1, 8);

			range_low_permeability_ = 0.001;
			range_high_permeability_ = 10000.0;
			default_low_permeability_ = 150.0;
			default_high_permeability_ = 250.0;

			doubleSpinbBox_low_permeability_->setMinimum(range_low_permeability_);
			doubleSpinbBox_low_permeability_->setMaximum(range_high_permeability_);
            doubleSpinbBox_low_permeability_->setDecimals(3);
			doubleSpinbBox_high_permeability_->setMinimum(range_low_permeability_);
			doubleSpinbBox_high_permeability_->setMaximum(range_high_permeability_);
            doubleSpinbBox_high_permeability_->setDecimals(3);

            qxt_span_slider_permeability_->setMinimum(1);
            qxt_span_slider_permeability_->setMaximum(100);
            slider_permeability_->setMinimum(1);
            slider_permeability_->setMaximum(100);

			/// @FIXME September   POROSITY
			i += 2;
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

			permeability_GUI_.resize(3);
			std::vector<QString> labels = { "Permeability X", "Permeability Y", "Permeability Z" };

			for (std::size_t index = 0; index < permeability_GUI_.size(); index++)
			{
				i += 2;
				/// @FIXME September  PERMEABILITY			
				permeability_GUI_[index].slider_ = new QSlider(Qt::Orientation::Horizontal);
				permeability_GUI_[index].slider_->setVisible(false);
				permeability_GUI_[index].qxt_span_slider_ = new QxtSpanSlider(Qt::Orientation::Horizontal);
				permeability_GUI_[index].qxt_span_slider_->setHandleMovementMode(QxtSpanSlider::HandleMovementMode::NoCrossing);
				permeability_GUI_[index].qxt_span_slider_->setInvertedAppearance(this->is_reversed_permeability_);

				permeability_GUI_[index].gradient_Label_ = new QLabel(tr(labels[index].toStdString().c_str()));
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
			}


			/// @FIXME September	 Water Saturation
			i += 2;
			slider_Water_Saturation_ = new QSlider(Qt::Orientation::Horizontal);
			label_Water_Saturation_ = new QLabel(tr("Water Saturation"));
			doubleSpinBox_Region_Water_Saturation_ = new QDoubleSpinBox();
			
			this->ui_->gridLayout_Region_Attributes_->addWidget(label_Water_Saturation_, i, 0);
			this->ui_->gridLayout_Region_Attributes_->addWidget(doubleSpinBox_Region_Water_Saturation_, i, j+1);
			this->ui_->gridLayout_Region_Attributes_->addWidget(slider_Water_Saturation_, i, j+2, 1, 8);
			
			water_saturation_position_ = std::make_tuple<int, int, int, int>(i + 0, j + 2, 1, 8);

			// Values in the GUI
			low_water_saturation_  = 0.0;
			high_water_saturation_ = 1.0;
			// Initial values in the GUI
			default_water_saturation_  = 0.0;


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
			std::vector<double >& _pct,
			std::vector<double >& _siw,
			std::vector<double >& _sort_factor,
			std::vector<double >& _staturation_values)
        {
            /// Ensure number of regions
            _number_of_regions = this->number_of_regions_;         

            _staturation_values.resize(this->number_of_regions_);

            for (int it = 0; it < this->number_of_regions_; it++)
            {
                _staturation_values[it]     = saturation_values[it];

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

                //// @FIXME September Permeability
					connect(qxt_span_slider_permeability_, &QxtSpanSlider::lowerValueChanged, this, [=]()
                {
					double ex = -3.0 + (0.07)*static_cast<double>(reversePermeability(is_inverted_permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()], qxt_span_slider_permeability_->lowerValue()));
                    ex = std::pow(10, ex);
                    //std::cout << "Step LINEAR" << qxt_span_slider_permeability_->lowerValue() << std::endl;

                    doubleSpinbBox_low_permeability_->setValue(ex);
                    permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = ex;
                });

                    connect(doubleSpinbBox_low_permeability_, &QDoubleSpinBox::editingFinished, this, [=]()
                    {
           

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
					double ex = -3.0 + (0.07)*static_cast<double>(reversePermeability(is_inverted_permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()], qxt_span_slider_permeability_->upperValue()));
                    ex= std::pow(10, ex);
                    //std::cout << "Step upper" << qxt_span_slider_permeability_->upperValue() << std::endl;

                    doubleSpinbBox_high_permeability_->setValue(ex);
                    permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = ex;
                });

                    connect(doubleSpinbBox_high_permeability_, &QDoubleSpinBox::editingFinished, this, [=]()
                    {

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

                        /// Exchange the QSlider and QxtSpanSlider - Permeability
                        this->ui_->gridLayout_Region_Attributes_->removeWidget(slider_permeability_);
                        this->ui_->gridLayout_Region_Attributes_->addWidget(qxt_span_slider_permeability_, std::get<0>(permeability_position_), std::get<1>(permeability_position_), std::get<2>(permeability_position_), std::get<3>(permeability_position_));
                        slider_permeability_->setVisible(false);
                        qxt_span_slider_permeability_->setVisible(true);

                        this->doubleSpinbBox_high_permeability_->setEnabled(true);

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

							permeability_GUI_[index].doubleSpinbBox_high_->setEnabled(true);
						}
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
					double ex = 0.01 + ((range_high_porosity_ - range_low_porosity_)*0.01)*static_cast<double>(slider_porosity_->value());
                    doubleSpinbBox_low_porosity_->setValue(ex);

					//std::cout << "Step CONSTANT " << slider_porosity_->value() << ex << std::endl;

                    porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = ex;
                    porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = ex;
                });

				/// FIXME January 2018
				connect(this->inverted_permeability_, &QPushButton::toggled, this, [=](bool _is_checked)
				{
					this->is_reversed_permeability_ = _is_checked;
					qxt_span_slider_permeability_->setInvertedAppearance(this->is_reversed_permeability_);
					double tmp = 0.0;
					tmp = this->doubleSpinbBox_high_permeability_->value();
					this->doubleSpinbBox_high_permeability_->setValue(this->doubleSpinbBox_low_permeability_->value());
					this->doubleSpinbBox_low_permeability_->setValue(tmp);


					permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = this->doubleSpinbBox_low_permeability_->value();
					permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = this->doubleSpinbBox_high_permeability_->value();
					is_inverted_permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()] = _is_checked;

					if (_is_checked)
					{
						this->inverted_permeability_->setToolTip("Check it to increase values bottom up");
					}
					else
					{
						this->inverted_permeability_->setToolTip("Check it to increase values top down");
					}
				});
				
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
						permeability_GUI_[index].qxt_span_slider_->setInvertedAppearance(this->is_reversed_permeability_);
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


			inverted_permeability_->blockSignals(true);
			inverted_permeability_->setChecked(is_inverted_permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()]);
			inverted_permeability_->blockSignals(false);
			qxt_span_slider_permeability_->setInvertedAppearance(is_inverted_permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()]);

            doubleSpinbBox_low_permeability_->setValue(permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].first);
            emit doubleSpinbBox_low_permeability_->editingFinished();
            doubleSpinbBox_high_permeability_->setValue(permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].second);
            emit doubleSpinbBox_high_permeability_->editingFinished();

			/// @FIXME Me September
			doubleSpinBox_Region_Water_Saturation_->setValue(saturation_values[ui_->comboBox_Region_->currentIndex()]);
			emit doubleSpinBox_Region_Water_Saturation_->editingFinished();

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

				inverted_permeability_->setChecked(false);
				inverted_porosity_->setChecked(false);

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
                        /// @FIXME Me September
                        doubleSpinbBox_low_porosity_->setValue(this->default_low_porosity_);
                        emit doubleSpinbBox_low_porosity_->editingFinished();
                        doubleSpinbBox_high_porosity_->setValue(this->default_high_porosity_);
                        emit doubleSpinbBox_high_porosity_->editingFinished();

                        porosity_gradient_values_[index_].first = default_low_porosity_;
                        porosity_gradient_values_[index_].second = default_high_porosity_;
						is_inverted_porosity_gradient_values_[index_] = false;

                        doubleSpinbBox_low_permeability_->setValue(this->default_low_permeability_);
                        emit doubleSpinbBox_low_permeability_->editingFinished();
                        doubleSpinbBox_high_permeability_->setValue(this->default_high_permeability_);
                        emit doubleSpinbBox_high_permeability_->editingFinished();

                        permeability_gradient_values_[index_].first = this->default_low_permeability_;
                        permeability_gradient_values_[index_].second = this->default_high_permeability_;
						is_inverted_permeability_gradient_values_[index_] = false;

						saturation_values[index_] = this->default_water_saturation_;
						doubleSpinBox_Region_Water_Saturation_->setValue(this->default_water_saturation_);
						emit doubleSpinBox_Region_Water_Saturation_->editingFinished();

						//// @FIXME January 2018
						for (std::size_t index = 0; index < permeability_GUI_.size(); index++)
						{
							permeability_GUI_[index].doubleSpinbBox_low_->setValue(permeability_GUI_[index].default_low_value_);
							emit permeability_GUI_[index].doubleSpinbBox_low_->editingFinished();
							permeability_GUI_[index].doubleSpinbBox_high_->setValue(permeability_GUI_[index].default_high_value_);
							emit permeability_GUI_[index].doubleSpinbBox_high_->editingFinished();

							permeability_GUI_[index].gradient_values_[index_].first = permeability_GUI_[index].default_low_value_;
							permeability_GUI_[index].gradient_values_[index_].second = permeability_GUI_[index].default_high_value_;
							permeability_GUI_[index].is_inverted_[index_] = false;
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
