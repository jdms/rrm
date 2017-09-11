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


#ifndef MODEL3DUTILS_HPP
#define MODEL3DUTILS_HPP

#include <random>


#include <QtGlobal>
#include <vector>

#include "Eigen/Dense"
#include "PolygonalCurve/CurveN.hpp"
//#include "core/geometry/PolygonalCurve/polygonal_curve_2d.hpp"

class Model3DUtils
{
	public:
	
	static std::vector< float > normalizePointCloud( const std::vector< float >& points );
    static std::vector< float > normalizePointCloud( const std::vector< double >& points );
    static std::vector< float > normalizePointCloud( const std::vector< Eigen::Vector3f >& points );
    static Eigen::Affine3f normalizePointCloud( float minx, float maxx, float miny, float maxy, float minz, float maxz );
    static Eigen::Vector3f normalizePointCloud( const Eigen::Vector3f& p, const Eigen::Vector3f& M, const Eigen::Vector3f& m );
    static Curve2D convertToCurve2D( const std::vector< double > &v );

    inline static int randomIntNumber( std::size_t min_, std::size_t max_ )
    {
        std::random_device rd;
        std::mt19937 eng( rd() );
        std::uniform_int_distribution< size_t > distr( min_, max_ );
        return distr( eng );
    }

    inline static void randomColor( int& r_, int& g_, int& b_ )
    {

        std::random_device rd;
        std::mt19937 eng( rd() );
        std::uniform_int_distribution< size_t > distr( 0, 255 );

        r_ = distr( eng );
        b_ = distr( eng );
        g_ = distr( eng );

    }

    static std::vector< float > normalizeMeshIntoABox( const std::vector< float >& points, const Eigen::Vector3f& M, const Eigen::Vector3f& m );


    private:




};




#endif

