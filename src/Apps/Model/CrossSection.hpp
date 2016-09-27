#ifndef CROSSSECTION_HPP
#define CROSSSECTION_HPP

#include <vector>
#include <map>

#include "Model/Boundary.hpp"
#include "Model/Stratigraphy.hpp"
#include "Model/Intersection.hpp"


class CrossSection
{

    public:

        CrossSection(){ boundary = nullptr; }

        inline bool isEmpty(){ return curves.empty(); }
        inline bool hasBoundary(){ if( boundary == nullptr ) return false; return true; }


        inline void setBoundary( Boundary* const& bd ){ boundary = bd; }
        inline Boundary* getBoundary(){ return boundary; }


        inline void setDepth( const float& d ){ depth = d; }
        inline float getDepth(){ return depth; }


        inline void addStratigraphy( Stratigraphy* const& strat )
        {
            Curve2D* c = strat->getCurve( depth );
            curves.push_back( c );

            stratigraphies[ c ] = strat;
        }


        inline Stratigraphy* getStratigraphy( const unsigned int& id )
        {
            Curve2D* c = curves[ id ];
            return stratigraphies[ c ];
        }

        inline void getIntersections( Stratigraphy* const& strat, std::vector< Curve2D* >& sec_curves, std::vector< std::vector < Point2D > >& points )
        {
            Curve2D* c = strat->getCurve( depth );
            intersections.getIntersections( c, sec_curves, points );
        }




    protected:


        float depth;
        Boundary* boundary;

        Intersection intersections;
        std::vector< Curve2D* > curves;
        std::map< Curve2D*, Stratigraphy* > stratigraphies;




};


#endif
