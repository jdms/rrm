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
                this->ui_->label_Region_Permeability->setVisible(false);
                this->ui_->doubleSpinBox_Region_Permeability_->setVisible(false);
                this->ui_->horizontalSlider_Permeability_->setVisible(false);
                this->ui_->label_Region_Porosity_->setVisible(false);
                this->ui_->doubleSpinBox_Region_Porosity_->setVisible(false);
                this->ui_->horizontalSlider_Porosity_->setVisible(false);

                this->ui_->label_Region_Permeability_Curves_->setVisible(false);
                this->ui_->comboBox_Region_Permeability_Curves_->setVisible(false);

                /// @FIXME September 2017
                this->doubleSpinbBox_low_permeability_->setSuffix(" (mD)");
                this->doubleSpinbBox_high_permeability_->setSuffix(" (mD)");
                this->doubleSpinbBox_low_porosity_->setSuffix(" (--)");
                this->doubleSpinbBox_high_porosity_->setSuffix(" (--)");

                this->ui_->doubleSpinBox_Region_Saturation_->setSuffix(" (--)");

                ///@September
                emit this->ui_->radioButton_Linear_->toggled(true);

        }

        void RegionWidget::setupWidget()
        {
            this->number_of_regions_ = 0;
            this->ui_->label_Region_Permeability_Curves_->setVisible(true);
            this->ui_->comboBox_Region_Permeability_Curves_->setVisible(true);


            /// @FIXME September
            slider_porosity_ = new QSlider(Qt::Orientation::Horizontal);
            slider_porosity_->setVisible(false);
            qxt_span_slider_porosity_ = new QxtSpanSlider(Qt::Orientation::Horizontal);
            qxt_span_slider_porosity_->setHandleMovementMode(QxtSpanSlider::HandleMovementMode::NoCrossing);

            gradient_porosity_Label_ = new QLabel(tr("Porosity"));
            doubleSpinbBox_low_porosity_ = new QDoubleSpinBox();
            doubleSpinbBox_high_porosity_ = new QDoubleSpinBox();

            this->ui_->gridLayout_Region_Attributes_->addWidget(gradient_porosity_Label_, 4, 0);
            this->ui_->gridLayout_Region_Attributes_->addWidget(doubleSpinbBox_low_porosity_, 4, 1);
            this->ui_->gridLayout_Region_Attributes_->addWidget(qxt_span_slider_porosity_, 4, 2, 1, 5);
            this->ui_->gridLayout_Region_Attributes_->addWidget(doubleSpinbBox_high_porosity_, 4, 8);
            this->porosity_position_ = std::make_tuple<int, int, int, int>(4, 2, 1, 5);

            /// @FIXME September
            slider_permeability_ = new QSlider(Qt::Orientation::Horizontal);
            slider_permeability_->setVisible(false);
            qxt_span_slider_permeability_ = new QxtSpanSlider(Qt::Orientation::Horizontal);
            qxt_span_slider_permeability_->setHandleMovementMode(QxtSpanSlider::HandleMovementMode::NoCrossing);

            gradient_permeability_Label_ = new QLabel(tr("Permeability"));
            doubleSpinbBox_low_permeability_ = new QDoubleSpinBox();
            doubleSpinbBox_high_permeability_ = new QDoubleSpinBox();

            this->ui_->gridLayout_Region_Attributes_->addWidget(gradient_permeability_Label_, 5, 0);
            this->ui_->gridLayout_Region_Attributes_->addWidget(doubleSpinbBox_low_permeability_, 5, 1);
            this->ui_->gridLayout_Region_Attributes_->addWidget(qxt_span_slider_permeability_, 5, 2, 1, 5);
            this->ui_->gridLayout_Region_Attributes_->addWidget(doubleSpinbBox_high_permeability_, 5, 8);
            this->permeability_position_ = std::make_tuple<int, int, int, int>(5, 2, 1, 5);

            doubleSpinbBox_low_permeability_->setMinimum(0.001);
            doubleSpinbBox_low_permeability_->setMaximum(10000.0);
            doubleSpinbBox_low_permeability_->setDecimals(3);
            doubleSpinbBox_high_permeability_->setMinimum(0.001);
            doubleSpinbBox_high_permeability_->setMaximum(10000.0);
            doubleSpinbBox_high_permeability_->setDecimals(3);

            qxt_span_slider_permeability_->setMinimum(-1);
            qxt_span_slider_permeability_->setMaximum(102);
            slider_permeability_->setMinimum(-1);
            slider_permeability_->setMaximum(102);



            doubleSpinbBox_low_porosity_->setMinimum(0.01);
            doubleSpinbBox_low_porosity_->setMaximum(0.35);
            doubleSpinbBox_low_porosity_->setDecimals(2);
            doubleSpinbBox_high_porosity_->setMinimum(0.01);
            doubleSpinbBox_high_porosity_->setMaximum(0.35);
            doubleSpinbBox_high_porosity_->setDecimals(2);

            qxt_span_slider_porosity_->setMinimum(-1);
            qxt_span_slider_porosity_->setMaximum(101);
            slider_porosity_->setMinimum(-1);
            slider_porosity_->setMaximum(101);
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

                _porosity_gratdients[it]	 = this->porosity_gradient_values_.at(it);
                _permeability_gratdients[it] = this->permeability_gradient_values_.at(it);
            }
        }

        void RegionWidget::setRegionData(const int _number_of_regions,
            const std::vector<double>& _positions,
            const std::vector<double>& _permeability_values,
            const std::vector<double>& _porosity_values,
            const std::vector<double >& _staturation_values
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

                connect( ui_->pushButton, &QPushButton::clicked, this, [=](){ emit getRegions(); } );

                //@see http://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
                /// 0 to 10101
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

                /// Porosity
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


                /// Saturation
                connect(ui_->horizontalSlider_Saturation_, static_cast<void (QSlider::*)(int)>(&QSlider::sliderMoved), this, [=]()
                {

                        double ex = (0.01)*static_cast<double>(ui_->horizontalSlider_Saturation_->value());
                        ui_->doubleSpinBox_Region_Saturation_->setValue(ex);
                    //	/// FIXME Viscosity have to be the same for all model for now, util a better
                        /// interface if design.
                        saturation_values[ui_->comboBox_Region_->currentIndex()] = ex;

                });

                ///// FIXME July 14
                connect(ui_->doubleSpinBox_Region_Saturation_, &QDoubleSpinBox::editingFinished, this, [=]()
                {

                    double ex = ui_->doubleSpinBox_Region_Saturation_->value();

                    int i = static_cast<int>((ex) / 0.01);
                    /// FIXME Viscosity have to be the same for all model for now, util a better
                    /// interface if design.

                    ui_->horizontalSlider_Saturation_->setValue(i);

                    saturation_values[ui_->comboBox_Region_->currentIndex()] = ex;

                });


                //// @FIXME September Porosity

                connect(qxt_span_slider_porosity_, &QxtSpanSlider::lowerPositionChanged, this, [=]()
                {
                    double ex = 0.01 + (0.0034)*static_cast<double>(qxt_span_slider_porosity_->lowerValue());
                    doubleSpinbBox_low_porosity_->setValue(ex);

                    porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = ex;
                });

                    connect(doubleSpinbBox_low_porosity_, &QDoubleSpinBox::editingFinished, this, [=]()
                    {
                        double ex = doubleSpinbBox_low_porosity_->value();
                        int i = static_cast<int>((ex - 0.01) / 0.0034);
                        qxt_span_slider_porosity_->setLowerPosition(i);

                        ///@September
                        slider_porosity_->setValue(i);

                        porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = doubleSpinbBox_low_porosity_->value();
                    });

                    connect(qxt_span_slider_porosity_, &QxtSpanSlider::upperPositionChanged, this, [=]()
                {
                    double ex = 0.01 + (0.0034)*static_cast<double>(qxt_span_slider_porosity_->upperValue());
                    doubleSpinbBox_high_porosity_->setValue(ex);
                    porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = ex;
                });

                    connect(doubleSpinbBox_high_porosity_, &QDoubleSpinBox::editingFinished, this, [=]()
                    {
                        double ex = doubleSpinbBox_high_porosity_->value();
                        int i = static_cast<int>((ex - 0.01) / 0.0034);
                        qxt_span_slider_porosity_->setUpperPosition(i);
                        porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = doubleSpinbBox_high_porosity_->value();
                    });

                //// @FIXME September Permeability
                    connect(qxt_span_slider_permeability_, &QxtSpanSlider::lowerPositionChanged, this, [=]()
                {
                    double ex = -3.0 + (0.07)*static_cast<double>(qxt_span_slider_permeability_->lowerValue());
                    ex = std::pow(10, ex);
                    std::cout << "Step " << qxt_span_slider_permeability_->lowerValue() << std::endl;

                    doubleSpinbBox_low_permeability_->setValue(ex);
                    permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = ex;
                });

                    connect(doubleSpinbBox_low_permeability_, &QDoubleSpinBox::editingFinished, this, [=]()
                    {
                        double p = doubleSpinbBox_low_permeability_->value();
                        double ex = std::log10(p);
                        int i = static_cast<int>((ex + 3.0) / 0.07);

                        qxt_span_slider_permeability_->setLowerPosition(i);
                        ///@September
                        slider_permeability_->setValue(i);

                        permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = doubleSpinbBox_low_permeability_->value();
                    });

                connect(qxt_span_slider_permeability_, &QxtSpanSlider::upperPositionChanged, this, [=]()
                {
                    double ex = -3.0 + (0.07)*static_cast<double>(qxt_span_slider_permeability_->upperValue());
                    ex= std::pow(10, ex);
                    std::cout << "Step upper" << qxt_span_slider_permeability_->upperValue() << std::endl;

                    doubleSpinbBox_high_permeability_->setValue(ex);
                    permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = ex;
                });

                    connect(doubleSpinbBox_high_permeability_, &QDoubleSpinBox::editingFinished, this, [=]()
                    {
                        double p = doubleSpinbBox_high_permeability_->value();
                        double ex = std::log10(p);
                        int i = static_cast<int>((ex + 3.0) / 0.07);

                        qxt_span_slider_permeability_->setUpperPosition(i);
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
                connect(slider_permeability_, static_cast<void (QSlider::*)(int)>(&QSlider::sliderMoved), this, [=]()
                {
                    double ex = -3.0 + (0.07)*static_cast<double>(slider_permeability_->value());
                    ex = std::pow(10, ex);

                    doubleSpinbBox_low_permeability_->setValue(ex);
                    permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = ex;
                    permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = ex;
                });

                //// @FIXME September Permeability
                connect(slider_porosity_, static_cast<void (QSlider::*)(int)>(&QSlider::sliderMoved), this, [=]()
                {
                    double ex = 0.01 + (0.0034)*static_cast<double>(slider_porosity_->value());
                    doubleSpinbBox_low_porosity_->setValue(ex);

                    porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].first = ex;
                    porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].second = ex;
                });


        }

        /// @FIXME There is a region ZERO that is represented as 1 ....
        /// Update text box with the current information
        void RegionWidget::updateRegionsWidget(int _index)
        {
            ui_->doubleSpinBox_Region_Permeability_->setValue(permeability_values[_index]);
            ui_->horizontalSlider_Permeability_->setValue(permeability_slider_values[_index]);

            ui_->doubleSpinBox_Region_Porosity_->setValue(porosity_values[_index]);
            ui_->horizontalSlider_Porosity_->setValue(porosity_slider_values[_index]);

            /// @FIXME Me September
            doubleSpinbBox_low_porosity_->setValue(porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].first);
            emit doubleSpinbBox_low_porosity_->editingFinished();
            doubleSpinbBox_high_porosity_->setValue(porosity_gradient_values_[ui_->comboBox_Region_->currentIndex()].second);
            emit doubleSpinbBox_high_porosity_->editingFinished();

            doubleSpinbBox_low_permeability_->setValue(permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].first);
            emit doubleSpinbBox_low_permeability_->editingFinished();
            doubleSpinbBox_high_permeability_->setValue(permeability_gradient_values_[ui_->comboBox_Region_->currentIndex()].second);
            emit doubleSpinbBox_high_permeability_->editingFinished();


            ui_->doubleSpinBox_Region_Saturation_->setValue(saturation_values[ui_->comboBox_Region_->currentIndex()]);
            emit ui_->doubleSpinBox_Region_Saturation_->editingFinished();

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

                        permeability_values[i] = ui_->doubleSpinBox_Region_Permeability_->value();
                        permeability_slider_values[i] = 75;
                        porosity_values[i] = ui_->doubleSpinBox_Region_Porosity_->value();
                        porosity_slider_values[i] = 88;


                        positions_values[i].x() = 0.0;
                        positions_values[i].y() = 0.0;
                        positions_values[i].z() = 0.0;

                        /// @FIXME Me September
                        doubleSpinbBox_low_porosity_->setValue(0.28);
                        emit doubleSpinbBox_low_porosity_->editingFinished();
                        doubleSpinbBox_high_porosity_->setValue(0.32);
                        emit doubleSpinbBox_high_porosity_->editingFinished();

                        porosity_gradient_values_[i].first = 0.28;
                        porosity_gradient_values_[i].second = 0.32;

                        doubleSpinbBox_low_permeability_->setValue(150.0);
                        emit doubleSpinbBox_low_permeability_->editingFinished();
                        doubleSpinbBox_high_permeability_->setValue(250.0);
                        emit doubleSpinbBox_high_permeability_->editingFinished();

                        permeability_gradient_values_[i].first = 150.0;
                        permeability_gradient_values_[i].second = 250.0;

                        saturation_values[i] = 0.0;
                        ui_->doubleSpinBox_Region_Saturation_->setValue(0.0);
                        emit ui_->doubleSpinBox_Region_Saturation_->editingFinished();

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

} /* namespace RRM */
