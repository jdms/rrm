/*
 * WellWidget.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: felipe
 */

#include "RegionWidget.hpp"

#include <iostream>

namespace RRM
{
        RegionWidget::RegionWidget ( QWidget * _parent)  : QWidget (_parent)
        {
                this->ui_ = new Ui::RegionWidgetForm;
                this->ui_->setupUi(this);

				this->ui_->label_Region_Point_->setVisible(false);
				this->ui_->doubleSpinBox_Region_Point_X_->setVisible(false);
				this->ui_->doubleSpinBox_Region_Point_Y_->setVisible(false);
				this->ui_->doubleSpinBox_Region_Point_Z_->setVisible(false);

				this->ui_->doubleSpinBox_Region_Viscosity_->setVisible(false);
				this->ui_->label_Region_Viscosity_->setVisible(false);
				this->ui_->horizontalSlider_Viscosity_->setVisible(false);

                this->setupWidget();
                this->createConnections();
        }

        void RegionWidget::setupWidget()
        {
                this->number_of_regions_ = 0;

        }


		int RegionWidget::getNumberOfRegion() const
		{
			return this->number_of_regions_;
		}

		void RegionWidget::clear()
		{
		}

		void RegionWidget::getRegionData(int& _number_of_regions,
										 std::vector<double>& _positions,
										 std::vector<double>& _permeability_values,
										 std::vector<double>& _porosity_values,
										 std::vector<double>& _viscosity_values
										 )
		{
			/// Ensure number of regions
			_number_of_regions = this->number_of_regions_;

			_permeability_values.resize(this->number_of_regions_);
			_porosity_values.resize(this->number_of_regions_);
			_viscosity_values.resize(this->number_of_regions_);
			_positions.resize(3 * this->number_of_regions_);

			for (int it = 0; it < this->number_of_regions_; it++)
			{
				_permeability_values[it]	= permeability_values[it];
				_porosity_values[it]		= porosity_values[it];
				_viscosity_values[it]		= viscosity_values[it];
				_positions[it * 3 + 0]		= positions_values[it].x();
				_positions[it * 3 + 1]		= positions_values[it].y();
				_positions[it * 3 + 2]		= positions_values[it].z();
			}
		}

		void RegionWidget::setRegionData(int& _number_of_regions,
			std::vector<double>& _positions,
			std::vector<double>& _permeability_values,
			std::vector<double>& _porosity_values,
			std::vector<double>& _viscosity_values
			)
		{

		}
		

        /// Create Qt Signal-Slot connection
        void RegionWidget::createConnections()
        {
				//@see http://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5

                /// New GUI ---->
                //// REGION
                connect(ui_->spinBox_Number_of_Regions_, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &RegionWidget::createRegions);
                connect(ui_->comboBox_Region_, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, [=](){ updateRegionsWidget(ui_->comboBox_Region_->currentIndex()); });

                //@see http://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5

				connect(ui_->horizontalSlider_Permeability_, static_cast<void (QSlider::*)(int)>(&QSlider::sliderMoved), this, [=]()
                {
                    double ex = -3.0 + (0.07)*static_cast<double>(ui_->horizontalSlider_Permeability_->value());
                    double p = std::pow(10, ex);
                    //std::cout << "Step " << horizontalSlider_Permeability->value() << " 10^" << ex << "= " << p << std::endl;
                    ui_->doubleSpinBox_Region_Permeability_->setValue(p);

					permeability_values[ui_->comboBox_Region_->currentIndex()] = ui_->doubleSpinBox_Region_Permeability_->value();
                    permeability_slider_values[ui_->comboBox_Region_->currentIndex()] = ui_->horizontalSlider_Permeability_->value();

                });


				///// FIXME July 14
				connect(ui_->doubleSpinBox_Region_Permeability_, &QDoubleSpinBox::editingFinished, this, [=]()
				{
					double p = ui_->doubleSpinBox_Region_Permeability_->value();

					double ex = std::log10(p);
					
					int i = static_cast<int>( (ex + 3.0) / 0.07);
					
					ui_->horizontalSlider_Permeability_->setValue(i);

					permeability_values[ui_->comboBox_Region_->currentIndex()] = ui_->doubleSpinBox_Region_Permeability_->value();
					permeability_slider_values[ui_->comboBox_Region_->currentIndex()] = ui_->horizontalSlider_Permeability_->value();

				});

				connect(ui_->horizontalSlider_Porosity_, static_cast<void (QSlider::*)(int)>(&QSlider::sliderMoved), this, [=]()
                {
                    double ex = 0.01 + (0.0034)*static_cast<double>(ui_->horizontalSlider_Porosity_->value());
                    ui_->doubleSpinBox_Region_Porosity_->setValue(ex);

					porosity_values[ui_->comboBox_Region_->currentIndex()] = ui_->doubleSpinBox_Region_Porosity_->value();
                    porosity_slider_values[ui_->comboBox_Region_->currentIndex()] = ui_->horizontalSlider_Porosity_->value();
                });

				///// FIXME July 14
				connect(ui_->doubleSpinBox_Region_Porosity_, &QDoubleSpinBox::editingFinished, this, [=]()
				{
					permeability_values[ui_->comboBox_Region_->currentIndex()] = ui_->doubleSpinBox_Region_Porosity_->value();

					double ex = ui_->doubleSpinBox_Region_Porosity_->value();

					int i = static_cast<int>((ex - 0.01) / 0.0034);

					ui_->horizontalSlider_Porosity_->setValue(i);

					porosity_values[ui_->comboBox_Region_->currentIndex()] = ui_->doubleSpinBox_Region_Porosity_->value();
					porosity_slider_values[ui_->comboBox_Region_->currentIndex()] = ui_->horizontalSlider_Porosity_->value();
				});


                //connect(ui_->horizontalSlider_Viscosity_, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), this, [=]()
                //{
                //    double ex = 0.1 + (0.1)*static_cast<double>(ui_->horizontalSlider_Viscosity_->value());
                //    ui_->doubleSpinBox_Region_Viscosity_->setValue(ex);
                //    /// FIXME Viscosity have to be the same for all model for now, util a better
                //    /// interface if design.
                //    for (auto& v : viscosity_slider_values)
                //    {
                //        v.second = ui_->horizontalSlider_Viscosity_->value();
                //    }
                //});
				      
				/// FIXME July 14

                //connect(ui_->doubleSpinBox_Region_Viscosity_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=]()
                //{
                //    /// FIXME Viscosity have to be the same for all model for now, until a better
                //    /// interface is designed.
                //    for (auto& v : viscosity_values)
                //    {
                //        v.second = ui_->doubleSpinBox_Region_Viscosity_->value();
                //    }

                //});

        }

		/// @FIXME There is a region ZERO that is represented as 1 ....
        /// Update text box with the current information
        void RegionWidget::updateRegionsWidget(int _index)
        {
				//emit ui_->horizontalSlider_Permeability_->sliderMoved(permeability_slider_values[_index]);
				//ui_->horizontalSlider_Permeability_->setValue(permeability_slider_values[_index]);
			ui_->doubleSpinBox_Region_Permeability_->setValue(permeability_values[_index]);
			ui_->horizontalSlider_Permeability_->setValue(permeability_slider_values[_index]);
			//emit ui_->doubleSpinBox_Region_Permeability_->editingFinished();
				

				//emit ui_->horizontalSlider_Porosity_->sliderMoved(porosity_slider_values[_index]);
				//ui_->horizontalSlider_Porosity_->setValue(porosity_slider_values[_index]);
			ui_->doubleSpinBox_Region_Porosity_->setValue(porosity_values[_index]);
			ui_->horizontalSlider_Porosity_->setValue(porosity_slider_values[_index]);
                //ui_->doubleSpinBox_Region_Viscosity_->setValue(viscosity_values[_index]);
                //ui_->horizontalSlider_Viscosity_->setValue(viscosity_slider_values[_index]);
                //ui_->doubleSpinBox_Region_Point_X_->setValue(positions_values[_index].x());
                //ui_->doubleSpinBox_Region_Point_Y_->setValue(positions_values[_index].y());
                //ui_->doubleSpinBox_Region_Point_Z_->setValue(positions_values[_index].z());
        }

        void RegionWidget::createRegions( int _number_of_well)
        {
                QStringList stringList;

                this->number_of_regions_ = _number_of_well;

                for (auto i = 0; i < this->number_of_regions_ ; i++)
                {
                    stringList.push_back("Region " + QString::number(i + 1));

                    if (permeability_values.count(i) == 0)
                    {
                        ui_->horizontalSlider_Permeability_->setValue(75);
                        emit ui_->horizontalSlider_Permeability_->sliderMoved(75);
                        ui_->horizontalSlider_Porosity_->setValue(88);
						emit ui_->horizontalSlider_Porosity_->sliderMoved(88);
                        //ui_->horizontalSlider_Viscosity_->setValue(ui_->horizontalSlider_Viscosity_->value());
						//emit ui_->horizontalSlider_Viscosity_->sliderMoved(ui_->horizontalSlider_Viscosity_->value());

                        permeability_values[i] = ui_->doubleSpinBox_Region_Permeability_->value();
                        permeability_slider_values[i] = 75;
                        porosity_values[i] = ui_->doubleSpinBox_Region_Porosity_->value();
                        porosity_slider_values[i] = 88;
                        //viscosity_values[i] = ui_->doubleSpinBox_Region_Viscosity_->value();
                        //viscosity_slider_values[i] = 9;
                        positions_values[i].x() = 0.0;
                        positions_values[i].y() = 0.0;
                        positions_values[i].z() = 0.0;
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
                    positions_values[index.first][1] = this->depth_;

                }

                this->updateRegionsWidget(ui_->comboBox_Region_->currentIndex());
        }

} /* namespace RRM */
