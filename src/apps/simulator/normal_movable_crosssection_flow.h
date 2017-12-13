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


#ifndef NORMALMOVABLECROSSSECTIONFLOW_H
#define NORMALMOVABLECROSSSECTIONFLOW_H

#include <QWidget>

#include "ui_normal_movable_crosssection_flow.h"



class NormalMovableCrossSectionFlow : public QWidget,
                                      private Ui::NormalMovableCrossSectionFlow
{
    Q_OBJECT

    public:

        explicit NormalMovableCrossSectionFlow(QWidget *parent = 0);
        ~NormalMovableCrossSectionFlow();

        void clear();

    private slots:

        void on_btn_acceptnormal_accepted();
        void on_btn_acceptnormal_rejected();

    signals:

        void sendCrossSectionNormalCoordinates( float x, float y, float z );
        void canceled();



};

#endif // NORMALMOVABLECROSSSECTIONFLOW_H
