#include "crosssection.h"

#include "volume.h"
#include "polycurve.h"


CrossSection::CrossSection()
{
    defineIndex();
    initialize();
}


CrossSection::CrossSection( const Volume* raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ): volume( raw_ )
{
    defineIndex();
    initialize();

//    setVolume( raw_ );
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


void CrossSection::setVolume( const Volume* raw_ )
{
//    volume = raw_;
}

const Volume* CrossSection::getVolume() const
{
    return volume;
}


void CrossSection::getCoordinates( std::vector< double >& vertices_ )
{

    double maxx , maxy, maxz;
    double minx , miny, minz;

    getMaxMin( maxx, maxy, maxz, minx, miny, minz );

    if( direction == Settings::CrossSection::CrossSectionDirections::Z )
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
    else if( direction == Settings::CrossSection::CrossSectionDirections::Y )
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
    else if( direction == Settings::CrossSection::CrossSectionDirections::X )
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


void CrossSection::setDirection( const Settings::CrossSection::CrossSectionDirections& dir_ )
{
    direction = dir_;
}

Settings::CrossSection::CrossSectionDirections CrossSection::getDirection() const
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

const std::string& CrossSection::getImagePath()
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
        PolyCurve* curve_ = objects.getElement( it->first );
        if( curve_ == nullptr ) continue;
        curve_ = nullptr;
    }
    objects.clear();
}


bool CrossSection::hasObjects() const
{
    return !objects.empty();
}


void CrossSection::clear()
{

    if( image_path.empty() == false )
        image_path.clear();

    removeObjects();
    initialize();
}

void CrossSection::initialize()
{
    depth = 0.0;
    direction = Settings::CrossSection::CrossSectionDirections::Z;
    is_visible = true;
    number_of_csections = 0;
//    volume = nullptr;
}


CrossSection::~CrossSection()
{
    if( objects.empty() == false )
        clear();
    if( volume != nullptr )
    volume = nullptr;
}


void CrossSection::resetAllCrossSections()
{
    number_of_csections = 0;
}
