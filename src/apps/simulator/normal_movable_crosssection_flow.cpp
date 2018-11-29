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
    ui = new Ui::NormalMovableCrossSectionFlow();
    ui->setupUi(this);

    this->setLayout( ui->hl_layout );
}

NormalMovableCrossSectionFlow::~NormalMovableCrossSectionFlow()
{
}

void NormalMovableCrossSectionFlow::on_btn_acceptnormal_accepted()
{
    float X = ui->edt_normalCoordX->text().toFloat();
    float Y = ui->edt_normalCoordY->text().toFloat();
    float Z = ui->edt_normalCoordZ->text().toFloat();

    emit sendCrossSectionNormalCoordinates( X, Y, Z );

}

void NormalMovableCrossSectionFlow::on_btn_acceptnormal_rejected()
{
    ui->edt_normalCoordX->clear();
    ui->edt_normalCoordY->clear();
    ui->edt_normalCoordZ->clear();

    emit canceled();

}

void NormalMovableCrossSectionFlow::clear()
{
    ui->edt_normalCoordX->clear();
    ui->edt_normalCoordY->clear();
    ui->edt_normalCoordZ->clear();
}

/* https://stackoverflow.com/questions/19866424/cmake-and-qt5-automoc-error */
/* https://stackoverflow.com/questions/16472543/errors-in-generated-moc-files-for-qt5-from-cmake/16481750#16481750 */  
/* https://cmake.org/cmake/help/v3.12/prop_tgt/AUTOMOC.html?highlight=automoc */
#include "moc_normal_movable_crosssection_flow.cpp"
