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

#include "normal_movable_crosssection_flow.h"

NormalMovableCrossSectionFlow::NormalMovableCrossSectionFlow(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    this->setLayout( hl_layout );
}

NormalMovableCrossSectionFlow::~NormalMovableCrossSectionFlow()
{
}

void NormalMovableCrossSectionFlow::on_btn_acceptnormal_accepted()
{
    float X = edt_normalCoordX->text().toFloat();
    float Y = edt_normalCoordY->text().toFloat();
    float Z = edt_normalCoordZ->text().toFloat();

    emit sendCrossSectionNormalCoordinates( X, Y, Z );

}

void NormalMovableCrossSectionFlow::on_btn_acceptnormal_rejected()
{
    edt_normalCoordX->clear();
    edt_normalCoordY->clear();
    edt_normalCoordZ->clear();

    emit canceled();

}

void NormalMovableCrossSectionFlow::clear()
{
    edt_normalCoordX->clear();
    edt_normalCoordY->clear();
    edt_normalCoordZ->clear();
}
