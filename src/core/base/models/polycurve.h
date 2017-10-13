#ifndef POLYCURVE_H
#define POLYCURVE_H

#include <vector>

class PolyCurve
{

    public:

        PolyCurve() = default;

        PolyCurve( const std::vector< double >& vs_, const std::vector< std::size_t >& es_ =
                std::vector< std::size_t >() )
        {
            setVertices( vs_ );
            setEdges( es_ );
        }


        bool isEmpty() const
        {
            return data.vertices.empty();
        }


        void setVertices( const std::vector< double >& vs_ )
        {
             data.vertices.clear();
             data.vertices.assign( vs_.begin(), vs_.end() );
        }

        void getVertices( std::vector< double >& vs_ ) const
        {
             vs_ = data.vertices;
        }


        void setEdges( const std::vector< std::size_t >& es_ )
        {
             data.edges.clear();
             data.edges.assign( es_.begin(), es_.end() );
        }

        void getEdges( std::vector< std::size_t >& es_ ) const
        {
             es_ = data.edges;
        }


        void clear()
        {
            data.vertices.clear();
            data.edges.clear();
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
