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



#include "volume.h"


Volume::Volume()
{
    initialize();
}


void Volume::setOrigin( double ox_, double oy_, double oz_ )
{
    origin_x = ox_;
    origin_y = oy_;
    origin_z = oz_;
}

void Volume::getOrigin( double& ox_, double& oy_, double& oz_ ) const
{
    ox_ = origin_x;
    oy_ = origin_y;
    oz_ = origin_z;
}

void Volume::setGeometry( double w_, double h_, double l_ )
{
    width = w_;
    height = h_;
    lenght = l_;
}

void Volume::getGeometry( double& w_, double& h_, double& l_ ) const
{
    w_ = width ;
    h_ = height;
    l_ = lenght;
}


void Volume::setWidth( double w_ )
{
    width = w_;
}

double Volume::getWidth() const
{
    return width;
}


void Volume::setHeight( double h_  )
{
    height = h_;
}

double Volume::getHeight() const
{
    return  height;
}


void Volume::setLenght( double l_ )
{
    lenght = l_;
}

double Volume::getLenght() const
{
    return lenght;
}



void Volume::setName( const std::string name_ )
{
    name.clear();
    name = name_;
}


const std::string& Volume::getName() const
{
    return name;
}


void Volume::getFrontFace( std::vector< double >& vertices_ ) const
{
    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z + lenght );
}

void Volume::getRightFace( std::vector< double >& vertices_ ) const
{

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z + lenght );


    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z  );
}

void Volume::getBackFace( std::vector< double >& vertices_ ) const
{

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z );
}

void Volume::getLeftFace( std::vector< double >& vertices_ ) const
{

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z );
}

void Volume::getTopFace( std::vector< double >& vertices_ ) const
{

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z + lenght );
}

void Volume::getBottomFace( std::vector< double >& vertices_ ) const
{

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y  );
    vertices_.push_back( origin_z + lenght );
}


bool Volume::addObject( std::size_t id_, Object* const& obj_ )
{
    return objects.addElement( id_, obj_ );
}


bool Volume::removeObject( std::size_t id_ )
{
    if( objects.findElement( id_ ) == false )
        return false;

    Object* obj_ = objects.getElement( id_ );
    if( obj_ != nullptr ) obj_ = nullptr;

    objects.removeElement( id_ );
    return true;
}

Volume::ObjectsContainer Volume::getObjects() const
{
    return objects;
}


bool Volume::addCrossSection( std::size_t id_, CrossSection* const& csection_ )
{
    csections.addElement( id_, csection_ );
    return true;
}


bool Volume::removeCrossSection( std::size_t id_ )
{
    if( csections.findElement( id_ ) == false )
        return false;

    csections.removeElement( id_ );
    return true;
}


Volume::CrossSectionsContainer Volume::getCrossSections() const
{
    return csections;
}



void Volume::addTetrahedralFaces( const std::vector< std::size_t >& faces_ )
{
    faces.push_back( faces_[ 0 ] );
    faces.push_back( faces_[ 1 ] );
    faces.push_back( faces_[ 2 ] );

    faces.push_back( faces_[ 0 ] );
    faces.push_back( faces_[ 2 ] );
    faces.push_back( faces_[ 3 ] );

    faces.push_back( faces_[ 0 ] );
    faces.push_back( faces_[ 3 ] );
    faces.push_back( faces_[ 1 ] );

    faces.push_back( faces_[ 1 ] );
    faces.push_back( faces_[ 3 ] );
    faces.push_back( faces_[ 2 ] );
}


void Volume::getFaces( std::vector< std::size_t >& faces_ ) const
{
    faces_.clear();
    faces_.assign( faces.begin(), faces.end() );
}

void Volume::setVertices( const std::vector< double >& vertices_  )
{
    vertices.clear();
    vertices.assign( vertices_.begin(), vertices_.end() );

//    updateDimensions();
}


void Volume::getVertices( std::vector< double >& vertices_  ) const
{
    vertices_.clear();
    vertices_.assign( vertices.begin(), vertices.end() );
}


void Volume::addRegion( std::size_t index_, const std::vector< std::size_t >& faces_, const Volume::Color& c_ )
{
    regions[ index_ ] = faces_;
    regions_colors[ index_ ] = c_;
    addTetrahedralFaces( faces_ );

}

void Volume::getRegion( std::size_t index_, std::vector< std::size_t >& faces_, Volume::Color& c_ ) const
{
    if( regions.find(index_ ) == regions.end() ) return;
    faces_ = regions.at( index_ );
    c_ = regions_colors.at( index_ );
}


void Volume::updateDimensions()
{
    if( vertices.empty() == true ) return;

    std::size_t number_of_vertices_ = vertices.size()/3;


    double maxx_ = 0.0, maxy_ = 0.0, maxz_ = 0.0;
    double minx_ = 0.0, miny_ = 0.0, minz_ = 0.0;

    maxx_ = vertices[ 0 ];
    maxy_ = vertices[ 1 ];
    maxz_ = vertices[ 2 ];

    minx_ = vertices[ 0 ];
    miny_ = vertices[ 1 ];
    minz_ = vertices[ 2 ];

    for( std::size_t i = 0; i < number_of_vertices_; ++i )
    {
        double x_ = vertices[ 3*i ];
        double y_ = vertices[ 3*i + 1 ];
        double z_ = vertices[ 3*i + 2 ];

        if( x_ >= maxx_  ) maxx_ = x_;
        if( y_ >= maxy_  ) maxy_ = y_;
        if( z_ >= maxz_  ) maxz_ = z_;

        if( x_ <= minx_  ) minx_ = x_;
        if( y_ <= miny_  ) miny_ = y_;
        if( z_ <= minz_  ) minz_ = z_;
    }


    std::cout << "New origin: " << minx_ << ", " << miny_ << ", " << minz_ << std::endl << std::flush;
    std::cout << "New maxs: " << maxx_ << ", " << maxy_ << ", " << maxz_ << std::endl << std::flush;

//    setOrigin( minx_, miny_, minz_ );
    setGeometry( std::fabs( maxx_ - minx_ ), std::fabs( maxy_ - miny_ ), std::fabs( maxz_ - minz_ ) );

//    setWidth( std::fabs( maxx_ - minx_ ) );
//    setHeight( std::fabs( maxy_ - miny_ ) );
//    setLenght( std::fabs( maxz_ - minz_ ) );

}


void Volume::setVisible( bool status_ )
{
    is_visible = status_;
}

bool Volume::isVisible() const
{
    return is_visible;
}

void Volume::setResizable( bool status_ )
{
    is_resizable = status_;
}

bool Volume::isResizable()
{
    if( objects.empty() == true )
        return true;

    ObjectsContainer::Iterator it =  objects.begin();
    Object* obj_ = objects.getElement( it->first );
    if( obj_->isEmpty() == true )
        return true;

    return is_resizable;
}


bool Volume::isEmpty() const
{
    return vertices.empty();
}

void Volume::removeAllObjects()
{
    for ( ObjectsContainer::Iterator it =  objects.begin(); it != objects.end(); ++it )
    {
        Object* obj_ = objects.getElement( it->first );
        if( obj_ == nullptr ) continue;
        obj_ = nullptr;
    }
    objects.clear();
}


void Volume::removeAllCrossSections()
{
    for ( CrossSectionsContainer::Iterator it =  csections.begin(); it != csections.end(); ++it )
    {
        CrossSection* csection_ = csections.getElement( it->first );
        if( csection_ == nullptr ) continue;
        csection_ = nullptr;
    }
    csections.clear();
}



void Volume::clear()
{
    vertices.clear();
    faces.clear();
    for( std::size_t id = 0; id < regions.size(); ++id )
    {
        regions[id].clear();
    }
    regions.clear();
    regions_colors.clear();


    removeAllCrossSections();
    removeAllObjects();
    initialize();
}

void Volume::initialize()
{
    setOrigin( 0.0, 0.0, 0.0 );
    setGeometry( 500, 500, 500 );
    setVisible( true );
    setResizable( false );
}
