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



#ifndef _WELLWIDGET_HPP_
#define _WELLWIDGET_HPP_

#include <QtWidgets/QWidget>
#include <Eigen/Dense>


#include "ui_well_widget_form.h"

#include "../widgets/spanslider/qxtspanslider.h"

namespace RRM
{

        class WellWidget : public QWidget
        {
                 Q_OBJECT

                public:

                         WellWidget ( QWidget * parent );
                        virtual ~WellWidget ( ) = default;

                        std::map< int, int >			   getWellsType() const;
                        std::map< int, int >               getWellsSign() const;
                        std::map< int, double >            getWellsPressure() const;
                        std::map< int, Eigen::Vector4d >   getWellsPosition() const;
                        std::map< int, Eigen::Vector2d >   getWellsRange() const;

                        int									getNumberOfWells() const;

                        void getWellData(int& _number_of_well, std::vector< unsigned int >& _types, std::vector< double >& _pressure, std::vector< int >& _sign, std::vector<Eigen::Vector4d >& _positions, std::vector<Eigen::Vector2d>& _range);
                        void setBoundingBoxDimension(const Eigen::Vector3d& _box_min, const Eigen::Vector3d& _box_max);

                public slots:
                        void updateWellPosition(const std::map< int, Eigen::Vector4d >& _positions);
                        void clear(){};
						void reset(){};
                signals:
                        void numberOfWells(int _number_of_wells);
                        void wellPositionsHasChanged();
                private:
                        // Designer form
                        Ui::WellWidgetForm * ui_;
                        QxtSpanSlider*        qxt_span_slider_;

                        // Create  the default widget stage
                        void setupWidget();
                        void createConnections();
                        void updateWellsWidget(int _index);
                        void createWells( int _number_of_well);

                        int								 number_of_wells_;

                        std::map< int, int >			 wells_type_;
                        std::map< int, int >             wells_sign_;
                        std::map< int, double >          wells_pressure_;
                        std::map< int, Eigen::Vector4d > wells_position_;
                        std::map< int, Eigen::Vector2d > wells_range_;

                        Eigen::Vector3d dim_max_;
                        Eigen::Vector3d dim_min_;

						/// FIXME January 2018

						double default_injector_pressure_;
						double default_producer_pressure_;
						

                        /// FIXME

                        int wellType();
                        void setWellType(int i);

        };

} /* namespace RRM */

#endif /* _WELLWIDGET_HPP_ */
