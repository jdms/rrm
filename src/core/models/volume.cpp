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
 * @file volume.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class Volume
 */


#include "volume.h"


Volume::Volume():Object()
{
    std::cout << "VOLUME CREATED" << std::endl << std::flush;
    initialize();
}

Volume::Volume(const Volume& volume_)
{

    this->originx = volume_.originx;
    this->originy = volume_.originy;
    this->originz = volume_.originz;

    this->width = volume_.width;
    this->height = volume_.height;
    this->lenght = volume_.lenght;

    std::cout << "VOLUME CREATED" << std::endl << std::flush;

}


Volume& Volume::operator=(const Volume& volume_)
{
    this->originx = volume_.originx;
    this->originy = volume_.originy;
    this->originz = volume_.originz;

    this->width = volume_.width;
    this->height = volume_.height;
    this->lenght = volume_.lenght;

    return *this;
}

Volume::~Volume()
{
    clear();
}


void Volume::setGeometry( double ox_, double oy_, double oz_, double w_, double h_, double l_ )
{
    setOrigin( ox_, oy_, oz_ );
    setDimensions( w_, h_, l_ );
}

void Volume::getGeometry( double & ox_, double & oy_, double & oz_, double & w_, double & h_, double & l_ ) const
{
    getOrigin( ox_, oy_, oz_ );
    getDimensions( w_, h_, l_ );
}


void Volume::setOrigin( double ox_, double oy_, double oz_ )
{
    originx = ox_;
    originy = oy_;
    originz = oz_;
}

void Volume::getOrigin( double& ox_, double& oy_, double& oz_ ) const
{
    ox_ = originx;
    oy_ = originy;
    oz_ = originz;
}

void Volume::setDimensions( double w_, double h_, double l_ )
{
    width = w_;
    height = h_;
    lenght = l_;
}

void Volume::getDimensions( double & w_, double & h_, double & l_ ) const
{
    w_ = width;
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


void Volume::getFrontFace( std::vector< double >& vertices_ ) const
{
    vertices_.push_back( originx );
    vertices_.push_back( originy );
    vertices_.push_back( originz + lenght );

    vertices_.push_back( originx + width );
    vertices_.push_back( originy );
    vertices_.push_back( originz + lenght );

    vertices_.push_back( originx + width );
    vertices_.push_back( originy + height );
    vertices_.push_back( originz + lenght );

    vertices_.push_back( originx );
    vertices_.push_back( originy + height );
    vertices_.push_back( originz + lenght );
}

void Volume::getRightFace( std::vector< double >& vertices_ ) const
{

    vertices_.push_back( originx + width );
    vertices_.push_back( originy + height );
    vertices_.push_back( originz + lenght );


    vertices_.push_back( originx + width );
    vertices_.push_back( originy );
    vertices_.push_back( originz + lenght );

    vertices_.push_back( originx + width );
    vertices_.push_back( originy );
    vertices_.push_back( originz );

    vertices_.push_back( originx + width );
    vertices_.push_back( originy + height );
    vertices_.push_back( originz  );
}

void Volume::getBackFace( std::vector< double >& vertices_ ) const
{

    vertices_.push_back( originx + width );
    vertices_.push_back( originy );
    vertices_.push_back( originz );

    vertices_.push_back( originx );
    vertices_.push_back( originy );
    vertices_.push_back( originz );

    vertices_.push_back( originx );
    vertices_.push_back( originy + height );
    vertices_.push_back( originz );

    vertices_.push_back( originx + width );
    vertices_.push_back( originy + height );
    vertices_.push_back( originz );
}

void Volume::getLeftFace( std::vector< double >& vertices_ ) const
{

    vertices_.push_back( originx );
    vertices_.push_back( originy );
    vertices_.push_back( originz );

    vertices_.push_back( originx );
    vertices_.push_back( originy );
    vertices_.push_back( originz + lenght );

    vertices_.push_back( originx );
    vertices_.push_back( originy + height );
    vertices_.push_back( originz + lenght );

    vertices_.push_back( originx );
    vertices_.push_back( originy + height );
    vertices_.push_back( originz );
}

void Volume::getTopFace( std::vector< double >& vertices_ ) const
{

    vertices_.push_back( originx + width );
    vertices_.push_back( originy + height );
    vertices_.push_back( originz );

    vertices_.push_back( originx );
    vertices_.push_back( originy + height );
    vertices_.push_back( originz );

    vertices_.push_back( originx );
    vertices_.push_back( originy + height );
    vertices_.push_back( originz + lenght );

    vertices_.push_back( originx + width );
    vertices_.push_back( originy + height );
    vertices_.push_back( originz + lenght );
}

void Volume::getBottomFace( std::vector< double >& vertices_ ) const
{

    vertices_.push_back( originx );
    vertices_.push_back( originy );
    vertices_.push_back( originz );

    vertices_.push_back( originx + width );
    vertices_.push_back( originy );
    vertices_.push_back( originz );

    vertices_.push_back( originx + width );
    vertices_.push_back( originy );
    vertices_.push_back( originz + lenght );

    vertices_.push_back( originx );
    vertices_.push_back( originy  );
    vertices_.push_back( originz + lenght );
}


void Volume::clear()
{
    originx = DEFAULT_ORIGIN_X;
    originy = DEFAULT_ORIGIN_Y;
    originz = DEFAULT_ORIGIN_Z;

    width = DEFAULT_WIDTH;
    height = DEFAULT_HEIGHT;
    lenght = DEFAULT_LENGHT;

//    Object::clear();
}


