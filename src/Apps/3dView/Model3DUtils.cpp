#include "Model3DUtils.hpp"

std::vector< float > Model3DUtils::normalizePointCloud( const std::vector< float >& points )
{
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
	
	
	dim /= scale;
	
	//dimx /= scale;
	//dimy /= scale;
	//dimz /= scale;
	
	max /= scale;
	
	//maxx /= scale;
	//maxy /= scale;
	//maxz /= scale;
		
	min /= scale;
	
	//minx /= scale;
	//miny /= scale;
	//minz /= scale;
		
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
	
	return matrix;
	

}


Curve2D Model3DUtils::convertToCurve2D( const std::vector< float >& v )
{

    Curve2D curve;

    int number_of_vertices = v.size()/2;


    for( int i = 0; i < number_of_vertices; ++i )
    {
        curve.add( Point2D ( v[ 2*i ], v[ 2*i + 1 ] ) );
    }

    return curve;
}

	
	
