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

#include "Core/Base/Constants.hpp"
#include "PolygonalCurve/CurveN.hpp"


/*!
 * @brief  Wrapper to class PolygonalCurve, which represents curves built from the  concatenation of straight lines.
 */



typedef double                                                           Real;
typedef PolygonalCurve< Real, 2, PointN< Real, 2 >, VectorN< Real, 2 > > Curve2D;
typedef PointN< Real, 2 >                                                Point2D;



#endif /* _RRM_POLYGONALCURVE2D_HPP_ */
