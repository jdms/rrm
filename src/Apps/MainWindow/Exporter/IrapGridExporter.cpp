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

#include "IrapGridExporter.hpp"

IrapGridExporter::IrapGridExporter()
{
	undefValueGrid = "9999900.000000";
    angle = 0.0f;
    
}

void IrapGridExporter::setBoundingBox( float xMin, float xMax, float yMin, float yMax, float zMin, float zMax )
{
    maximum[ 0 ] = xMax;
    maximum[ 1 ] = yMax;
    maximum[ 2 ] = zMax;

    minimum[ 0 ] = xMin;
    minimum[ 1 ] = yMin;
    minimum[ 2 ] = zMin;
}

void IrapGridExporter::setVectorValues( const std::vector<float>& vvalues )
{
    if( vvalues.empty() == true ) return;

    values.clear();
    values.resize( vvalues.size() );
    std::copy( vvalues.begin(), vvalues.end(), values.begin() );
}

void IrapGridExporter::setSize( int nlinesI, int nlinesJ )
{
    nI = nlinesI;
    nJ = nlinesJ;
}

void IrapGridExporter::setSpacing( float dx, float dy )
{
    dX = dx;
    dY = dy;
}


void IrapGridExporter::clearData()
{

	maximum[ 0 ] = 0;
	maximum[ 1 ] = 0;

	minimum[ 0 ] = 0;
	minimum[ 1 ] = 0;
	
	
	nI = 0;
    nJ = 0;
	
	dX = 0;
    dY = 0;
	
	values.clear();
	
	
}

bool IrapGridExporter::writeGridData( const std::string& filename ) const
{

    std::ofstream file;
    file.open( filename.c_str() );
    if( !file.is_open() ) return false;
	
	  ///
    /// writing the header.
    /// the values are distributed by nlinesJ blocks, which contains nlinesI elements distributed by NCOLUMNS columns.
    ///


    file <<  std::setprecision( 15 ) << "-996 " << nJ  << " " << dX  << " " << dY  << std::endl;

    file << std::setprecision( 15 ) << minimum[ 0 ] << " " << maximum[ 0 ] << " " << minimum[ 1 ] << " " << maximum[ 1 ] << std::endl;


    file <<  std::setprecision( 15 ) << nI << " " << angle << " " << minimum[ 0 ] << " " << minimum[ 1 ] << std::endl;

    file << "0 0 0 0 0 0 0" << std::endl;

    ///
    /// writing the values.
    /// the values are distributed by nlinesJ blocks, which contains nlinesI elements distributed by NCOLUMNSVALUES columns.
    ///


    if( values.empty() == true  )
        return false;



    int int_elemens = (int) values.size()/NCOLUMNSGRID;
    float indef = std::atof( undefValueGrid.c_str() );


    int elem = 0;
    for( int i = 0; i < int_elemens; i++, elem += NCOLUMNSGRID )
    {

        if( fabs( values[ elem ] - indef ) < FLT_EPSILON )
            file << undefValueGrid.c_str() << " ";
        else
            file << std::setprecision( 15 ) << values[ elem ] << " ";


        if( fabs( values[ elem + 1 ] - indef ) < FLT_EPSILON )
             file << undefValueGrid.c_str() << " ";
        else
            file << std::setprecision( 15 ) << values[ elem + 1 ] << " ";


        if( fabs( values[ elem + 2 ] - indef ) < FLT_EPSILON )
            file << undefValueGrid.c_str() << " ";
        else
            file << std::setprecision( 15 ) << values[ elem + 2 ] << " ";


        if( fabs( values[ elem + 3 ] - indef ) < FLT_EPSILON )
             file << undefValueGrid.c_str() << " ";
        else
            file << std::setprecision( 15 ) << values[ elem + 3 ] << " ";


        if( fabs( values[ elem + 4 ] - indef ) < FLT_EPSILON )
            file << undefValueGrid.c_str() << " ";
        else
            file << std::setprecision( 15 ) << values[ elem + 4 ] <<  " ";


        if( fabs( values[ elem + 5 ] - indef ) < FLT_EPSILON )
            file << undefValueGrid.c_str() << " ";
        else
            file << std::setprecision( 15 ) << values[ elem + 5 ] << std::endl;

    }

    for( int k = elem; k < (int) values.size(); ++k )
    {
        if( fabs( values[ k ] - indef ) < FLT_EPSILON )
             file << "9999900.000000 ";
        else
            file << std::setprecision( 15 ) << values[ k ] << " ";

        if( k == values.size() - 1 )
            file << std::endl;
    }



    return true;


	file.close();
    return true;
	
	
}
