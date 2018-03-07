#ifndef POLYCURVE_H
#define POLYCURVE_H

#include <vector>

class PolyCurve
{

    public:

        PolyCurve(){}

        PolyCurve( const std::vector< double >& vs_, const std::vector< std::size_t >& es_ =
                std::vector< std::size_t >() )
        {
            setVertices( vs_ );
            setEdges( es_ );
        }


        bool isEmpty() const
        {
            if( data.vertices.empty() == true )
                return true;
            else if( data.vertices.size() < 7 )
                return true;

            return false;
        }


        void setVertices( const std::vector< double >& vs_ )
        {
             data.vertices.clear();
             data.vertices.assign( vs_.begin(), vs_.end() );
        }

        std::vector< double > getVertices() const
        {
            return data.vertices;
        }


        void setEdges( const std::vector< std::size_t >& es_ )
        {
             data.edges.clear();
             data.edges.assign( es_.begin(), es_.end() );
        }

        std::vector< std::size_t > getEdges() const
        {
             return data.edges;
        }


        void clear()
        {
            if( data.vertices.empty() == true ) return;
            data.vertices.clear();
            data.edges.clear();
        }

        ~PolyCurve()
        {

        }

    private:



        struct PolyCurveData
        {
            std::vector< double > vertices;
            std::vector< std::size_t > edges;
        };

        PolyCurveData data;

};

#endif // POLYCURVE_H
