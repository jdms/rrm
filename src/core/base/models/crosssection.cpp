#include "crosssection.h"

#include "volume.h"
#include "polycurve.h"


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


CrossSection& CrossSection::operator=( const CrossSection& other_ )
{
    volume = other_.volume;
    objects = other_.objects;
    direction = other_.direction;
    depth = other_.depth;
    image_path = other_.image_path;
    is_visible = other_.is_visible;
    index = other_.index;

    return *this;
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


void CrossSection::getCoordinates( std::vector< double >& vertices_ )
{


    double maxx , maxy, maxz;
    double minx , miny, minz;

    getMaxMin( maxx, maxy, maxz, minx, miny, minz );

    if( direction == Direction::Z )
    {
        if( depth > maxz )
            depth = maxz;
        else if( depth < minz )
            depth = minz;

        volume->getFrontFace( vertices_ );
        vertices_[ 2 ]  = depth;
        vertices_[ 5 ]  = depth;
        vertices_[ 8 ]  = depth;
        vertices_[ 11 ] = depth;
    }
    else if( direction == Direction::Y )
    {

        if( depth > maxy )
            depth = maxy;
        else if( depth < miny )
            depth = miny;


        volume->getBottomFace( vertices_ );
        vertices_[ 1 ] = depth;
        vertices_[ 4 ] = depth;
        vertices_[ 7 ] = depth;
        vertices_[ 10 ] = depth;
    }
    else if( direction == Direction::X )
    {

        if( depth > maxx )
            depth = maxx;
        else if( depth < minx )
            depth = minx;


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

CrossSection::Direction CrossSection::getDirection() const
{
    return direction;
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


bool CrossSection::addObject(const Object& id_, PolyCurve* const& curve_ )
{
    return objects.addElement( id_, curve_ );
}

const PolyCurve* CrossSection::getObjectCurve( const Object& id_ )
{
    if( objects.findElement( id_ ) == false )
        return nullptr;

    return objects.getElement( id_ );
}


bool CrossSection::removeObjectCurve( const Object& id_ )
{
    if( objects.findElement( id_ ) == false )
        return false;

    objects.removeElement( id_ );
    return true;

}


CrossSection::ObjectsContainer CrossSection::getObjects() const
{
    return objects;
}



void CrossSection::removeObjects()
{
    for ( ObjectsContainer::Iterator it =  objects.begin(); it != objects.end(); ++it )
    {
        (it->second) = nullptr;
    }
    objects.clear();
}


bool CrossSection::hasObjects() const
{
    return !objects.empty();
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
