/*
 * WellWidget.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: felipe
 */

#include "WellWidget.hpp"

#include <iostream>

namespace RRM
{
        WellWidget::WellWidget ( QWidget * _parent)  : QWidget (_parent)
        {
                this->ui_ = new Ui::WellWidgetForm;
                this->ui_->setupUi(this);

                this->setupWidget();
                this->createConnections();

				/// FIXEME

				this->ui_->spinBox_Well_Type_->setVisible(false);
				this->ui_->spinBox_Well_Sign_->setVisible(false);
				this->ui_->label_Well_Sign_->setVisible(false);

				this->ui_->horizontalSlider_Well_Bottom_->setVisible(false);
				this->ui_->label_Bottom_->setVisible(false);


				this->ui_->horizontalSlider_Well_Top_->setVisible(false);
				this->ui_->label_Top_->setVisible(false);
        }

		std::map< int, int >			   WellWidget::getWellsType() const
		{
			return wells_type_;
		};

		std::map< int, int >               WellWidget::getWellsSign() const
		{
			return wells_sign_;
		};

		std::map< int, double >            WellWidget::getWellsPressure() const
		{
			return wells_pressure_;
		};

		std::map< int, Eigen::Vector4d >   WellWidget::getWellsPosition() const
		{
			return wells_position_;
		};

		int WellWidget::getNumberOfWells() const
		{
			return this->number_of_wells_;
		}

		void WellWidget::setBoundingBoxDimension(const Eigen::Vector3d& _box_min, const Eigen::Vector3d& _box_max)
		{
			dim_min_ = _box_min;
			dim_max_ = _box_max;
		}

        void WellWidget::setupWidget()
        {
                // The default number of well of the module is 2
                this->number_of_wells_ = 2;

				// Creating Fixed valeu for wells
				this->wells_type_[0] = 1;
				this->wells_pressure_[0] = 400;
				this->wells_sign_[0] = 1;
				this->wells_position_[0] = Eigen::Vector4d(0.0, 0.0, 0.0, 100.0);
				this->wells_range_[0] = Eigen::Vector2d(100.0, 0.0);

				this->wells_type_[1] = 1;
				this->wells_pressure_[1] = 300;
				this->wells_sign_[1] = -1;
				this->wells_position_[1] = Eigen::Vector4d(0.0, 0.0, 0.0, 100.0);
				this->wells_range_[1] = Eigen::Vector2d(100.0, 0.0);

                this->createWells(2);

        }
		/// OLD HWU Interface
		void WellWidget::getWellData(int& _number_of_well, std::vector< unsigned int >& _types, std::vector< double >& _pressure, std::vector< int >& _sign, std::vector<Eigen::Vector4d >& _positions, std::vector<Eigen::Vector2d>& _range)
		{
			_number_of_well = this->number_of_wells_;

			_types.resize(_number_of_well);
			_pressure.resize(_number_of_well);
			_sign.resize(_number_of_well);
			_positions.resize(_number_of_well);
			_range.resize(_number_of_well);

			for (int it = 0; it < _number_of_well; it++)
			{
				_types[it] = static_cast<unsigned int>(this->wells_type_[it]);
				_pressure[it] = this->wells_pressure_[it];
				_sign[it] = this->wells_sign_[it];
				_positions[it] = this->wells_position_.at(it);
				_range[it] = this->wells_range_.at(it);
			}

		}
        /// Create Qt Signal-Slot connection
        void WellWidget::createConnections()
        {
                //@see http://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
                connect(this->ui_->spinBox_Number_of_Wells_, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](){ this->createWells(this->ui_->spinBox_Number_of_Wells_->value()); });
                /// We added to one, because the comboBox index start from 0
                connect(this->ui_->comboBox_Well_, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, [=](){ this->updateWellsWidget(this->ui_->comboBox_Well_->currentIndex()); });

				connect(this->ui_->horizontalSlider_Well_Depth_, &QSlider::valueChanged, this, [=]()
				{
					wells_position_[this->ui_->comboBox_Well_->currentIndex()].w() = static_cast<double>(this->ui_->horizontalSlider_Well_Depth_->value());					
					emit wellPositionsHasChanged(wells_position_);
				});


				/// FIXME

				connect(this->ui_->doubleSpinBox_Well_Pressure_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=]()
				{
					this->wells_pressure_[this->ui_->comboBox_Well_->currentIndex()] = this->ui_->doubleSpinBox_Well_Pressure_->value();

				});


				connect(this->ui_->comboBox_Well_Type_, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, [=]()
				{ 
					this->wells_sign_[this->ui_->comboBox_Well_->currentIndex()] = wellType();
					this->createWells(this->getNumberOfWells());
				});

				connect(this->ui_->horizontalSlider_Well_Bottom_, &QSlider::valueChanged, this, [=]()
				{
					if (this->ui_->horizontalSlider_Well_Bottom_->value() <= this->ui_->horizontalSlider_Well_Top_->value())
					{
						this->ui_->horizontalSlider_Well_Bottom_->setValue(this->ui_->horizontalSlider_Well_Top_->value());
					}

					this->wells_range_[this->ui_->comboBox_Well_->currentIndex()].x() = this->ui_->horizontalSlider_Well_Bottom_->value();
				});

				connect(this->ui_->horizontalSlider_Well_Top_, &QSlider::valueChanged, this, [=]()
				{
					if (this->ui_->horizontalSlider_Well_Top_->value() >= this->ui_->horizontalSlider_Well_Bottom_->value())
					{
						this->ui_->horizontalSlider_Well_Top_->setValue(this->ui_->horizontalSlider_Well_Bottom_->value());						
					}

					this->wells_range_[this->ui_->comboBox_Well_->currentIndex()].y() = this->ui_->horizontalSlider_Well_Top_->value();
				});


        }
        /// Update text box with the current information
        void WellWidget::updateWellsWidget(int _index)
        {
                this->ui_->spinBox_Well_Type_->setValue(wells_type_[_index]);
			
                this->ui_->doubleSpinBox_Well_Pressure_->setValue(wells_pressure_[_index]);
                this->ui_->spinBox_Well_Sign_->setValue(wells_sign_[_index]);
				this->ui_->horizontalSlider_Well_Depth_->setValue(wells_position_[_index].w());

				/// FIXEME 

				setWellType(wells_sign_[_index]);

				/// July 14
				this->ui_->horizontalSlider_Well_Bottom_->setValue(this->wells_range_[_index].x());
				this->ui_->horizontalSlider_Well_Top_->setValue(this->wells_range_[_index].y());

        }

        void WellWidget::createWells( int _number_of_well)
        {
                QStringList l;

                this->number_of_wells_ = _number_of_well;

                for (auto i = 0; i < this->number_of_wells_; i++)
                {
                    l.push_back("Well " + QString::number(i + 1));		

					if (wells_position_.count(i) == 0)
					{
						wells_position_[i] = Eigen::Vector4d(0.0, 0.0, 0.0, 100.0); 
						wells_sign_[i] = -1;
						wells_pressure_[i] = 300;

						this->ui_->horizontalSlider_Well_Bottom_->setValue(0.0);
						this->ui_->horizontalSlider_Well_Top_->setValue(100.0);
					}
                }

                this->ui_->comboBox_Well_->clear();
                this->ui_->comboBox_Well_->insertItems(0, l);

				// Set the Current position to the last one added
				this->ui_->comboBox_Well_->setCurrentIndex(this->ui_->comboBox_Well_->count()-1);

                this->updateWellsWidget(this->ui_->comboBox_Well_->currentIndex());

                /// It tells the wellScene to create well items
                emit numberOfWells(this->number_of_wells_);
        }

        void WellWidget::updateWellPosition(const std::map< int, Eigen::Vector4d >& _positions)
        {       
				for ( int i = 0; i < this->number_of_wells_; i++)
				{
					/// Well Positio in Model Coordinates
					wells_position_[i].x() = dim_min_.x() + (dim_max_.x() - dim_min_.x()) * (_positions.at(i).x() / (dim_max_.x() - dim_min_.x()));
					wells_position_[i].y() = dim_min_.y() + (dim_max_.y() - dim_min_.y()) * (_positions.at(i).y() / (dim_max_.y() - dim_min_.y()));
					wells_position_[i].z() = dim_max_.z();
					//float x = dim_min.x() + (dim_max.x() - dim_min.x()) * (p.second.x() / (dim_max.x() - dim_min.x()));
					//float y = dim_min.y() + (dim_max.y() - dim_min.y()) * (p.second.y() / (dim_max.y() - dim_min.y()));
					//float z = dim_max.z() + (dim_max.z() - dim_min.z()) * (p.second.z() / 100.0f);
				}

				emit wellPositionsHasChanged(wells_position_);

        }

		/// FIXME

		int WellWidget::wellType( )
		{
			if (this->ui_->comboBox_Well_Type_->currentIndex() == 0)
			{
				return 1;
			}
			else
			{
				return -1;
			}			
		}

		void WellWidget::setWellType(int i)
		{
			if ( i == 1)
			{
				this->ui_->comboBox_Well_Type_->setCurrentIndex(0); /// injector
			}
			else
			{
				this->ui_->comboBox_Well_Type_->setCurrentIndex(1); /// producer
			}
		}




} /* namespace RRM */
