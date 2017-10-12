#ifndef POLYCURVE_H
#define POLYCURVE_H


#include <vector>

#include "./libs/PolygonalCurve/CurveN.hpp"


class PolyCurve
{

    public:

        PolyCurve() = default;

        PolyCurve( const std::vector< double >& vs_, const std::vector< std::size_t >& es_ =
                std::vector< std::size_t >() );





    protected:


        std::vector< Curve2D > curve;


};

#endif // POLYCURVE_H
