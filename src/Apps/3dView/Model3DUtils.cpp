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

#include "Model3DUtils.hpp"

std::vector< float > Model3DUtils::normalizePointCloud( const std::vector< float >& points )
{
    if( points.empty() == true ) return points;

	float maxx, minx;
	float maxy, miny;
	float maxz, minz;
	
	int number_of_points = points.size()/3;
	
	maxx = points[ 0 ];
	minx = points[ 0 ];
	maxy = points[ 1 ];
	miny = points[ 1 ];
	maxz = points[ 2 ];
	minz = points[ 2 ];
		
	for( int i = 0; i < number_of_points; ++i )
	{
		if( points[ 3*i ] >= maxx ) maxx = points[ 3*i ];
		if( points[ 3*i ] <= minx ) minx = points[ 3*i ];
		
		if( points[ 3*i + 1 ] >= maxy ) maxy = points[ 3*i + 1 ];
		if( points[ 3*i + 1 ] <= miny ) miny = points[ 3*i + 1 ];
		
		if( points[ 3*i + 2 ] >= maxz ) maxz = points[ 3*i + 2 ];
		if( points[ 3*i + 2 ] <= minz ) minz = points[ 3*i + 2 ];
	}
	
	float dimx = maxx - minx;
	float dimy = maxy - miny;
	float dimz = maxz - minz;
	
	float scale = std::max( std::max( dimx, dimy ), dimz );

    dimx /= scale;
    dimy /= scale;
    dimz /= scale;

    maxx /= scale;
    maxy /= scale;
    maxz /= scale;
		
    minx /= scale;
    miny /= scale;
    minz /= scale;
		
	Eigen::Vector4f center = Eigen::Vector4f( ( maxx + minx )*0.5f,( maxy + miny )*0.5f,( maxz + minz )*0.5f, 0.0f  );
	Eigen::Affine3f matrix = Eigen::Affine3f::Identity();
	
    matrix.translation() = -center.head< 3 >();
	matrix.scale( Eigen::Vector3f( 1.0f/scale, 1.0f/scale, 1.0f/scale ) );
	

	std::vector< float > normalized_points;
	for( int i = 0; i < number_of_points; ++i )
	{
		Eigen::Vector4f p( points[ 3*i ], points[ 3*i + 1 ], points[ 3*i + 2 ], 1.0f );
		p = matrix*p;
		
        normalized_points.push_back( p.x() );
        normalized_points.push_back( p.y() );
        normalized_points.push_back( p.z() );
	}
		
	return normalized_points;
	
}


std::vector< float > Model3DUtils::normalizePointCloud( const std::vector< double >& points )
{
    if( points.empty() == true ) return std::vector< float >();

    double maxx, minx;
    double maxy, miny;
    double maxz, minz;

    int number_of_points = (int)points.size()/3;

    maxx = points[ 0 ];
    minx = points[ 0 ];
    maxy = points[ 1 ];
    miny = points[ 1 ];
    maxz = points[ 2 ];
    minz = points[ 2 ];

    for( int i = 0; i < number_of_points; ++i )
    {
        if( points[ 3*i ] >= maxx ) maxx = points[ 3*i ];
        if( points[ 3*i ] <= minx ) minx = points[ 3*i ];

        if( points[ 3*i + 1 ] >= maxy ) maxy = points[ 3*i + 1 ];
        if( points[ 3*i + 1 ] <= miny ) miny = points[ 3*i + 1 ];

        if( points[ 3*i + 2 ] >= maxz ) maxz = points[ 3*i + 2 ];
        if( points[ 3*i + 2 ] <= minz ) minz = points[ 3*i + 2 ];
    }

    double dimx = maxx - minx;
    double dimy = maxy - miny;
    double dimz = maxz - minz;

    double scale = std::max( std::max( dimx, dimy ), dimz );

    dimx /= scale;
    dimy /= scale;
    dimz /= scale;

    maxx /= scale;
    maxy /= scale;
    maxz /= scale;

    minx /= scale;
    miny /= scale;
    minz /= scale;

    Eigen::Vector4f center = Eigen::Vector4f( ( maxx + minx )*0.5f,( maxy + miny )*0.5f,( maxz + minz )*0.5f, 0.0f  );
    Eigen::Affine3f matrix = Eigen::Affine3f::Identity();

    matrix.translation() = -center.head< 3 >();
    matrix.scale( Eigen::Vector3f( 1.0f/scale, 1.0f/scale, 1.0f/scale ) );


    std::vector< float > normalized_points;
    for( int i = 0; i < number_of_points; ++i )
    {
        Eigen::Vector4f p( ( float )points[ 3*i ], ( float )points[ 3*i + 1 ],( float ) points[ 3*i + 2 ], 1.0f );
        p = matrix*p;

        normalized_points.push_back( p.x() );
        normalized_points.push_back( p.y() );
        normalized_points.push_back( p.z() );
    }

    return normalized_points;

}


std::vector< float > Model3DUtils::normalizePointCloud( const std::vector< Eigen::Vector3f >& points )
{
	Eigen::Vector3f max, min;
	
	int number_of_points = points.size();

    max = points[ 0 ];
	min = points[ 0 ];
		
	for( int i = 0; i < number_of_points; ++i )
	{
        if( points[ i ].x() >= max.x() ) max.x() = points[ i ].x();
        if( points[ i ].x() <= min.x() ) min.x() = points[ i ].x();
		
        if( points[ i ].y() >= max.y() ) max.y() = points[ i ].y();
        if( points[ i ].y() <= min.y() ) min.y() = points[ i ].y();
		
        if( points[ i ].z() >= max.z() ) max.z() = points[ i ].z();
        if( points[ i ].z() <= min.z() ) min.z() = points[ i ].z();
	}
	
	Eigen::Vector3f dim = max - min;
	
    float scale = std::max( std::max( dim.x(), dim.y() ), dim.z() );
	
    max /= scale;
    min /= scale;

    Eigen::Vector4f center = Eigen::Vector4f( ( max.x() + min.x() )*0.5f,( max.y() + min.y() )*0.5f,( max.z() + min.z() )*0.5f, 0.0f  );
	Eigen::Affine3f matrix = Eigen::Affine3f::Identity();
	
    matrix.translation() = -center.head< 3 >();
	matrix.scale( Eigen::Vector3f( 1.0f/scale, 1.0f/scale, 1.0f/scale ) );
	
	
	std::vector< float > normalized_points;
	for( int i = 0; i < number_of_points; ++i )
	{
        Eigen::Vector4f p( points[ i ].x(), points[ i ].y(), points[ i ].z(), 1.0f );
		p = matrix*p;
		
        normalized_points.push_back( p.x() );
        normalized_points.push_back( p.y() );
        normalized_points.push_back( p.z() );
	}
		
	return normalized_points;
	
}


Eigen::Affine3f Model3DUtils::normalizePointCloud( float minx, float maxx, float miny, float maxy, float minz, float maxz )
{

	float dimx = maxx - minx;
	float dimy = maxy - miny;
	float dimz = maxz - minz;
	
    float L = std::max( std::max( dimx, dimy ), dimz );

    maxx /= L;
    maxy /= L;
    maxz /= L;

    minx /= L;
    miny /= L;
    minz /= L;

    Eigen::Vector4f center = Eigen::Vector4f( (maxx + minx)*0.5f, (maxy + miny)*0.5f, (maxz + minz)*0.5f, 0.0f );

    Eigen::Affine3f matrix = Eigen::Affine3f::Identity();
    matrix.translation() = -center.head< 3 >();
    matrix.scale( Eigen::Vector3f( 1.0f/L, 1.0f/L, 1.0f/L ) );
	
	return matrix;
	}


Curve2D Model3DUtils::convertToCurve2D( const std::vector< double >& v )
{

    Curve2D curve;

    int number_of_vertices = v.size()/2;


    for( int i = 0; i < number_of_vertices; ++i )
    {
        curve.add( Point2D ( v[ 2*i ], v[ 2*i + 1 ] ) );
    }

    return curve;
}

		
Eigen::Vector3f Model3DUtils::normalizePointCloud( const Eigen::Vector3f& p, const Eigen::Vector3f& M, const Eigen::Vector3f& m )
{

    float dimx = M.x() - m.x();
    float dimy = M.y() - m.y();
    float dimz = M.z() - m.z();


    float scale = std::max( std::max( dimx, dimy ), dimz );



    Eigen::Vector3f center = Eigen::Vector3f( ( M.x() + m.x() )*0.5f,( M.y() + m.y() )*0.5f,( M.z() + m.z() )*0.5f );
    Eigen::Vector3f cpy( p.x(), p.y(), p.z() );

    cpy -= center;
    cpy /= scale;


    return cpy;
}


