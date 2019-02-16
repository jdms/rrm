/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "well_widget.hpp"

#include <iostream>

namespace RRM
{
	WellWidget::WellWidget(QWidget * _parent) : QWidget(_parent)
	{
		this->ui_ = new Ui::WellWidgetForm;
		this->ui_->setupUi(this);

		/// @FIXME September 2017
		qxt_span_slider_ = new QxtSpanSlider(Qt::Orientation::Horizontal);
		qxt_span_slider_->setHandleMovementMode(QxtSpanSlider::HandleMovementMode::NoOverlapping);


		this->ui_->gridLayout->addWidget(qxt_span_slider_, 0, 1, 1, 1);

		this->setupWidget();
		this->createConnections();

		/// FIXEME

		this->ui_->spinBox_Well_Type_->setVisible(false);
		this->ui_->spinBox_Well_Sign_->setVisible(false);
		this->ui_->label_Well_Sign_->setVisible(false);

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

	std::map< int, Eigen::Vector2d >   WellWidget::getWellsRange() const
	{
		return wells_range_;
	};

	std::map< int, Eigen::Vector2d >   WellWidget::getWells2DPosition() const
	{
		return wells_position_in_2D_;
	};

	Eigen::Vector2d					   WellWidget::getDimension2D()
	{
		Eigen::Vector2d dim( dim_max_.x() - dim_min_.x() , dim_max_.y() - dim_min_.y() );
		return dim;
	}

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

		/// @FIXEME September    Bottom max = 100 / Top Max = 0
		connect(qxt_span_slider_, &QxtSpanSlider::lowerValueChanged, this, [=]()
		{
			this->wells_range_[this->ui_->comboBox_Well_->currentIndex()].y() = qxt_span_slider_->lowerValue();
			std::cout << qxt_span_slider_->lowerValue() << " Lower " << std::endl;
			emit wellPositionsHasChanged();
		});

		connect(qxt_span_slider_, &QxtSpanSlider::upperValueChanged, this, [=]()
		{
			this->wells_range_[this->ui_->comboBox_Well_->currentIndex()].x() = qxt_span_slider_->upperValue();
			std::cout << qxt_span_slider_->upperValue() << " Upper " << std::endl;
			emit wellPositionsHasChanged();
		});

	}
	/// Update text box with the current information
	void WellWidget::updateWellsWidget(int _index)
	{
		this->ui_->spinBox_Well_Type_->setValue(wells_type_[_index]);

		this->ui_->doubleSpinBox_Well_Pressure_->setValue(wells_pressure_[_index]);
		this->ui_->spinBox_Well_Sign_->setValue(wells_sign_[_index]);

		/// FIXEME

		setWellType(wells_sign_[_index]);

		/// September SpanSilder

		qxt_span_slider_->setLowerPosition(static_cast<int>(this->wells_range_.at(_index).y()));
		qxt_span_slider_->setUpperPosition(static_cast<int>(this->wells_range_.at(_index).x()));

	}

	void WellWidget::createWells(int _number_of_well)
	{
		QStringList l;

		this->number_of_wells_ = _number_of_well;

		for (auto i = 0; i < this->number_of_wells_; i++)
		{
			l.push_back("Well " + QString::number(i + 1));

			if (wells_position_.count(i) == 0)
			{
				this->wells_position_[i] = Eigen::Vector4d(0.0, 0.0, 0.0, 100.0);
				this->wells_sign_[i] = -1;
				this->wells_pressure_[i] = 300;
				this->wells_range_[i] = Eigen::Vector2d(100.0, 0.0);
				/// SpanSlider
			}
		}

		this->ui_->comboBox_Well_->clear();
		this->ui_->comboBox_Well_->insertItems(0, l);

		// Set the Current position to the last one added
		this->ui_->comboBox_Well_->setCurrentIndex(this->ui_->comboBox_Well_->count() - 1);

		this->updateWellsWidget(this->ui_->comboBox_Well_->currentIndex());

		/// It tells the wellScene to create well items
		emit numberOfWells(this->number_of_wells_);
	}

	void WellWidget::loadWells()
	{
			QStringList l;			

			for (auto i = 0; i < this->number_of_wells_; i++)
			{
				l.push_back("Well " + QString::number(i + 1));
			}

			this->ui_->spinBox_Number_of_Wells_->blockSignals(true);
			this->ui_->spinBox_Number_of_Wells_->setValue(this->number_of_wells_);
			this->ui_->spinBox_Number_of_Wells_->blockSignals(false);

			this->ui_->comboBox_Well_->clear();
			this->ui_->comboBox_Well_->insertItems(0, l);

			// Set the Current position to the last one added
			this->ui_->comboBox_Well_->setCurrentIndex(this->ui_->comboBox_Well_->count() - 1);

			this->updateWellsWidget(this->ui_->comboBox_Well_->currentIndex());
		
	}

	void WellWidget::updateWellPosition(const std::map< int, Eigen::Vector2d >& _positions)
	{
		for (int i = 0; i < this->number_of_wells_; i++)
		{
			/// Well Positio in Model Coordinates
			wells_position_[i].x() = dim_min_.x() + (dim_max_.x() - dim_min_.x()) * (_positions.at(i).x() / (dim_max_.x() - dim_min_.x()));
			wells_position_[i].y() = dim_min_.y() + (dim_max_.y() - dim_min_.y()) * (_positions.at(i).y() / (dim_max_.y() - dim_min_.y()));
			wells_position_[i].z() = dim_max_.z();

			wells_position_in_2D_[i] = _positions.at(i);

			std::cout << "Position of Well " << i << " " << wells_position_in_2D_[i] << std::endl;
			//float x = dim_min.x() + (dim_max.x() - dim_min.x()) * (p.second.x() / (dim_max.x() - dim_min.x()));
			//float y = dim_min.y() + (dim_max.y() - dim_min.y()) * (p.second.y() / (dim_max.y() - dim_min.y()));
			//float z = dim_max.z() + (dim_max.z() - dim_min.z()) * (p.second.z() / 100.0f);
		}
		/// Notify 3D View with information of wells position in "StratMod Space"
		emit wellPositionsHasChanged();
	}

	/// FIXME

	int WellWidget::wellType()
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
		if (i == 1)
		{
			this->ui_->comboBox_Well_Type_->setCurrentIndex(0); /// injector
		}
		else
		{
			this->ui_->comboBox_Well_Type_->setCurrentIndex(1); /// producer
		}
	}

	/// Read and Write the currrent Well Data Model
	bool WellWidget::write(QJsonObject& wells_data)
	{			
		/// Geometry Releated Attributes
		wells_data["model_bounding_box_min_x"] = this->dim_min_.x();
		wells_data["model_bounding_box_min_y"] = this->dim_min_.y();
		wells_data["model_bounding_box_min_z"] = this->dim_min_.z();

		wells_data["model_bounding_box_max_x"] = this->dim_max_.x();
		wells_data["model_bounding_box_max_y"] = this->dim_max_.y();
		wells_data["model_bounding_box_max_z"] = this->dim_max_.z();

		wells_data["number_of_wells_"] = this->number_of_wells_;

		QJsonArray wells_attributes;

		for (int index = 0; index < this->number_of_wells_; index++)
		{
			QJsonObject attributes;

			attributes["id"] = index;
			attributes["type"] = this->wells_type_.at(index);
			attributes["sign"] = this->wells_sign_.at(index);
			attributes["pressure"] = this->wells_pressure_.at(index);
			attributes["completion_x"] = this->wells_range_.at(index).x();
			attributes["completion_y"] = this->wells_range_.at(index).y();
			attributes["position_2d_x"] = this->wells_position_in_2D_.at(index).x();
			attributes["position_2d_y"] = this->wells_position_in_2D_.at(index).y();

			wells_attributes.append(attributes);
		}

		wells_data["wells_data_array"] = wells_attributes;
		
		return false;
	}

	bool WellWidget::read(const QJsonObject& wells_data)
	{


		/// Attributes

		//// min Dim
		if (wells_data.contains("model_bounding_box_min_x") && wells_data["model_bounding_box_min_x"].isDouble())
		{
			this->dim_min_.x() = wells_data["model_bounding_box_min_x"].toDouble();
		}
		if (wells_data.contains("model_bounding_box_min_y") && wells_data["model_bounding_box_min_y"].isDouble())
		{
			this->dim_min_.y() = wells_data["model_bounding_box_min_y"].toDouble();
		}
		if (wells_data.contains("model_bounding_box_min_z") && wells_data["model_bounding_box_min_z"].isDouble())
		{
			this->dim_min_.z() = wells_data["model_bounding_box_min_z"].toDouble();
		}
		/// max Dim
		if (wells_data.contains("model_bounding_box_max_x") && wells_data["model_bounding_box_max_x"].isDouble())
		{
			this->dim_max_.x() = wells_data["model_bounding_box_max_x"].toDouble();
		}
		if (wells_data.contains("model_bounding_box_max_y") && wells_data["model_bounding_box_max_y"].isDouble())
		{
			this->dim_max_.y() = wells_data["model_bounding_box_max_y"].toDouble();
		}
		if (wells_data.contains("model_bounding_box_max_z") && wells_data["model_bounding_box_max_z"].isDouble())
		{
			this->dim_max_.z() = wells_data["model_bounding_box_max_z"].toDouble();
		}

		/// Get Number of Wells
		if (wells_data.contains("number_of_wells_") && wells_data["number_of_wells_"].isDouble())
		{
			this->number_of_wells_ = wells_data["number_of_wells_"].toDouble();
		}

		this->wells_type_.clear();
		this->wells_sign_.clear();
		this->wells_pressure_.clear();
		this->wells_range_.clear();
		this->wells_position_in_2D_.clear();

		if (wells_data.contains("wells_data_array") && wells_data["wells_data_array"].isArray())
		{
			QJsonArray attributes = wells_data["wells_data_array"].toArray();

			for (int index = 0; index < attributes.size(); ++index)
			{
				QJsonObject object = attributes[index].toObject();

				int id = object["id"].toDouble();

				this->wells_type_[id] = object["type"].toDouble();
				this->wells_sign_[id] = object["sign"].toDouble();
				this->wells_pressure_[id] = object["pressure"].toDouble();
				this->wells_range_[id].x() = object["completion_x"].toDouble();
				this->wells_range_[id].y() = object["completion_y"].toDouble();
				this->wells_position_in_2D_[id].x() = object["position_2d_x"].toDouble();
				this->wells_position_in_2D_[id].y() = object["position_2d_y"].toDouble();
			}
		}

		loadWells();

		return false;
	}


} /* namespace RRM */
