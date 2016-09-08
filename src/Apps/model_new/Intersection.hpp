#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <vector>
#include <map>



#include "Core/Geometry/PolygonalCurve/CurveN.hpp"


class Intersection
{

public:


    typedef qreal			Real;
    typedef PolygonalCurve<Real, 2, PointN<Real, 2>, VectorN<Real, 2> > Curve2D;
    typedef PointN<Real, 2> 					    Point2D;


    Intersection(){}

    inline void addIntersection( Curve2D* c1, Curve2D* c2, const std::vector< Point2D > points )
    {
        ResultIntersection res_c1;
        res_c1.second = c2;
        res_c1.intersection_points = points;

        std::vector< ResultIntersection > resultsc1_list;
        resultsc1_list.push_back( res_c1 );

        intersections[ c1 ] = resultsc1_list;



        ResultIntersection res_c2;
        res_c2.second = c1;
        res_c2.intersection_points = points;

        std::vector< ResultIntersection > resultsc2_list;
        resultsc2_list.push_back( res_c2 );

        intersections[ c2 ] = resultsc2_list;

    }

    inline void getIntersections( Curve2D* c, std::vector< Curve2D* >& curves, std::vector< std::vector < Point2D > >& points )
    {
        std::vector< ResultIntersection > res = intersections[ c ];

        for( int i = 0; i < res.size(); ++i )
        {
            curves.push_back( res[ i ].second );
            points.push_back( res[ i ].intersection_points );
        }

    }




protected:


    struct ResultIntersection
    {
        Curve2D* second;
        std::vector< Point2D > intersection_points;
    };


    std::map< Curve2D*,  std::vector< ResultIntersection > > intersections;



};


#endif




/*


#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <pair>
#include <vector>




class Intersection
{

        public:

                Intersection( Curve2D* c1, Curve2D* c2 ){ entities = std::make_pair ( c1, c2 ); }

                inline void setEntities( Curve2D* c1, Curve2D* c2 ){ entities.first = c1; entities.second = c2; }

                void setFirstCurve( Curve2D* c1 ){ entities.first = c1; }
                Curve2D* getFirstCurve(){ return entities.first; }

                void setSecondCurve( Curve2D* c2 ){ entities.second = c2; }
                Curve2D* getSecondCurve(){ return entities.second; }


                void setIntersectionPoints( const std::vector< Point2D >& points ){ intersection_points =  points; }
                std::vector< Point2D > getIntersectionPoints(){ return intersection_points; }


        protected:


                struct ResultIntersection
                {
                        Curve2D* second;
                        std::vector< Point2D > intersection_points;
                };


                std::map< Curve2D*,  std::vector< ResultIntersection > > intersections;


                //std::pair< Curve2D*, Curve2D* > entities;
                //std::vector< Point2D > intersection_points;

};


#endif

*/
