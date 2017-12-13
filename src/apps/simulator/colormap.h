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



#ifndef COLORMAP_H
#define COLORMAP_H

#include <vector>
#include <algorithm>
#include <QVector3D>
#include <limits>

class ColorMap
{

    public:


		enum class COLORMAP{ COOL_TO_WARM,JET, HOT, COOL, PARULA, SPRING, SUMMER, COPPER, POLAR, WINTER, CONSTANT, QUALITATIVE };


        ColorMap();

        QVector3D getColor( COLORMAP map, double value, double low, double high ) const;
		QVector3D getColor(const std::vector< QVector3D >& map, double value, double low, double high) const;
        QVector3D getConstantColor() const;
        std::vector< QVector3D > getColors( COLORMAP cm, unsigned int& nc ) const;

    protected:

		std::vector< QVector3D > cool_to_warm;
        std::vector< QVector3D > jet;
        std::vector< QVector3D > hot;
        std::vector< QVector3D > cool;
        std::vector< QVector3D > parula;
        std::vector< QVector3D > spring;
        std::vector< QVector3D > summer;
        std::vector< QVector3D > copper;
        std::vector< QVector3D > polar;
        std::vector< QVector3D > winter;
		/// @see http://colorbrewer2.org/#type=qualitative&scheme=Set3&n=12
		std::vector< QVector3D > qualitative;

};

#endif // COLORMAP_H
