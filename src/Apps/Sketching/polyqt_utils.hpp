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



#ifndef POLYQTUTILS_H
#define POLYQTUTILS_H


#include <QPolygonF>

#include "Core/Geometry/PolygonalCurve/polygonal_curve_2d.hpp"

/// \brief collection of methods to use QT polyline library facilities
class PolyQtUtils
{
public:


    /// \brief a simple facade for containsPoint of Qt Polygon
    static bool containsPoint(const Curve2D& , const Point2D& ,
                              Qt::FillRule = Qt::OddEvenFill );

    /// \brief returns a vetor with true in i if the Point2D i is inside of the
    ///        Curve2D. This method only converts the Curve2D to QPolygonF once.
    static std::vector<bool> containsPoints(const Curve2D& ,
                                           const std::vector<Point2D> & ,
                                           Qt::FillRule = Qt::OddEvenFill );

    /// \brief converts a curve2D to a QPolygonF (Linear cost)
    static QPolygonF curve2DToQPolyginF( const Curve2D&  ) ;

    /// \brief converts a QPolygonF to a Curve2D (Linear cost)
    static Curve2D qPolyginFToCurve2D( const QPolygonF&  ) ;

    /// \brief converts a Point2D to QPointF
    static QPointF Point2DToQPointF( const Point2D& );

    /// \brief converts a QPointF to Point2D
    static Point2D QPointFToPoint2D(const QPointF &p) ;

};

#endif // INTERFACEQT2DOBJECTS_H
