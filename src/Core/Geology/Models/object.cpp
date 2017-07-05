#include "object.h"


Object::Object()
{
    setDefaultValues();
    ++count_objects;
}


Object::Object( const Object::Type& t )
{
    setDefaultValues();
    type = t;

    ++count_objects;
}


void Object::setId( std::size_t id )
{
    index = id;
    count_objects = id + 1;
}



std::size_t Object::getId() const
{
    return index;
}

void Object::setType( const Object::Type& t )
{
    type = t;
}


Object::Type Object::getType() const
{
    return type;
}


void Object::setName( const std::string& n )
{
    name = n;
}

std::string Object::getName() const
{
    return name;
}


void Object::setColor( int r, int g, int b )
{
    color = std::make_tuple( r, g, b );
}


void Object::getColor( int& r, int& g, int& b ) const
{
    r = std::get<0>( color );
    g = std::get<1>( color );
    b = std::get<2>( color );
}


void Object::setEditable( bool status )
{
    is_editable = status;
}


bool Object::getEditable() const
{
    return is_editable;
}



void Object::setSelectable( bool status )
{
    is_selectable = status;
}


bool Object::getSelectable() const
{
    return is_selectable;
}



void Object::setSelected( bool status )
{
    is_selected = status;
}


bool Object::getSelected() const
{
    return is_selected;
}


void Object::setVisibility( bool status )
{
    is_visible = status;
}


bool Object::getVisibility() const
{
    return is_visible;
}


bool Object::isEmpty() const
{
    bool empty = csections_curves.empty() && csections_edges.empty() &&
                 ( !has_trajectory );
    return empty;
}

bool Object::isTesting() const
{
    return is_test;
}

void Object::setTesting( bool status )
{
    is_test = status;
}

void Object::setCrossSectionCurve( double depth, const Curve2D& curve,
                           std::vector< std::size_t > edges )
{
    csections_curves[ depth ] = curve;
    csections_edges[ depth ] = edges;
}


Curve2D Object::getCrossSectionCurve( double depth )
{
    if( hasCurve( depth ) == false ) return Curve2D();
    return csections_curves[ depth ];
}


std::vector< std::tuple< Curve2D, double > > Object::getCrossSectionCurves()
{
    std::vector< std::tuple< Curve2D, double > > curves;

    for( auto it: csections_curves )
    {
        std::cout <<  it.first << " " << std::flush;
        curves.push_back( std::make_tuple( it.second, it.first ) );
    }

    std::cout <<  std::endl << std::flush;

    return curves;
}


std::vector< std::size_t > Object::getCrossSectionCurveEdges( double depth )
{
    if( hasCurve( depth ) == false ) return std::vector< std::size_t >();
    return csections_edges[ depth ];
}


void Object::removeCrossSectionCurve( double depth )
{
    if( hasCurve( depth ) == false ) return;

    csections_curves[ depth ].clear();
    csections_curves.erase( depth );

    csections_edges[ depth ].clear();
    csections_edges.erase( depth );
}


void Object::removeCrossSections()
{
    if( csections_curves.empty() == true ) return;

    std::map< double, Curve2D >::iterator it = csections_curves.begin();
    while ( it != csections_curves.end()  )
    {
        removeCrossSectionCurve( it->first );
        it = csections_curves.begin();
    }

    csections_curves.clear();
    csections_edges.clear();
}



bool Object::isTrajectoryAdmissible()
{

    if( ( csections_curves.size() > 1  ) || ( has_trajectory == true ) )
        return false;

    return true;
}



bool Object::isCurveAdmissible()
{
    if( ( has_trajectory == true ) && ( csections_curves.size() > 0  ) )
        return false;

    return true;
}


bool Object::hasCurve( double depth )
{
    auto search = csections_curves.find( depth );
    if( search != csections_curves.end() )
        return true;
    else
        return false;
}

bool Object::hasTrajectoryCurve()
{
    return has_trajectory;
}


void Object::setTrajectoryCurve( const Curve2D& path )
{
    trajectory_curve.clear();
    trajectory_curve = path;
    has_trajectory = true;
}


Curve2D Object::getTrajectoryCurve() const
{
    return trajectory_curve;
}


void Object::removeTrajectoryCurve()
{
    trajectory_curve.clear();
    has_trajectory = false;
}


void Object::setSurface( const std::vector< double >& vertices,
                                    const std::vector< std::size_t >& faces, bool test )
{
    removeSurface();
    surface_vertices.assign( vertices.begin(), vertices.end() );
    surface_faces.assign( faces.begin(), faces.end() );

    is_editable = test;
    is_test = test;
}


void Object::setSurfaceNormals( const std::vector< double >& normals )
{
    surface_normals.clear();
    surface_normals.assign( normals.begin(), normals.end() );
}


std::vector< double > Object::getSurfaceVertices() const
{
    return surface_vertices;
}


std::vector< std::size_t > Object::getSurfaceFaces() const
{
    return surface_faces;
}


std::vector< double > Object::getSurfaceNormals()
{
    std::vector< double > normals_list;

    if( surface_normals.empty() == true )
        normals_list = computeNormals();
    else
        normals_list = surface_normals;

    return normals_list;
}


std::vector< double > Object::computeNormals()
{
    std::size_t number_of_faces = surface_faces.size()/3;
    std::size_t size_vector  = surface_vertices.size();

    std::vector< double > normals;
    normals.resize( size_vector );

    if( ( number_of_faces == 0 ) || ( size_vector == 0 ) ) return std::vector< double >();

    for( std::size_t i = 0; i < number_of_faces; ++i )
    {
        std::size_t i1 = surface_faces[ 3*i ];
        std::size_t i2 = surface_faces[ 3*i + 1 ];
        std::size_t i3 = surface_faces[ 3*i + 2 ];

        Eigen::Vector3f v1( surface_vertices[ 3*i1 ], surface_vertices[ 3*i1 + 1 ],
                            surface_vertices[ 3*i1 + 2 ] );

        Eigen::Vector3f v2( surface_vertices[ 3*i2 ], surface_vertices[ 3*i2 + 1 ],
                            surface_vertices[ 3*i2 + 2 ] );

        Eigen::Vector3f v3( surface_vertices[ 3*i3 ], surface_vertices[ 3*i3 + 1 ],
                            surface_vertices[ 3*i3 + 2 ] );

        Eigen::Vector3f a = v2 - v1;
        Eigen::Vector3f b = v3 - v1;

        Eigen::Vector3f face_normal = b.cross( a );

        normals[ 3*i1 ] += face_normal.x();
        normals[ 3*i1 + 1 ] += face_normal.y();
        normals[ 3*i1 + 2 ] += face_normal.z();


        normals[ 3*i2 ] += face_normal.x();
        normals[ 3*i2 + 1 ] += face_normal.y();
        normals[ 3*i2 + 2 ] += face_normal.z();


        normals[ 3*i3 ] += face_normal.x();
        normals[ 3*i3 + 1 ] += face_normal.y();
        normals[ 3*i3 + 2 ] += face_normal.z();

    }

    return normals;

}



void Object::removeSurface()
{
    surface_vertices.clear();
    surface_faces.clear();
    surface_normals.clear();

    is_editable = true;
    is_test = false;
}


void Object::clear()
{
    removeSurface();
    removeCrossSections();
    removeTrajectoryCurve();

    setDefaultValues();
}


void Object::setDefaultValues()
{
    index = count_objects;

    type = Type::STRATIGRAPHY;
    name = std::string( "Surface " ) + std::to_string( (int) index );
    has_trajectory = false;

    is_selectable = false;
    is_selected = false;
    is_visible = true;
    is_editable = true;
    is_test = false;
}


