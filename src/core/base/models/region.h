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

        void updateCells( const std::vector< std::size_t >& cells_ );
        void getCells( std::vector< std::size_t >& cells_ ) const;
        void clearCells();

        void clear();
        void initialize();



    private:

        struct  Point { double x, y, z; };

        std::size_t index;
        Point center;

        bool is_visible;
        std::vector< std::size_t > index_cells;

};

#endif // REGION_H
