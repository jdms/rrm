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


/*
 * PolygonalCurve2D.hpp
 *
 *  Created on: Aug 23, 2015
 *      Author: felipe, Emilio Vital Brazil
 */

#ifndef _RRM_POLYGONALCURVE2D_HPP_
#define _RRM_POLYGONALCURVE2D_HPP_

#include <map>
#include <vector>
#include <algorithm>
#include <tuple>

#include "./src/Core/Base/Constants/Constants.hpp"
#include "PolygonalCurve/CurveN.hpp"


/*!
 * @brief  Wrapper to class PolygonalCurve, which represents curves built from the  concatenation of straight lines.
 */



typedef double                                                           Real;
typedef PolygonalCurve< Real, 2, PointN< Real, 2 >, VectorN< Real, 2 > > Curve2D;
typedef PointN< Real, 2 >                                                Point2D;



#endif /* _RRM_POLYGONALCURVE2D_HPP_ */
