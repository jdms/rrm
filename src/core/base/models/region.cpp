#include "region.h"

Region::Region()
{
    initialize();
}

void Region::setIndex( const std::size_t id_ )
{
    index = id_;
    name = "Region " + std::to_string( index );
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


void Region::setName( const std::string& name_ )
{
    name.clear();
    name = name_;
}


std::string Region::getName() const
{
    return name;
}


void Region::setVisible( const bool status_ )
{
    is_visible = status_;
}

bool Region::isVisible() const
{
    return is_visible;
}


void Region::setTetrahedralCells( const std::vector< std::size_t >& faces_ )
{

    clearCells();

    std::size_t number_of_tetrahedrals = faces_.size()/4;
    std::cout << "Number of tetrahedrals: " << number_of_tetrahedrals << std::endl
              << std::flush;


    for( std::size_t i = 0; i < number_of_tetrahedrals; ++i )
    {
        std::size_t v0 = faces_[ 4*i + 0 ];
        std::size_t v1 = faces_[ 4*i + 1 ];
        std::size_t v2 = faces_[ 4*i + 2 ];
        std::size_t v3 = faces_[ 4*i + 3 ];

        index_cells.push_back( v0 );
        index_cells.push_back( v1 );
        index_cells.push_back( v2 );

        index_cells.push_back( v0 );
        index_cells.push_back( v2 );
        index_cells.push_back( v3 );

        index_cells.push_back( v0 );
        index_cells.push_back( v3 );
        index_cells.push_back( v1 );

        index_cells.push_back( v1 );
        index_cells.push_back( v3 );
        index_cells.push_back( v2 );

    }

}


void Region::getTetrahedralCells( std::vector< std::size_t >& cells_ ) const
{
    cells_.clear();
    cells_.assign( index_cells.begin(), index_cells.end() );
}


void Region::clearCells()
{
    index_cells.clear();
}


void Region::setVertices( const std::vector< double >& vertices_  )
{
    vertices.clear();
    vertices.assign( vertices_.begin(), vertices_.end() );
}


void Region::getVertices( std::vector< double >& vertices_  ) const
{
    vertices_.clear();
    vertices_.assign( vertices.begin(), vertices.end() );
}


void Region::clearVertices()
{
    vertices.clear();
}


void Region::setMaxMin( double maxx_, double maxy_, double maxz_,
                        double minx_, double miny_, double minz_ )
{
    max.x = maxx_;
    max.y = maxy_;
    max.z = maxz_;

    min.x = minx_;
    min.y = miny_;
    min.z = minz_;


}


void Region::getMaxMin( double& maxx_, double& maxy_, double& maxz_,
                        double& minx_, double& miny_, double& minz_ ) const
{

    maxx_ = max.x;
    maxy_ = max.y;
    maxz_ = max.z;

    minx_ = min.x;
    miny_ = min.y;
    minz_ = min.z;

}


void Region::setColor( int r_, int g_, int b_ )
{
    color.r = r_;
    color.g = g_;
    color.b = b_;
}


void Region::getColor( int& r_, int& g_, int& b_ ) const
{
    r_ = color.r;
    g_ = color.g;
    b_ = color.b;
}


void Region::clear()
{
    clearCells();
    clearVertices();
    initialize();
}

void Region::initialize()
{
    index = 0;

    center.x = 0.0;
    center.y = 0.0;
    center.z = 0.0;

    is_visible = true;

    color.r = 255;
    color.g = 0;
    color.b = 0;
}
