#ifndef POLYQTUTILS_H
#define POLYQTUTILS_H


#include <QPolygonF>

#include "Core/Geometry/PolygonalCurve/CurveN.hpp"

/// \brief collection of methods to use QT polyline library facilities
class PolyQtUtils
{
public:


        typedef qreal			Real;
        typedef PolygonalCurve<Real, 2, PointN<Real, 2>, VectorN<Real, 2> > Curve2D;
        typedef PointN<Real, 2> 					    Point2D;



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
