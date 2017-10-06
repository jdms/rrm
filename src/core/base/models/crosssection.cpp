#include "crosssection.h"
#include "volume.h"

CrossSection::CrossSection()
{
    defineIndex();
    initialize();
}


CrossSection::CrossSection( Volume* const& raw_, const CrossSection::Direction& dir_, const double depth_ )
{
    defineIndex();
    initialize();

    setVolume( raw_ );
    setDirection( dir_ );
    setDepth( depth_ );
}



void CrossSection::defineIndex()
{
    index = number_of_csections;
    number_of_csections++;
}

void CrossSection::setIndex( const std::size_t id_ )
{
    index = id_;
    number_of_csections = id_ + 1;
}

std::size_t CrossSection::getIndex() const
{
    return index;
}


void CrossSection::setVolume( Volume* const& raw_ )
{
    volume = raw_;
}

Volume* CrossSection::getVolume() const
{
    return volume;
}


void CrossSection::getCoordinates( std::vector< double >& vertices_ ) const
{
    if( direction == Direction::Z )
    {
        volume->getFrontFace( vertices_ );
        vertices_[ 2 ]  = depth;
        vertices_[ 5 ]  = depth;
        vertices_[ 8 ]  = depth;
        vertices_[ 11 ] = depth;
    }
    else if( direction == Direction::Y )
    {
        volume->getBottomFace( vertices_ );
        vertices_[ 1 ] = depth;
        vertices_[ 4 ] = depth;
        vertices_[ 7 ] = depth;
        vertices_[ 10 ] = depth;
    }
    else if( direction == Direction::X )
    {
        volume->getRightFace( vertices_ );
        vertices_[ 0 ] = depth;
        vertices_[ 3 ] = depth;
        vertices_[ 6 ] = depth;
        vertices_[ 9 ] = depth;
    }
}


void CrossSection::getMaxMin( double& maxx_, double& maxy_, double& maxz_,
                              double& minx_, double& miny_, double& minz_ ) const
{
    double w = 0, h = 0,  l = 0;

    volume->getOrigin( minx_, miny_, minz_ );
    volume->getGeometry( w, h, l );

    maxx_ = minx_ + w;
    maxy_ = miny_ + h;
    maxz_ = minz_ + l;

}


void CrossSection::setDirection( const CrossSection::Direction& dir_ )
{
    direction = dir_;
}

void CrossSection::getDirection( CrossSection::Direction& dir_ ) const
{
    dir_ = direction;
}

void CrossSection::setDepth( double depth_ )
{
    depth = depth_;
}

double CrossSection::getDepth() const
{
    return depth;
}

void CrossSection::setImagePath( const std::string& path_ )
{
    image_path = path_;
}

std::string CrossSection::getImagePath() const
{
    return image_path;
}

void CrossSection::setVisible( const bool status_ )
{
    is_visible = status_;
}

bool CrossSection::isVisible() const
{
    return is_visible;
}


void CrossSection::addObject( const Object& id_, Curve2D* const& curve_ )
{
    objects[ id_ ] = curve_;
}

CrossSection::Curve2D* const& CrossSection::getObjectCurve( const Object& id_ ) const
{
    //TODO: check if id is valid
    return objects[ id_ ];

}


void CrossSection::removeObjectCurve( const Object& id_ )
{
    //TODO: check if id is valid
    //TODO: check clear curve2d
    objects.erase( id_ );

}


void CrossSection::removeObjects()
{
    for( auto it: objects )
    {
//        removeObjectCurve( it->first );
    }
    objects.clear();
}


void CrossSection::clear()
{
    image_path.clear();
    removeObjects();
    initialize();
}

void CrossSection::initialize()
{
    depth = 0.0;
    direction = Direction::Z;
    is_visible = true;
    volume = nullptr;
}
