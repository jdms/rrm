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


#ifndef IRAPGRIDEXPORTER_H
#define IRAPGRIDEXPORTER_H

#include <iostream>
#include <istream>
#include <fstream>

#include <QString>


#include <math.h>
#include <iomanip>
#include <float.h>

#include <vector>


class IrapGridExporter
{

    public:


        IrapGridExporter();

        void setBoundingBox(float xMin, float xMax, float yMin, float yMax , float zMin, float zMax );
        void setVectorValues( const std::vector<float>& vvalues );
        void setSize( int nlinesI, int nlinesJ );
        void setSpacing( float dx, float dy );
		
        bool writeGridData( const std::string& filename ) const;
        void clearData();



    protected:

        std::string fileName;               ///< Name of the CPS3 file to be read or write.
        static const int NCOLUMNSGRID = 6;

        int             nI, nJ, nK;

        float           dX, dY, dZ;
        float           maximum[ 3 ], minimum[ 3 ];

        std::string undefValueGrid;         ///< Define the used value to represent undefined values.

        std::vector< float > values;
        float           angle;

};

#endif // IRAPGRIDEXPORTER_H
