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
 * @file irap_grid_exporter.h
 * @author Clarissa C. Marques
 * @date 9 Sep 2012
 * @brief File containing the class IrapGridExporter
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

/**
 *  A wrapper to export surfaces meshes to the format Irap Grid
 */

class IrapGridExporter
{

    public:


        /**
        * Constructor.
        */
        IrapGridExporter();


        /**
        * Method to set the bounding box of the object
        * @param xMax the maximum value of X
        * @param yMax the maximum value of Y
        * @param zMax the maximum value of Z
        * @param xMin the minimum value of X
        * @param yMin the minimum value of Y
        * @param zMin the minimum value of Z
        * @return Void
        */
        void setBoundingBox(float xMin, float xMax, float yMin, float yMax , float zMin, float zMax );


        /**
        * Method to set the height values of each vertice of the surface mesh
        * @param vvalues the height values of each vertice of the surface mesh
        * @return Void
        */
        void setVectorValues( const std::vector<float>& vvalues );


        /**
        * Method to set the size of the grid, i.e., the number of discretization in both directions, 'WIDTH' and 'DEPTH'
        * @param nlinesI the number of discretization in the direction 'WIDTH'
        * @param nlinesJ the number of discretization in the direction 'DEPTH'
        * @return Void
        */
        void setSize( int nlinesI, int nlinesJ );


        /**
        * Method to set the discretization intervals in both directions, 'WIDTH' and 'DEPTH'
        * @param dx the discretization interval in the direction 'WIDTH'
        * @param dy the discretization interval in the direction 'DEPTH'
        * @return Void
        */
        void setSpacing( float dx, float dy );


        /**
        * Method to save the mesh information in a file
        * @param filename the name of the file to be saved
        * @return boolean returns true if the file was saved with success and false otherwise
        */
        bool writeGridData( const std::string& filename ) const;


        /**
        * Method to clear all mesh information
        * @return Void
        */
        void clearData();


    protected:

        std::string fileName;                                               /**< Name of the file to be read or write. */

        static const int NCOLUMNSGRID = 6;                                  /**< Number of columns for each row to write the elements. */

        int             nI, nJ, nK;                                         /**< Number of discretization in each direction 'WIDTH', 'HEIGHT' and 'DEPTH', respectively. */

        float           dX, dY, dZ;                                         /**< Interval of discretization in each direction 'WIDTH', 'HEIGHT' and 'DEPTH', respectively. */

        float           maximum[ 3 ];                                       /**< The maximum point of the bounding box. */

        float           minimum[ 3 ];                                       /**< The minimum point of the bounding box. */

        std::string undefValueGrid;                                         /**< Define the used value to represent undefined values. */

        std::vector< float > values;                                        /**< Vector with the height coordinate of each vertice */

        float           angle;                                              /**< Define the angle of rotation applied to the vertices, if existent. */


};

#endif // IRAPGRIDEXPORTER_H
