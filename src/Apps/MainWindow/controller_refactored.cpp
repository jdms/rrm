#include "controller_refactored.h"


#include "Object_Tree/ObjectTree.h"
#include "Sketching/csectionscene.h"
#include "Sketching/topviewscene.h"
#include "3dView/scene3d_refactored.h"
#include "3dView/Model3DUtils.hpp"

#include "./Core/Geology/Models/Volume.h"


Controller_Refactored::Controller_Refactored()
{
    current_csection = 0;
    volume = nullptr;
    csection_scene = nullptr;
    topview_scene = nullptr;
    scene3d = nullptr;
    object_tree = nullptr;

}


void Controller_Refactored::setScene3d( Scene3d_refactored* const& sc )
{
    scene3d = sc;
}


void Controller_Refactored::setCSectionScene( CSectionScene* const& sc )
{
    csection_scene = sc;
}


void Controller_Refactored::setTopViewScene( TopViewScene* const& sc )
{
    topview_scene = sc;
}


void Controller_Refactored::setObjectTree( ObjectTree* const& ot )
{
    object_tree = ot;
}


void Controller_Refactored::init()
{
    addVolume();
    addVolumeToInterface();

    setCurrentCrossSection( volume->getDepth() );
    addObject();
}


void Controller_Refactored::addVolume()
{
    if( volume != nullptr )
        delete volume;

    volume = new Volume();
    initRulesProcessor();

}


void Controller_Refactored::addVolumeToInterface()
{
    scene3d->addVolume( volume );
    csection_scene->addVolume( volume );
    topview_scene->addVolume( volume );

    object_tree->addInputVolume();
}


void Controller_Refactored::setVolumeWidth( double width )
{
    volume->setWidth( width );
}


void Controller_Refactored::setVolumeHeight( double height )
{
    volume->setHeight( height );
}


void Controller_Refactored::setVolumeDepth( double depth )
{
    volume->setDepth( depth );
}


double Controller_Refactored::getVolumeWidth() const
{
    return volume->getWidth();
}


double Controller_Refactored::getVolumeHeight() const
{
    return volume->getHeight();
}


double Controller_Refactored::getVolumeDepth() const
{
    return volume->getDepth();
}


void Controller_Refactored::setVolumeVisibility( bool status )
{
    volume->setVisibility( status );
}


bool Controller_Refactored::getVolumeVisibility() const
{
    return volume->getVisibility();
}


void Controller_Refactored::addObject()
{
    Object_Refactored* const& obj = new Object_Refactored();

    int r = std::get<0>( current_color );
    int g = std::get<1>( current_color );
    int b = std::get<2>( current_color );

    obj->setColor( r, g, b );

    objects[ obj->getId() ] = obj;
    current_object = obj->getId();
}


void Controller_Refactored::addObjectToInterface()
{
    if ( isValidObject( current_object ) == false ) return;

    Object_Refactored* const& obj = objects[ current_object ];
    csection_scene->addObject( obj );
    scene3d->addObject( obj );

}


bool Controller_Refactored::isValidObject( std::size_t id ) const
{
    auto search = objects.find( id );
    if( search != objects.end() )
        return true;
    else
        return false;
}


void Controller_Refactored::setObjectType( std::size_t id, const Object_Refactored::Type& type )
{
    if( isValidObject( id ) == false ) return;
    Object_Refactored* const& object = objects[ id ];
    object->setType( type );
}


Object_Refactored::Type Controller_Refactored::getObjectType( std::size_t id )
{
    if( isValidObject( id ) == false ) return Object_Refactored::Type::NONE;
    return objects[ id ]->getType();
}


void Controller_Refactored::setObjectName( std::size_t id, const std::string& name )
{
    if( isValidObject( id ) == false ) return;
    Object_Refactored* const& object = objects[ id ];
    object->setName( name );
}


std::string Controller_Refactored::getObjectName( std::size_t id )
{
    if( isValidObject( id ) == false ) return std::string();
    return objects[ id ]->getName();
}


void Controller_Refactored::setObjectColor( std::size_t id, int r, int g, int b )
{
    if( isValidObject( id ) == false ) return;
    Object_Refactored* const& object = objects[ id ];
    object->setColor( r, g, b );
}


void Controller_Refactored::getObjectColor( std::size_t id, int& r, int& g, int& b )
{
    if( isValidObject( id ) == false ) return;
    objects[ id ]->getColor( r, g, b );
}


void Controller_Refactored::setObjectVisibility( std::size_t id,bool status )
{
    if( isValidObject( id ) == false ) return;
    Object_Refactored* const& object = objects[ id ];
    object->setVisibility( status );
}


bool Controller_Refactored::getObjectVisibility( std::size_t id )
{
    if( isValidObject( id ) == false ) return false;
    return objects[ id ]->getVisibility();
}


void Controller_Refactored::addCurveToObject( const Curve2D& curve )
{
    Object_Refactored* const& object = objects[ current_object ];
    object->setCrossSectionCurve( current_csection, curve );

//    setCrossSectionAsUsed();
    addObjectToInterface();

    createObjectSurface();

}


void Controller_Refactored::removeCurveFromObject( double depth )
{
    Object_Refactored* const& object = objects[ current_object ];
    object->removeCrossSectionCurve( depth );
}


void Controller_Refactored::addTrajectoryToObject( const Curve2D& curve )
{
    Object_Refactored* const& object = objects[ current_object ];
    object->setTrajectoryCurve( curve );

}


bool Controller_Refactored::createObjectSurface()
{
    if( isValidObject( current_object ) == false ) return false;

    Object_Refactored* const& obj = objects[ current_object ];
    std::vector< std::tuple< Curve2D, double > > curves = obj->getCrossSectionCurves();

    if( curves.empty() == true ) return false;

    rules_processor.createSurface( current_object, curves );
    updateActiveObjects();

    return true;
}


void Controller_Refactored::desactiveObjects()
{
    for( auto it: objects )
    {
        Object_Refactored* obj = objects[ it.first ];
        obj->setVisibility( false );

        // TODO: set visibility as false in object tree too
    }
}


void Controller_Refactored::updateActiveObjects()
{
    desactiveObjects();

    std::vector< std::size_t > actives = rules_processor.getSurfaces();
    actives.push_back( current_object );

    for( auto it: actives )
    {

        bool has_surface = updateActiveSurface( it );
//        if( has_surface == true )
//            showObjectInCrossSection( it );
//        else
//            std::cout << "there is not curve in this cross-section\n" << std::flush;


        bool has_curve = updateActiveCurve( it );
        if( has_curve == true )
            showObjectInCrossSection( it );
        else
            std::cout << "there is not curve in this cross-section\n" << std::flush;
    }


}


bool Controller_Refactored::updateActiveCurve( std::size_t id )
{
    Object_Refactored* obj = objects[ id ];

    std::vector< double > curve_vertices;
    std::vector< std::size_t > curve_edges;
    bool has_curve = rules_processor.getCrossSection( id, 0/*index_*/ , curve_vertices,
                                                      curve_edges );


    if( has_curve == false ) return false;


    obj->setCrossSectionCurve( current_csection, Model3DUtils::convertToCurve2D( curve_vertices ),
                               curve_edges );


    return true;


}


bool Controller_Refactored::updateActiveSurface( std::size_t id )
{
    Object_Refactored* obj = objects[ id ];

    std::vector< double > surface_vertices;
    std::vector< std::size_t > surface_faces;

    bool has_surface = rules_processor.getMesh( id, surface_vertices, surface_faces );
    if( has_surface  == false ) return false;


    std::vector< double > surface_normals;
    rules_processor.getNormals( id, surface_normals );

    obj->setSurface( surface_vertices, surface_faces );
    obj->setSurfaceNormals( surface_normals );
    obj->setVisibility( true );

    scene3d->updateObject( id );

    return true;

}


void Controller_Refactored::showObjectInCrossSection( std::size_t id )
{
    csection_scene->reActiveObject( id );
}




void Controller_Refactored::setCurrentCrossSection( double depth )
{
    if( ( isValidCrossSection( depth ) == false ) || ( objects.empty() == true ) ) return;

    current_csection = depth;
    csection_scene->setCurrentCrossSection( current_csection );


    updateActiveObjects();

}


double Controller_Refactored::getCurrentCrossSection() const
{
    return current_csection;
}


bool Controller_Refactored::isValidCrossSection( double depth ) const
{
    double ox = 0.0f, oy = 0.0f, oz = 0.0f;
    volume->getOrigin( ox, oy, oz );

    if( std::fabs(  depth - oz ) <= volume->getDepth() )
        return true;
    else
        return false;
}




void Controller_Refactored::setCurrentColor( int r, int g, int b )
{
    current_color = std::make_tuple( r, g, b );

    if( csection_scene != nullptr )
        csection_scene->setCurrentColor( r, g, b );

    if( topview_scene != nullptr )
        topview_scene->setCurrentColor( r, g, b );

    setObjectColor( r, g, b );
}


void Controller_Refactored::getCurrentColor( int& r, int& g, int& b ) const
{
    r = std::get<0>( current_color );
    g = std::get<1>( current_color );
    b = std::get<2>( current_color );
}


void Controller_Refactored::initRulesProcessor()
{
    updateBoundingBoxRulesProcessor();
    rules_processor.setMediumResolution();
    rules_processor.removeAbove();
}


void Controller_Refactored::updateBoundingBoxRulesProcessor()
{
    double ox = 0.0f, oy = 0.0f, oz = 0.0f;
    volume->getOrigin( ox, oy, oz );

    rules_processor.setOrigin( ox, oy, oz );
    rules_processor.setLenght( volume->getWidth(), volume->getHeight(), volume->getDepth() );
}
