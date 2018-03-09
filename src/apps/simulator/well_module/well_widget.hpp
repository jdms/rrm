/*
 * WellWidget.hpp
 *
 *  Created on: Apr 16, 2017
 *      Author: felipe
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


                        /// FIXME

                        int wellType();
                        void setWellType(int i);

        };

} /* namespace RRM */

#endif /* _WELLWIDGET_HPP_ */
