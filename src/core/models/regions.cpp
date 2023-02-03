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
 * @file regions.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class Regions
 */


#include "regions.h"


Regions::Regions()
{
    initialize();
}

Regions::Regions(const Regions & reg_)
{

    this->index_cells = reg_.index_cells;
    this->vertices = reg_.vertices;

}


Regions::~Regions()
{
    clear();
}

Regions & Regions::operator=(const Regions & reg_)
{
    this->index_cells = reg_.index_cells;
    this->vertices = reg_.vertices;

    return *this;
}


void Regions::setIndex( const std::size_t id_ )
{
    index = id_;
    name = "Region " + std::to_string( index );
}

std::size_t Regions::getIndex() const
{
    return index;
}


void Regions::setTetrahedralCells( const std::vector< std::size_t >& faces_ )
{
    std::lock_guard<std::mutex> g(geometry_mutex);

    index_cells.clear();
    index_cells.assign( faces_.begin(), faces_.end() );
    tetrahedral_mesh_is_fresh = true;

}


void Regions::getTriangleCells( std::vector< std::size_t >& cells_ ) const
{
    std::lock_guard<std::mutex> g(geometry_mutex);

    cells_.clear();

    std::size_t number_of_tetrahedrals = index_cells.size()/4;
    std::cout << "Number of tetrahedrals: " << number_of_tetrahedrals << std::endl
              << std::flush;

    for( std::size_t i = 0; i < number_of_tetrahedrals; ++i )
    {
        std::size_t v0 = index_cells[ 4*i + 0 ];
        std::size_t v1 = index_cells[ 4*i + 1 ];
        std::size_t v2 = index_cells[ 4*i + 2 ];
        std::size_t v3 = index_cells[ 4*i + 3 ];

        cells_.push_back( v0 );
        cells_.push_back( v1 );
        cells_.push_back( v2 );

        cells_.push_back( v0 );
        cells_.push_back( v2 );
        cells_.push_back( v3 );

        cells_.push_back( v0 );
        cells_.push_back( v3 );
        cells_.push_back( v1 );

        cells_.push_back( v1 );
        cells_.push_back( v3 );
        cells_.push_back( v2 );

    }

    tetrahedral_mesh_is_fresh = false;
}


void Regions::getTetrahedralCells( std::vector< std::size_t >& cells_ ) const
{
    std::lock_guard<std::mutex> g(geometry_mutex);

    cells_.clear();
    cells_.assign( index_cells.begin(), index_cells.end() );
    tetrahedral_mesh_is_fresh = false;
}


void Regions::clearCells()
{
    index_cells.clear();
}


void Regions::setVertices( const std::vector< double >& vertices_  )
{
    std::lock_guard<std::mutex> g(geometry_mutex);

    vertices.clear();
    vertices.assign( vertices_.begin(), vertices_.end() );
    vertices_list_is_fresh = true;
}


void Regions::getVertices( std::vector< double >& vertices_  ) const
{
    std::lock_guard<std::mutex> g(geometry_mutex);

    vertices_.clear();
    vertices_.assign( vertices.begin(), vertices.end() );
    vertices_list_is_fresh = false;
}



void Regions::setLowerBound( const PolyCurve& lower_ )
{
    lower.clear();
    lower = PolyCurve( lower_ );
}

void Regions::setUpperBound( const PolyCurve& upper_ )
{
    upper.clear();
    upper = PolyCurve( upper_ );
}

const PolyCurve& Regions::getLowerBound() const
{
    return lower;
}

const PolyCurve& Regions::getUpperBound() const
{
    return upper;
}


void Regions::setPoint( double x_, double y_, double z_ )
{
    center.x = x_;
    center.y = y_;
    center.z = z_;
}

void Regions::getPoint( double& x_, double& y_, double& z_ ) const
{
    x_ = center.x;
    y_ = center.y;
    z_ = center.z;
}


void Regions::setName( const std::string& name_ )
{
    Object::setName(name_);
/*     name.clear(); */
/*     name = name_; */
}


std::string Regions::getName() const
{
    return Object::getName();
/*     return name; */
}



void Regions::clearVertices()
{
    vertices.clear();
}


void Regions::setMaxMin( double maxx_, double maxy_, double maxz_,
                        double minx_, double miny_, double minz_ )
{
    max.x = maxx_;
    max.y = maxy_;
    max.z = maxz_;

    min.x = minx_;
    min.y = miny_;
    min.z = minz_;


}


void Regions::getMaxMin( double& maxx_, double& maxy_, double& maxz_,
                        double& minx_, double& miny_, double& minz_ ) const
{

    maxx_ = max.x;
    maxy_ = max.y;
    maxz_ = max.z;

    minx_ = min.x;
    miny_ = min.y;
    minz_ = min.z;

}


void Regions::setColor( int r_, int g_, int b_ )
{
    std::lock_guard<std::mutex> g(color_mutex);

    Object::setColor(r_, g_, b_);
    color_is_fresh = true;
}


void Regions::setDomainColor( int r_, int g_, int b_ )
{
    std::lock_guard<std::mutex> g(color_mutex);

    dcolor = {r_, g_, b_};
    color_is_fresh = true;
}


void Regions::getColor( int& r_, int& g_, int& b_ ) const
{
    /* std::lock_guard<std::mutex> g(color_mutex); */

    /* Object::getColor(r_, g_, b_); */
    /* color_is_fresh = false; */

    getBlendedColor(r_, g_, b_);
}


void Regions::getBlendedColor( int& r_, int& g_, int& b_, double f ) const
{
    /* std::lock_guard<std::mutex> g(color_mutex); */

    Object::getColor(r_, g_, b_);

    if (indomain)
    {
        r_ = f * r_ + (1 - f) * dcolor.red;
        g_ = f * g_ + (1 - f) * dcolor.green;
        b_ = f * b_ + (1 - f) * dcolor.blue;
    }

    /* color_is_fresh = false; */
}

void Regions::getColorFor3DView( int& r_, int& g_, int& b_ ) const
{
    std::lock_guard<std::mutex> g(color_mutex);

    getBlendedColor(r_, g_, b_);

    color_is_fresh = false;
}

void Regions::setVolume( double volume_ )
{
    volume = volume_;
}


double Regions::getVolume() const
{
    return volume;
}

void Regions::clear()
{
    lower.clear();
    upper.clear();
    name.clear();

    clearCells();
    clearVertices();
    initialize();
}


void Regions::initialize()
{
    index = 0;

    center.x = 0.0;
    center.y = 0.0;
    center.z = 0.0;

    is_visible = true;
    indomain = false;

    color.red = 255;
    color.green = 0;
    color.blue = 0;


    max.x = 0;
    max.y = 0;
    max.z = 0;

    min.x = 0;
    min.y = 0;
    min.z = 0;

    volume = 0;
}

bool Regions::geometryWasUpdatedSinceLastRead() const
{
    std::lock_guard<std::mutex> g(geometry_mutex);

    return vertices_list_is_fresh || tetrahedral_mesh_is_fresh;
}

bool Regions::colorWasUpdatedSinceLastRead() const
{
    std::lock_guard<std::mutex> g(color_mutex);

    return color_is_fresh;
}

