#include "region.h"

Region::Region()
{
    initialize();
}

void Region::setIndex( const std::size_t id_ )
{
    index = id_;
}

std::size_t Region::getIndex() const
{
    return index;
}


void Region::setPoint( double x_, double y_, double z_ )
{
    center.x = x_;
    center.y = y_;
    center.z = z_;
}

void Region::getPoint( double& x_, double& y_, double& z_ ) const
{
    x_ = center.x;
    y_ = center.y;
    z_ = center.z;
}

void Region::setVisible( const bool status_ )
{
    is_visible = status_;
}

bool Region::isVisible() const
{
    return is_visible;
}

void Region::updateCells( const std::vector< std::size_t >& cells_ )
{
    clearCells();
    index_cells.assign( cells_.begin(), cells_.end() );
}

void Region::getCells( std::vector< std::size_t >& cells_ ) const
{
    cells_.clear();
    cells_.assign( index_cells.begin(), index_cells.end() );
}


void Region::clearCells()
{
    index_cells.clear();
}


void Region::clear()
{
    clearCells();
    initialize();
}

void Region::initialize()
{
    index = 0;

    center.x = 0.0;
    center.y = 0.0;
    center.z = 0.0;

    is_visible = true;
}
