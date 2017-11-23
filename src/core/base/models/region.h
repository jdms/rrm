#ifndef REGION_H
#define REGION_H

#include <iostream>
#include <string>
#include <vector>


class Region
{
    public:

        Region();

        void setIndex( const std::size_t id_ );
        std::size_t getIndex() const;

        void setPoint( double x_, double y_, double z_ );
        void getPoint( double& x_, double& y_, double& z_ ) const;

        void setVisible( const bool status_ );
        bool isVisible() const;

        void setTetrahedralCells( const std::vector< std::size_t >& cells_ );
        void getTetrahedralCells( std::vector< std::size_t >& cells_ ) const;
        void clearCells();

        void setVertices( const std::vector< double >& vertices_  );
        void getVertices( std::vector< double >& vertices_  ) const;
        void clearVertices();


        void setColor( int r_, int g_, int b_ );
        void getColor( int& r_, int& g_, int& b_ ) const;


        void clear();
        void initialize();



    private:

        struct  Point { double x, y, z; };

        struct Color
        {
          int r = 255;
          int g = 0;
          int b = 0;
        };


        std::size_t index;
        Point center;

        bool is_visible;
        std::vector< std::size_t > index_cells;
        std::vector< double > vertices;
        Color color;
};

#endif // REGION_H
