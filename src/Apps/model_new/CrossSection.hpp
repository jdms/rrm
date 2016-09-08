#ifndef CROSSSECTION_HPP
#define CROSSSECTION_HPP

#include <vector>
#include <map>

#include "model_new/Boundary.hpp"
#include "model_new/Stratigraphy.hpp"
#include "model_new/Intersection.hpp"


class CrossSection
{

public:

    CrossSection(){ boundary = nullptr; }

    inline bool isEmpty(){ return stratigraphy_list.empty(); }
    inline bool hasBoundary(){ if( boundary == nullptr ) return false; return true; }


    inline void setBoundary( Boundary* const& bd ){ boundary = bd; }
    inline Boundary* getBoundary(){ return boundary; }


    inline void setDepth( const float& d ){ depth = d; }
    inline float getDepth(){ return depth; }


    inline void addStratigraphy( Stratigraphy* strat )
    {
        Stratigraphy::Curve2D c = strat->getCurve( depth );
        curves.push_back( c );

//        stratigraphies[ c ] = strat;
    }


    inline Stratigraphy* getStratigraphy( unsigned int id ){ return stratigraphy_list[ id ]; }


    inline void getIntersections( Stratigraphy* strat )
    {
        Stratigraphy::Curve2D c = strat->getCurve( depth );

//        intersections.getIntersections( c, );
    }




protected:

    Boundary* boundary;
    std::vector< Stratigraphy* > stratigraphy_list;



    Intersection intersections;
    std::vector< Stratigraphy::Curve2D > curves;
    std::map< Stratigraphy::Curve2D, Stratigraphy* > stratigraphies;



    float depth;
};


#endif
