#include "controller_refactored.h"


#include "Object_Tree/ObjectTree.h"
#include "Sketching/csectionscene.h"
#include "Sketching/topviewscene.h"
#include "3dView/scene3d_refactored.h"
#include "3dView/Model3DUtils.hpp"

#include "./Core/Geology/Models/Volume.h"


Controller_Refactored::Controller_Refactored()
{
    volume = nullptr;
    csection_scene = nullptr;
    topview_scene = nullptr;
    scene3d = nullptr;
    object_tree = nullptr;

    setDefaultValues();

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
    updateVolume();
}


void Controller_Refactored::setVolumeHeight( double height )
{
    volume->setHeight( height );
    updateVolume();
}


void Controller_Refactored::setVolumeDepth( double depth )
{
    volume->setDepth( depth );
    updateVolume();
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
    updateVolume();
}


bool Controller_Refactored::getVolumeVisibility() const
{
    return volume->getVisibility();
}


void Controller_Refactored::updateVolume()
{

    csection_scene->updateVolume();
    scene3d->updateVolume();
    topview_scene->updateVolume();

    updateBoundingBoxRulesProcessor();


}


bool Controller_Refactored::isVolumeResizable() const
{

    for( auto &it: objects )
    {
        Object_Refactored* const& obj = ( it.second );
        if( obj->isEmpty() == false )
            return false;
    }

    return true;
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


    enableCurve( true );
    enableTrajectory( true );
    enableDeletingCurves( false );

}


void Controller_Refactored::addObject( std::size_t id )
{
    Object_Refactored* const& obj = new Object_Refactored();

    int r = std::get<0>( current_color );
    int g = std::get<1>( current_color );
    int b = std::get<2>( current_color );

    obj->setColor( r, g, b );

    obj->setId( id );
    objects[ id ] = obj;
    current_object = id;

    enableDeletingCurves( false );

}


void Controller_Refactored::addObjectToInterface()
{
    if ( isValidObject( current_object ) == false ) return;

    csection_scene->removeObjectTest();

    Object_Refactored* const& obj = objects[ current_object ];
    csection_scene->addObject( obj );
    scene3d->addObject( obj );

    int r, g, b;
    obj->getColor( r, g, b );
    object_tree->addObject( obj->getId(), r, g, b );


}


bool Controller_Refactored::isValidObject( std::size_t id ) const
{
    auto search = objects.find( id );
    if( search != objects.end() )
        return true;
    else
        return false;
}


void Controller_Refactored::setObjectType( const Object_Refactored::Type& type )
{
    setObjectType( current_object, type );
}


void Controller_Refactored::setObjectType( std::size_t id, const Object_Refactored::Type& type )
{
    if( isValidObject( id ) == false ) return;
    Object_Refactored* const& object = objects[ id ];
    object->setType( type );
}


Object_Refactored::Type Controller_Refactored::getObjectType()
{
    return getObjectType( current_object );
}


Object_Refactored::Type Controller_Refactored::getObjectType( std::size_t id )
{
    if( isValidObject( id ) == false ) return Object_Refactored::Type::NONE;
    return objects[ id ]->getType();
}


void Controller_Refactored::setObjectName( const std::string& name )
{
    setObjectName( current_object, name ) ;
}


void Controller_Refactored::setObjectName( std::size_t id, const std::string& name )
{
    if( isValidObject( id ) == false ) return;
    Object_Refactored* const& object = objects[ id ];
    object->setName( name );
}


std::string Controller_Refactored::getObjectName()
{
    return getObjectName( current_object );
}


std::string Controller_Refactored::getObjectName( std::size_t id )
{
    if( isValidObject( id ) == false ) return std::string();
    return objects[ id ]->getName();
}


void Controller_Refactored::setObjectColor( int r, int g, int b )
{
    setObjectColor( current_object, r, g, b );
}


void Controller_Refactored::getObjectColor( int& r, int& g, int& b )
{
    getObjectColor( current_object, r, g, b );
}


void Controller_Refactored::setObjectColor( std::size_t id, int r, int g, int b )
{
    if( isValidObject( id ) == false ) return;
    Object_Refactored* const& object = objects[ id ];
    object->setColor( r, g, b );

    object_tree->updateObjectColor( id, r, g, b );
    updateObject( id );
}


void Controller_Refactored::getObjectColor( std::size_t id, int& r, int& g, int& b )
{
    if( isValidObject( id ) == false ) return;
    objects[ id ]->getColor( r, g, b );
}


void Controller_Refactored::setObjectVisibility( bool status )
{
    setObjectVisibility( current_object, status );
}


bool Controller_Refactored::getObjectVisibility()
{
    return getObjectVisibility( current_object );
}


void Controller_Refactored::setObjectVisibility( std::size_t id,bool status )
{
    if( isValidObject( id ) == false ) return;
    Object_Refactored* const& object = objects[ id ];
    object->setVisibility( status );

    updateObject( id );
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


    bool status = object->isCurveAdmissible();
    enableCurve( status );

    status = object->isTrajectoryAdmissible();
    enableTrajectory( status );


    enableDeletingCurves( true );


    topview_scene->addCrossSection( current_csection );
    addObjectToInterface();

    testObjectSurface();

}


bool Controller_Refactored::testObjectSurface()
{
    if( isValidObject( current_object ) == false ) return false;


    Object_Refactored* const& obj = objects[ current_object ];
    std::vector< std::tuple< Curve2D, double > > curves = obj->getCrossSectionCurves();
    if( curves.empty() == true ) return false;


    bool surface_created = rules_processor.testSurface( current_object, curves );


    if( surface_created == true )
    {

        std::vector< double > surface_vertices;
        std::vector< std::size_t > surface_faces;
        bool has_surface = rules_processor.getMesh( current_object, surface_vertices, surface_faces );
        if( has_surface  == false ) return false;


        std::vector< double > surface_normals;
        rules_processor.getNormals( current_object, surface_normals );

        obj->setSurface( surface_vertices, surface_faces, true );
        obj->setSurfaceNormals( surface_normals );
        obj->setVisibility( true );

        scene3d->updateObject( current_object );
    }

    return surface_created;
}


void Controller_Refactored::removeCurveFromObject( double depth )
{
    Object_Refactored* const& object = objects[ current_object ];
    object->removeCrossSectionCurve( depth );


    bool status = object->isCurveAdmissible();
    enableCurve( status );

    status = object->isTrajectoryAdmissible();
    enableTrajectory( status );

    testObjectSurface();


}


void Controller_Refactored::removeTrajectoryFromObject()
{
    Object_Refactored* const& object = objects[ current_object ];
    object->removeTrajectoryCurve();


    bool status = object->isCurveAdmissible();
    enableCurve( status );

    status = object->isTrajectoryAdmissible();
    enableTrajectory( status );

}



void Controller_Refactored::removeCurrentObject()
{
    object_tree->removeObject( current_object );
    delete objects[ current_object ];
    objects.erase( current_object );
}


void Controller_Refactored::enableTrajectory( bool status )
{
    topview_scene->enableSketch( status );
}


void Controller_Refactored::enableCurve( bool status )
{
    csection_scene->enableSketch( status );
}


void Controller_Refactored::enableDeletingCurves( bool status  )
{
    csection_scene->enableDeletingCurves( status );
    topview_scene->enableDeletingCurves( status );
}


void Controller_Refactored::addTrajectoryToObject( const Curve2D& curve )
{
    Object_Refactored* const& object = objects[ current_object ];
    object->setTrajectoryCurve( curve );
    topview_scene->addObject( object );

    bool status = object->isCurveAdmissible();
    enableCurve( status );

    status = object->isTrajectoryAdmissible();
    enableTrajectory( status );
}


bool Controller_Refactored::createObjectSurface()
{
    if( isValidObject( current_object ) == false ) return false;


    Object_Refactored* const& obj = objects[ current_object ];
    obj->setTesting( false );

    std::vector< std::tuple< Curve2D, double > > curves = obj->getCrossSectionCurves();
    if( curves.empty() == true ) return false;


    setCurrentRule( current_rule );


    bool surface_created;
    if( obj->hasTrajectoryCurve() == true )
    {
        Curve2D path = obj->getTrajectoryCurve();
        Curve2D curve = std::get<0>( curves[ 0 ] );
        double depth = std::get<1>( curves[ 0 ] );

        surface_created = rules_processor.extrudeAlongPath( obj->getId(), curve, depth,
                                                           path );
    }

    else
        surface_created = rules_processor.createSurface( current_object, curves );


    if( surface_created == true )
    {
        updateActiveObjects();
        addObject();
    }

    return surface_created;
}


void Controller_Refactored::desactiveObjects()
{
    for( auto it: objects )
    {
        Object_Refactored* obj = objects[ it.first ];
        obj->setVisibility( false );

        object_tree->setObjectHidden( it.first, true );
        updateObject( it.first );
    }


    if( isValidObject( current_object ) == true )
        objects[ current_object ]->setVisibility( true );

    csection_scene->removeObjectsFromScene();
}


void Controller_Refactored::updateObject( std::size_t id )
{
    csection_scene->updateObject( id );
    scene3d->updateObject( id );
}


void Controller_Refactored::updateActiveObjects()
{
    desactiveObjects();

    std::vector< std::size_t > actives = rules_processor.getSurfaces();

    for( auto it: actives )
    {
        if( isValidObject( it ) == false ) continue;

        bool has_surface = updateActiveSurface( it );
        if( has_surface == false ) continue;

        bool has_curve = updateActiveCurve( it );
        if( has_curve == true )
            showObjectInCrossSection( it );
        else
            std::cout << "there is not curve in this cross-section\n" << std::flush;

        object_tree->setObjectHidden( it, false );

    }

}


bool Controller_Refactored::updateActiveCurve( std::size_t id )
{

    Object_Refactored* obj = objects[ id ];


    std::vector< double > curve_vertices;
    std::vector< std::size_t > curve_edges;
    bool has_curve = rules_processor.getCrossSection( id, indexCrossSection( current_csection ) ,
                                                      curve_vertices, curve_edges );


    if( has_curve == false ) return false;


    bool testing = obj->isTesting();
    bool has_crosssection = obj->hasCurve( current_csection );


    if( ( testing == false ) || ( has_crosssection == true ) )
        obj->setCrossSectionCurve( current_csection, Model3DUtils::convertToCurve2D( curve_vertices ),
                                   curve_edges );
    else if( testing == true )
        csection_scene->addObjectTest( curve_vertices, curve_edges );


    return true;

}


bool Controller_Refactored::updateActiveSurface( std::size_t id )
{


    Object_Refactored* obj = objects[ id ];
    bool testing = obj->isTesting();

    std::vector< double > surface_vertices;
    std::vector< std::size_t > surface_faces;

    bool has_surface = rules_processor.getMesh( id, surface_vertices, surface_faces );
    if( has_surface  == false ) return false;


    std::vector< double > surface_normals;
    rules_processor.getNormals( id, surface_normals );

    obj->setSurface( surface_vertices, surface_faces, testing );
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
    if ( isValidCrossSection( depth ) == false ) return;

    current_csection = depth;
    csection_scene->setCurrentCrossSection( current_csection );
    topview_scene->moveCurrentCrossSection( current_csection );
    scene3d->moveCrossSection( current_csection );

    if ( objects.empty() == false )
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


std::size_t Controller_Refactored::setupCrossSectionDiscretization()
{
    std::size_t discretization = rules_processor.getDepthResolution();
    csection_step = static_cast< double >( volume->getDepth()/discretization );
    return discretization;
}


std::size_t Controller_Refactored::indexCrossSection( double value ) const
{
    return static_cast< std::size_t > ( value/csection_step );
}


double Controller_Refactored::depthCrossSection( std::size_t index ) const
{
    return static_cast< double > ( index*csection_step );
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
    rules_processor.removeAboveIntersection();
}


void Controller_Refactored::updateBoundingBoxRulesProcessor()
{
    double ox = 0.0f, oy = 0.0f, oz = 0.0f;
    volume->getOrigin( ox, oy, oz );

    rules_processor.setOrigin( ox, oy, oz );
    rules_processor.setLenght( volume->getWidth(), volume->getHeight(), volume->getDepth() );
    rules_processor.setMediumResolution();
}




Controller_Refactored::StratigraphicRules Controller_Refactored::getCurrentRule() const
{
    return current_rule;
}


void Controller_Refactored::setCurrentRule( const Controller_Refactored::StratigraphicRules& rule )
{

    current_rule = rule;

    if( rule == Controller_Refactored::StratigraphicRules::REMOVE_ABOVE )
    {
        rules_processor.removeAbove();
    }
    else if( rule == Controller_Refactored::StratigraphicRules::REMOVE_ABOVE_INTERSECTION )
    {
        rules_processor.removeAboveIntersection();
    }
    else if( rule == Controller_Refactored::StratigraphicRules::REMOVE_BELOW )
    {
        rules_processor.removeBelow();
    }
    else if( rule == Controller_Refactored::StratigraphicRules::REMOVE_BELOW_INTERSECTION )
    {
        rules_processor.removeBelowIntersection();
    }

}



void Controller_Refactored::enableCreateAbove( bool status )
{
    setObjectsAsSelectable( selectable_upper, false );


    if( status == false )
        stopCreateAbove();
    else
        requestCreateAbove();

}


void Controller_Refactored::stopCreateAbove()
{
    rules_processor.stopDefineAbove();
    setObjectSelected( boundering_above, false );

}


void Controller_Refactored::requestCreateAbove()
{
    bool request_accept = rules_processor.requestCreateAbove( selectable_upper );
    if( request_accept == false ) return;

    current_region = RequestRegion::ABOVE;

    setObjectsAsSelectable( selectable_below, false );
    setObjectsAsSelectable( selectable_upper, true );

    csection_scene->setModeSelecting();
}



void Controller_Refactored::enableCreateBelow( bool status )
{

    setObjectsAsSelectable( selectable_below, false );

    if( status == false )
        stopCreateBelow();
    else
        requestCreateBelow();

}


void Controller_Refactored::stopCreateBelow()
{
    rules_processor.stopDefineBelow();
    setObjectSelected( boundering_below, false );

}


void Controller_Refactored::requestCreateBelow()
{

    bool request_accept = rules_processor.requestCreateBelow( selectable_below );
    if( request_accept == false ) return;

    current_region = RequestRegion::BELOW;

    setObjectsAsSelectable( selectable_upper, false );
    setObjectsAsSelectable( selectable_below, true );

    csection_scene->setModeSelecting();
}



void Controller_Refactored::setObjectsAsSelectable( std::vector< std::size_t >& indexes,
                                                    bool status )
{

    for( std::size_t id: indexes )
    {
        if( isValidObject( id ) == false ) continue;

        Object_Refactored* const& obj = objects[ id ];
        obj->setSelectable( status );

        updateObject( id );
    }

    if( status == false )
        indexes.clear();
}


void Controller_Refactored::setObjectSelected( std::size_t id, bool status )
{

    if( isValidObject( id ) == false ) return;

    Object_Refactored* const& obj = objects[ id ];
    obj->setSelected( status );
    updateObject( id );

    csection_scene->setModeSketching();
}


void Controller_Refactored::defineObjectSelected( std::size_t id )
{
    if( isValidObject( id ) == false ) return;

    if( current_region == RequestRegion::ABOVE )
    {
        boundering_above = id;
        rules_processor.defineAbove( id );
        setObjectsAsSelectable( selectable_upper, false );
    }

    else if( current_region == RequestRegion::BELOW )
    {
        boundering_below = id;
        rules_processor.defineBelow( id );
        setObjectsAsSelectable( selectable_below, false );
    }

    setObjectSelected( id, true);

}



void Controller_Refactored::saveFile( const std::string& filename )
{
    rules_processor.saveFile( filename );
}

void Controller_Refactored::loadFile( const std::string& filename )
{
    rules_processor.loadFile( filename );
    loadObjects();
//    loadStatus();
}


void Controller_Refactored::loadObjects()
{
    double ox, oy, oz;
    double width, height, depth;

    rules_processor.getOrigin( ox, oy, oz );
    rules_processor.getLenght( width, height, depth );


    volume->setOrigin( ox, oy, oz );
    volume->setDimensions( width, height, depth );
    updateVolume();


    if( isValidObject( current_object ) == true )
        removeCurrentObject();


    std::vector< std::size_t > actives = rules_processor.getSurfaces();
    std::size_t number_of_objects = actives.size();
    std::vector< int > colors = createVectorOfColors( number_of_objects );

    std::size_t i = 0;
    for( auto id: actives )
    {
        addObject( id );
        setObjectColor( id, colors[ 3*i ], colors[ 3*i + 1 ], colors[ 3*i + 2 ] );
        addObjectToInterface();
        ++i;
    }

    addObject();
    updateActiveObjects();

}


std::vector< int > Controller_Refactored::createVectorOfColors( std::size_t number_of_colors )
{

    std::vector< int > colors;
    colors.resize( 3*number_of_colors );

    for( std::size_t i = 0; i < number_of_colors; ++i )
    {
        std::random_device rd;
        std::mt19937 eng( rd() );
        std::uniform_int_distribution< size_t > distr( 0, 255 );
        colors[ 3*i ] = distr( eng );
        colors[ 3*i + 1 ] = distr( eng );
        colors[ 3*i + 2 ] = distr( eng );
    }

    return colors;
}

void Controller_Refactored::loadStatus()
{
    isDefineAboveActive();
    isDefineBelowActive();
}


bool Controller_Refactored::undo()
{
    bool undo_done = rules_processor.undo();
    if( undo_done == false ) return false;

    updateActiveObjects();
    return true;
}

bool Controller_Refactored::redo()
{
    bool redo_done = rules_processor.redo();
    if( redo_done == false ) return false;

    updateActiveObjects();
    return true;
}


bool Controller_Refactored::canUndo()
{
    return rules_processor.canUndo();
}

bool Controller_Refactored::canRedo()
{
    return rules_processor.canRedo();
}


bool Controller_Refactored::isDefineAboveActive()
{
    std::size_t id;

    bool status = rules_processor.defineAboveIsActive( id );
    if( status == false ) return false;

    current_region = RequestRegion::ABOVE;
    defineObjectSelected( id );
    return true;

}



bool Controller_Refactored::isDefineBelowActive()
{
    std::size_t id;

    bool status = rules_processor.defineBelowIsActive( id );
    if( status == false ) return false;

    current_region = RequestRegion::BELOW;
    defineObjectSelected( id );
    return true;
}


void Controller_Refactored::clear()
{
    clearScenes();
    clearData();

    setDefaultValues();
}


void Controller_Refactored::clearScenes()
{
    if( scene3d != nullptr )
        scene3d->clear();

    if( csection_scene != nullptr )
        csection_scene->clear();

    if( topview_scene != nullptr )
        topview_scene->clear();
}


void Controller_Refactored::clearData()
{
    if( volume != nullptr )
    {
        volume->clear();
        delete volume;
    }
    volume = nullptr;



    for( auto &it: objects )
    {
        ( it.second )->clear();
        delete ( it.second );
    }
    objects.clear();


    if( object_tree != nullptr )
        object_tree->clear();


    selectable_upper.clear();
    selectable_below.clear();

    rules_processor.clear();
}


void Controller_Refactored::setDefaultValues()
{
    csection_step = 0.0;
    boundering_above = 0;
    boundering_below = 0;

    setCurrentColor( 255, 0, 0 );
    current_object = 0;
    current_csection = 0.0;
    current_rule = RULE_DEFAULT;
    current_region = RequestRegion::NONE;
}


