#include "../Arrangement2D/PolygonalCurve2.hpp"


namespace RRM
{
    /// Default Constructor;
    PolygonalCurve2::PolygonalCurve2 ( ) : Curve2D()
    {

    }


    PolygonalCurve2::PolygonalCurve2 ( const Curve2D& _curve) : Curve2D(_curve)
    {

    }

    /// XXX 2D Implementation of basic operations on Lines
    Point2D PolygonalCurve2::perpendicular ( Point2D p ) const
    {
        return Point2D ( -p[1] , p[0] );
    }

    /// \brief slices curve at vertices.
    ///                 0,1 ........... n-1,n
    /// Minimum range [first=1,last=size()-1]
    /// Creates n new curves by splitting this curve.
    /// \param indices of the split points.
    /// \param segments (output) list of curve segments.
    bool PolygonalCurve2::slices ( std::vector<std::size_t>& indices , std::deque<Self>& segments ) const
    {
        segments.clear ( );
        /// The end will be the [begin()+jump=indices[last],end()];
        segments.resize ( indices.size ( ) + 1 );
        /// Testing whether is out of range or not
        bool is_in_range = ( indices.size ( ) < this->size ( ) ) && ( indices.size ( ) < this->size ( ) );
        if ( is_in_range )
        {
            std::size_t jump = 0;

            for ( std::size_t it = 0; it < indices.size ( ); ++it )
            {
                segments[it].setPoints ( std::vector<Point2D> ( this->pPoints.begin ( ) + jump , this->pPoints.begin ( ) + ( indices[it] + 1 ) ) );
                jump = indices[it];
            }

            segments[indices.size ( )].setPoints ( std::vector<Point2D> ( this->pPoints.begin ( ) + jump , this->pPoints.end ( ) ) );
        }
        return is_in_range;
    }

    bool PolygonalCurve2::is_x_monotonic_curve ( const Curve2D&                                      _curve,
                                                  std::map<std::size_t,std::vector<std::size_t> >& _x_monotone_subcurves ) const
    {
        std::size_t current_index = 0;
        std::map<std::size_t,std::vector<std::size_t> > x_monotone_subcurves;

        bool is_left_to_right = _curve[1].x() >= _curve[0].x();
        x_monotone_subcurves[current_index].push_back(0);
        // Help in alternate between x_monotone_segments
        bool is_x_monotone_segment = true;

        // is oriented left to right
        if ( is_left_to_right )
        {
            for ( std::size_t it = 1; it < _curve.size(); it++ )
           {
                if ( !((_curve[it].x() >= _curve[it-1].x()) ^( is_x_monotone_segment))  )
                {
                    x_monotone_subcurves[current_index].push_back(it);
                }else
                {
                    is_x_monotone_segment = !is_x_monotone_segment;
                    current_index = it;
                    x_monotone_subcurves[current_index].push_back((it-1));
                    x_monotone_subcurves[current_index].push_back((it));
                }
           }
        }
        // is oriented right to left
        else
        {
            for ( std::size_t it = 1; it < _curve.size(); it++ )
           {
                if ( !((_curve[it].x() <=  _curve[it-1].x()) ^ (is_x_monotone_segment)) )
                {
                    x_monotone_subcurves[current_index].push_back ( it );
                }
                else
                {
                    is_x_monotone_segment = !is_x_monotone_segment;
                    current_index = it;
                    x_monotone_subcurves[current_index].push_back((it-1));
                    x_monotone_subcurves[current_index].push_back((it));
                }
           }
        }

        _x_monotone_subcurves = x_monotone_subcurves;

        // current_index == 0 means the curve is x_monotone
        return ( current_index == 0 );
    }


    /// XXX Follow the LEFT->RIGHT, TOP->DOWN orientation
    double PolygonalCurve2::CCW ( Point2D p1 , Point2D p2 , Point2D q ) const
    {
        return ( p2[0] - p1[0] ) * ( q[1] - p1[1] ) - ( q[0] - p1[0] ) * ( p2[1] - p1[1] );
    }


    Point2D PolygonalCurve2::projectPoint ( Point2D p1 , Point2D p2 , Point2D q , double& alpha , double& beta ) const
    {
        Point2D p2p1 = p2 - p1;
        double norm = p2p1.norm ( );
        if ( norm <= Math::Constants::Epsilon )
        {
            alpha = ( p1 - q ).norm ( );
            beta = 0.0;
            return p1;
        }

        Point2D N = perpendicular ( p2p1 ) / norm;
        alpha = ( p1 - q ).dot ( N );

        Point2D qp = q + alpha * N;

        unsigned indexMax = ( std::abs ( p2p1[0] ) < std::abs ( p2p1[1] ) ) ? 1 : 0;

        beta = ( ( qp - p1 )[indexMax] ) / p2p1[indexMax];

        return qp;
    }


    Point2D PolygonalCurve2::intersectionSegments ( Point2D p1 , Point2D p2 , Point2D q1 , Point2D q2 , double& alpha , double& beta ) const
    {
        Point2D p2p1 = p2 - p1;
        Point2D q1q2 = q1 - q2;
        Point2D q1p1 = q1 - p1;

        alpha = Math::Constants::Infinty;
        beta = Math::Constants::Infinty;

        double det = p2p1[0] * q1q2[1] - p2p1[1] * q1q2[0];
        if ( std::abs ( det ) > Math::Constants::Epsilon )
        {
            alpha = ( q1p1[0] * q1q2[1] - q1p1[1] * q1q2[0] ) / det;
            beta = ( q1p1[1] * p2p1[0] - q1p1[0] * p2p1[1] ) / det;

            Point2D pq = p1 + alpha * ( p2 - p1 );

            return pq;
        }

        /// Special cases ///

        if ( p2p1.norm2 ( ) <= Math::Constants::Epsilon && q1q2.norm2 ( ) <= Math::Constants::Epsilon )
        {
            if ( p1 == q1 || p1 == q2 )
            {
                alpha = 0.0, beta = 0.0;
                return p1;
            }
            return Point2D ( Math::Constants::Infinty , Math::Constants::Infinty );
        }

        if ( p2p1.norm2 ( ) <= Math::Constants::Epsilon )
        {
            if ( p1 == q1 )
            {
                alpha = 0.0, beta = 0.0;
                return p1;
            }
            if ( p1 == q2 )
            {
                alpha = 0.0, beta = 1.0;
                return p1;
            }
            return Point2D ( Math::Constants::Infinty , Math::Constants::Infinty );
        }

        if ( q1q2.norm2 ( ) <= Math::Constants::Epsilon )
        {
            if ( q1 == p1 )
            {
                alpha = 0.0, beta = 0.0;
                return q1;
            }
            if ( q1 == p2 )
            {
                alpha = 0.0, beta = 1.0;
                return q1;
            }
            return Point2D ( Math::Constants::Infinty , Math::Constants::Infinty );
        }

        /// if det = 0 test if the points are colinear if yes find alpha such that
        /// q1 = p1 + alpha*(p2-p1) and beta such that p1 = q1 + beta*(q2-q1)
        /// and returns pq = ( inf , inf )
        ///
        if ( std::abs ( CCW ( p1 , p2 , q1 ) ) <= Math::Constants::Epsilon )
        {
            unsigned indexMax = ( std::abs ( p2p1[0] ) < std::abs ( p2p1[1] ) ) ? 1 : 0;
            alpha = ( ( q1 - p1 )[indexMax] ) / p2p1[indexMax];

            Point2D q2q1 = q2 - q1;
            indexMax = ( std::abs ( q2q1[0] ) < std::abs ( q2q1[1] ) ) ? 1 : 0;
            beta = ( ( p1 - q1 )[indexMax] ) / q2q1[indexMax];
        }

        return Point2D ( Math::Constants::Infinty , Math::Constants::Infinty );
    }

    bool PolygonalCurve2::intersections (    const PolygonalCurve2& testCurve ,
                            std::vector<std::size_t>& thisIndex ,
                            std::vector<double>& thisAlphas ,
                            std::vector<std::size_t>& testIndex ,
                            std::vector<double>& testAlphas,
                            std::vector<Point2D> &itersPoints ) const
    {
        thisIndex.clear ( );
        thisAlphas.clear ( );
        testIndex.clear ( );
        testAlphas.clear ( );
        itersPoints.clear ( );

        PolygonalCurve2 cTestCurve = testCurve;
        if ( cTestCurve.isClosed ( ) )
            cTestCurve.add ( cTestCurve.atBegin ( ) );
        PolygonalCurve2 cThisCurve = ( *this );
        if ( cThisCurve.isClosed ( ) )
            cThisCurve.add ( cThisCurve.atBegin ( ) );

        for ( std::size_t i = 1; i < cThisCurve.size ( ); i++ )
        {
            for ( std::size_t j = 1; j < cTestCurve.size ( ); j++ )
            {
                double uThis, uTest;
                Point2D prPoint;
                /// FIXME Brute Force Procedure
                prPoint = intersectionSegments ( cThisCurve[i - 1] , cThisCurve[i] , cTestCurve[j - 1] , cTestCurve[j] , uThis , uTest );
                if ( uThis >= 0 && uThis <= 1.0 && uTest >= 0 && uTest <= 1.0 )
                {
                    thisIndex.push_back ( i - 1 );
                    testIndex.push_back ( j - 1 );
                    thisAlphas.push_back ( uThis );
                    testAlphas.push_back ( uTest );
                    itersPoints.push_back ( prPoint );
                }
            }
        }
        return thisIndex.size ( ) > 0;
    }

    void PolygonalCurve2::addPoints (std::vector<std::pair<std::size_t,Point2D> >& _intersection_points )
    {
            /// After a point is inserted on the curve, the subsequent indices are modified
            /// new_intersections_indices is used to track the modification
            std::vector<std::pair<std::size_t,Point2D> > new_intersections_indices = _intersection_points;

            for (std::size_t it = 0; it < new_intersections_indices.size(); it++)
            {
                this->insert(new_intersections_indices[it].first + it + 1, new_intersections_indices[it].second);
                _intersection_points[it].first  = new_intersections_indices[it].first + it + 1;
                _intersection_points[it].second = new_intersections_indices[it].second;
            }

    }
} /* namespace RRM */
