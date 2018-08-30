/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "crosssection.h"

#include "volume.h"
#include "polycurve.h"




CrossSection::CrossSection()
{
    std::cout << "CROSS-SECTION CREATED" << std::endl << std::flush;

    defineIndex();
    initialize();
}

CrossSection::CrossSection( const std::shared_ptr< Volume >& volume_, const Settings::CrossSection::CrossSectionDirections& direction_, double depth_ )
{
    volume = volume_;


    defineIndex();
    initialize();

    setDirection( direction_ );
    setDepth( depth_ );

//    updateDimensions();
    std::cout << "CROSS-SECTION CREATED" << std::endl << std::flush;

}


CrossSection::CrossSection( const CrossSection& csection_ )
{
    this->direction = csection_.direction;
    this->volume = csection_.volume;
    this->width = csection_.width;
    this->height = csection_.height;
    this->depth = csection_.depth;

    objects = csection_.objects;
    image_path = csection_.image_path;
    is_visible = csection_.is_visible;
    index = csection_.index;

//    updateDimensions();
    std::cout << "CROSS-SECTION CREATED" << std::endl << std::flush;

}


CrossSection& CrossSection::operator=( const CrossSection& csection_ )
{
    this->direction = csection_.direction;
    this->volume = csection_.volume;
    this->width = csection_.width;
    this->height = csection_.height;
    this->depth = csection_.depth;

    objects = csection_.objects;
    image_path = csection_.image_path;
    is_visible = csection_.is_visible;
    index = csection_.index;


//    updateDimensions();


    return *this;
}


CrossSection::~CrossSection()
{
    std::cout << "CROSS-SECTION DELETED" << std::endl << std::flush;

    if( volume != nullptr )
        volume.reset();
    volume = nullptr;

    if( objects.empty() == false )
        clear();

//    Object::clear();
}


void CrossSection::updateDimensions()
{
    if ( volume == nullptr ) return;


    if ( direction == Settings::CrossSection::CrossSectionDirections::X )
    {
        width = volume->getLenght();
        height = volume->getHeight();
    }
    else if ( direction == Settings::CrossSection::CrossSectionDirections::Y )
    {
        width = volume->getWidth();
        height = volume->getLenght();
    }
    else if ( direction == Settings::CrossSection::CrossSectionDirections::Z )
    {
        width = volume->getWidth();
        height = volume->getHeight();
    }

    std::cout << "Csection width = " << volume->getWidth() << std::endl << std::flush;

}



///========================================================================


void CrossSection::setIndex( const std::size_t id_ )
{
    index = id_;
    number_of_csections = id_ + 1;
}

std::size_t CrossSection::getIndex() const
{
    return index;
}


void CrossSection::setVolume( const std::shared_ptr< Volume >& volume_ )
{
    volume = volume_;
}

const std::shared_ptr< Volume >& CrossSection::getVolume() const
{
    return volume;
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

///========================================================================







//CrossSection::CrossSection()
//{
//    defineIndex();
//    initialize();
//}


//CrossSection::CrossSection( const Volume* raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ): volume( raw_ )
//{
//    defineIndex();
//    initialize();

//    setDirection( dir_ );
//    setDepth( depth_ );
//}


//CrossSection& CrossSection::operator=( const CrossSection& other_ )
//{
//    volume = other_.volume;
//    objects = other_.objects;
//    direction = other_.direction;
//    depth = other_.depth;
//    image_path = other_.image_path;
//    is_visible = other_.is_visible;
//    index = other_.index;

//    return *this;
//}


void CrossSection::defineIndex()
{
    index = number_of_csections;
    number_of_csections++;
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
    volume->getDimensions( w, h, l );

    maxx_ = minx_ + w;
    maxy_ = miny_ + h;
    maxz_ = minz_ + l;

}




void CrossSection::setImage( const std::string& path_, double ox_, double oy_, double x_, double y_ )
{
    image_path = path_;
    image_origin.x = ox_;
    image_origin.y = oy_;
    image_top_right.x = x_;
    image_top_right.y = y_;
}

void CrossSection::getImage( std::string& path_, double& ox_, double& oy_, double& x_, double& y_ )
{
    path_ = image_path;
    ox_ = image_origin.x;
    oy_ = image_origin.y;
    x_ = image_top_right.x;
    y_ = image_top_right.y;
}


bool CrossSection::hasImage()
{
    return !image_path.empty();
}


void CrossSection::clearImage()
{
    image_path.clear();
    image_origin.x = 0.0;
    image_origin.y = 0.0;
    image_top_right.x = 100.0;
    image_top_right.y = 100;
}


void CrossSection::setVisible( const bool status_ )
{
    is_visible = status_;
}

bool CrossSection::isVisible() const
{
    if( volume->isVisible() == false ) return false;
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
    image_origin.x = 0;
    image_origin.y = 0;
    image_top_right.x = 100.0;
    image_top_right.y = 100.0;
//    volume = nullptr;
}



void CrossSection::setBounderingArea( const std::vector< float >& vupper_,  const std::vector< std::size_t >& edupper_, const std::vector< float >& vlower_,  const std::vector< std::size_t >& edlower_  )
{

    boundering.vertices_upper = vupper_;
    boundering.edges_upper = edupper_;
    boundering.vertices_lower = vlower_;
    boundering.edges_lower = edlower_;
    boundering.empty = false;

}

void CrossSection::getBounderingArea( std::vector< float >& vupper_,  std::vector< std::size_t >& edupper_, std::vector< float >& vlower_,  std::vector< std::size_t >& edlower_  ) const
{

    vupper_ = boundering.vertices_upper;
    edupper_ = boundering.edges_upper;
    vlower_ = boundering.vertices_lower;
    edlower_ = boundering.edges_lower;
}


void CrossSection::clearBounderingArea()
{
    boundering.vertices_upper.clear();
    boundering.edges_upper.clear();
    boundering.vertices_lower.clear();
    boundering.edges_lower.clear();
    boundering.empty = true;
}

bool CrossSection::hasBounderingArea() const
{
    return !(boundering.empty);
}



void CrossSection::resetAllCrossSections()
{
    number_of_csections = 0;
}
