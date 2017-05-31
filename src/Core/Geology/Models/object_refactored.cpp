#include "object_refactored.h"


Object_Refactored::Object_Refactored()
{
    setDefaultValues();
    ++count_objects;
}


Object_Refactored::Object_Refactored( const Object_Refactored::Type& t )
{
    setDefaultValues();
    type = t;

    ++count_objects;
}


std::size_t Object_Refactored::getId() const
{
    return index;
}

void Object_Refactored::setType( const Object_Refactored::Type& t )
{
    type = t;
}


Object_Refactored::Type Object_Refactored::getType() const
{
    return type;
}


void Object_Refactored::setName( const std::string& n )
{
    name = n;
}

std::string Object_Refactored::getName() const
{
    return name;
}


void Object_Refactored::setColor( int r, int g, int b )
{
    color = std::make_tuple( r, g, b );
}


void Object_Refactored::getColor( int& r, int& g, int& b ) const
{
    r = std::get<0>( color );
    g = std::get<1>( color );
    b = std::get<2>( color );
}


void Object_Refactored::setEditable( bool status )
{
    is_editable = status;
}


bool Object_Refactored::getEditable() const
{
    return is_editable;
}



void Object_Refactored::setSelectable( bool status )
{
    is_selectable = status;
}


bool Object_Refactored::getSelectable() const
{
    return is_selectable;
}



void Object_Refactored::setSelected( bool status )
{
    is_selected = status;
}


bool Object_Refactored::getSelected() const
{
    return is_selected;
}


void Object_Refactored::setVisibility( bool status )
{
    is_visible = status;
}


bool Object_Refactored::getVisibility() const
{
    return is_visible;
}


bool Object_Refactored::isEmpty() const
{
    bool empty = csections_curves.empty() && csections_edges.empty() &&
                 ( !has_trajectory );
    return empty;
}


void Object_Refactored::setCrossSectionCurve( double depth, const Curve2D& curve,
                           std::vector< std::size_t > edges )
{
    csections_curves[ depth ] = curve;
    csections_edges[ depth ] = edges;
}


Curve2D Object_Refactored::getCrossSectionCurve( double depth )
{
    return csections_curves[ depth ];
}


std::vector< std::tuple< Curve2D, double > > Object_Refactored::getCrossSectionCurves()
{
    std::vector< std::tuple< Curve2D, double > > curves;
    for( auto it: csections_curves )
    {
        curves.push_back( std::make_tuple( it.second, (std::size_t)it.first ) );
    }

    return curves;
}


std::vector< std::size_t > Object_Refactored::getCrossSectionCurveEdges( double depth )
{
    return csections_edges[ depth ];
}


void Object_Refactored::removeCrossSectionCurve( double depth )
{
    csections_curves[ depth ].clear();
    csections_curves.erase( depth );

    csections_edges[ depth ].clear();
    csections_edges.erase( depth );
}


void Object_Refactored::removeCrossSections()
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


bool Object_Refactored::hasTrajectoryCurve()
{
    return has_trajectory;
}


void Object_Refactored::setTrajectoryCurve( const Curve2D& path )
{
    trajectory_curve = path;
}


Curve2D Object_Refactored::getTrajectoryCurve() const
{
    return trajectory_curve;
}


void Object_Refactored::removeTrajectoryCurve()
{
    trajectory_curve.clear();
    has_trajectory = false;
}


void Object_Refactored::setSurface( const std::vector< double >& vertices,
                                    const std::vector< std::size_t >& faces )
{
    removeSurface();
    surface_vertices.assign( vertices.begin(), vertices.end() );
    surface_faces.assign( faces.begin(), faces.end() );

    is_editable = false;
}


void Object_Refactored::setSurfaceNormals( const std::vector< double >& normals )
{
    surface_normals.clear();
    surface_normals.assign( normals.begin(), normals.end() );
}


void Object_Refactored::removeSurface()
{
    surface_vertices.clear();
    surface_faces.clear();
    surface_normals.clear();

    is_editable = true;
}


void Object_Refactored::clear()
{
    removeSurface();
    removeCrossSections();
    removeTrajectoryCurve();

    setDefaultValues();
}


void Object_Refactored::setDefaultValues()
{
    index = count_objects;

    type = Type::STRATIGRAPHY;
    name = std::string( "Surface " ) + std::to_string( (int) index );
    has_trajectory = false;

    is_selectable = false;
    is_selected = false;
    is_visible = true;
    is_editable = true;
}


