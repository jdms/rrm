/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015                                                       *
 * UofC - University of Calgary                                             *
 *                                                                          *
 * This file is part of RRM Software.                                       *
 *                                                                          *
 * RRM is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * RRM is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.             *
 ****************************************************************************/

/**
 * @file crosssection.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class CrossSection
 */


#include "crosssection.h"

#include "volume.h"
#include "polycurve.h"




CrossSection::CrossSection()
{
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

    return *this;
}


CrossSection::~CrossSection()
{
    if( volume != nullptr )
        volume.reset();
    volume = nullptr;

    if( objects.empty() == false )
        clear();

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

        // filling the depth coordinate
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

        // filling the height coordinate
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

        // filling the width coordinate
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


bool CrossSection::addObject(const std::size_t& id_, PolyCurve* const& curve_ )
{
    return objects.addElement( id_, curve_ );
}

const PolyCurve* CrossSection::getObjectCurve( const std::size_t& id_ )
{
    if( objects.findElement( id_ ) == false )
        return nullptr;

    return objects.getElement( id_ );
}


bool CrossSection::removeObjectCurve( const std::size_t& id_ )
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


    boundary.vertices_upper.clear();
    boundary.edges_upper.clear();
    boundary.vertices_lower.clear();
    boundary.edges_lower.clear();
    boundary.empty = true;

    removeObjects();
    initialize();
}

void CrossSection::initialize()
{
    width = 0.0;
    height = 0.0;
    depth = 0.0;;
    direction = Settings::CrossSection::DEFAULT_CSECTION_DIRECTION;
    is_visible = true;
    number_of_csections = 0;
    image_origin.x = 0;
    image_origin.y = 0;
    image_top_right.x = 100.0;
    image_top_right.y = 100.0;
}



void CrossSection::setBoundaryArea( const std::vector< float >& vupper_,  const std::vector< std::size_t >& edupper_, const std::vector< float >& vlower_,  const std::vector< std::size_t >& edlower_  )
{

    boundary.empty = false;

    boundary.vertices_upper.clear();
    boundary.vertices_upper.assign( vupper_.begin(), vupper_.end() );

    boundary.edges_upper.clear();
    boundary.edges_upper.assign( edupper_.begin(), edupper_.end() );


    boundary.vertices_lower.clear();
    boundary.vertices_lower.assign( vlower_.begin(), vlower_.end() );

    boundary.edges_lower.clear();
    boundary.edges_lower.assign( edlower_.begin(), edlower_.end() );


}

void CrossSection::getBoundaryArea( std::vector< float >& vupper_,  std::vector< std::size_t >& edupper_, std::vector< float >& vlower_,  std::vector< std::size_t >& edlower_  ) const
{

    vupper_.clear();
    vupper_.assign( boundary.vertices_upper.begin(), boundary.vertices_upper.end() );

    edupper_.clear();
    edupper_.assign( boundary.edges_upper.begin(), boundary.edges_upper.end() );

    vlower_.clear();
    vlower_.assign( boundary.vertices_lower.begin(), boundary.vertices_lower.end() );

    edlower_.clear();
    edlower_.assign( boundary.edges_lower.begin(), boundary.edges_lower.end() );

}


void CrossSection::clearBoundaryArea()
{
    boundary.vertices_upper.clear();
    boundary.edges_upper.clear();
    boundary.vertices_lower.clear();
    boundary.edges_lower.clear();
    boundary.empty = true;
}

bool CrossSection::hasBoundaryArea() const
{
    return !(boundary.empty);
}



void CrossSection::resetAllCrossSections()
{
    number_of_csections = 0;
}
