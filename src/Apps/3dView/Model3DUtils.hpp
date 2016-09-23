#ifndef MODEL3DUTILS_HPP
#define MODEL3DUTILS_HPP

#include <QtGlobal>
#include <vector>

#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"


#include "Core/Geometry/PolygonalCurve2D.hpp"

class Model3DUtils
{
	public:
	
	static std::vector< float > normalizePointCloud( const std::vector< float >& points );
    static std::vector< float > normalizePointCloud( const std::vector< Eigen::Vector3f >& points );
    static Eigen::Affine3f normalizePointCloud( float minx, float maxx, float miny, float maxy, float minz, float maxz );
    static Curve2D convertToCurve2D( const std::vector< float >& v );


    static Eigen::Vector3f normalizePointCloud( const Eigen::Vector3f& p, const Eigen::Vector3f& M, const Eigen::Vector3f& m );




};




#endif

