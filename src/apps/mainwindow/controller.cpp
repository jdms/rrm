#include "controller.h"


#include "object_tree/object_tree.h"
#include "sketching/csection_scene.h"
#include "sketching/topview_scene.h"
#include "3dview/scene3d.h"
#include "3dview/model3d_utils.hpp"

#include "./core/geology/models/volume.h"


Controller::Controller()
{
    volume = nullptr;
    csection_scene = nullptr;
    topview_scene = nullptr;
    scene3d = nullptr;
    object_tree = nullptr;

    setDefaultValues();

}




void Controller::setScene3d( Scene3d* const& sc )
{
    scene3d = sc;
}


void Controller::setCSectionScene( CSectionScene* const& sc )
{
    csection_scene = sc;
}


void Controller::setTopViewScene( TopViewScene* const& sc )
{
    topview_scene = sc;
}


void Controller::setObjectTree( ObjectTree* const& ot )
{
    object_tree = ot;
}




void Controller::init()
{
    addVolume();
    addVolumeToInterface();

    setCurrentCrossSection( volume->getDepth() );
    addObject();
}




void Controller::addVolume()
{
    if( volume != nullptr )
        delete volume;

    volume = new Volume();
    initRulesProcessor();

}


void Controller::addVolumeToInterface()
{
    scene3d->addVolume( volume );
    csection_scene->addVolume( volume );
    topview_scene->addVolume( volume );

    object_tree->addInputVolume();
}


void Controller::setVolumeWidth( double width )
{
    if( volume == nullptr ) return;
    volume->setWidth( width );
    updateVolume();
}


void Controller::setVolumeHeight( double height )
{
    if( volume == nullptr ) return;
    volume->setHeight( height );
    updateVolume();
}


void Controller::setVolumeDepth( double depth )
{
    if( volume == nullptr ) return;
    volume->setDepth( depth );
    updateVolume();
}


double Controller::getVolumeWidth() const
{
    if( volume == nullptr ) return 0.0;
    return volume->getWidth();
}


double Controller::getVolumeHeight() const
{
    if( volume == nullptr ) return 0.0;
    return volume->getHeight();
}


double Controller::getVolumeDepth() const
{
    if( volume == nullptr ) return 0.0;
    return volume->getDepth();
}


void Controller::setVolumeVisibility( bool status )
{
    if( volume == nullptr ) return;
    volume->setVisibility( status );
    updateVolume();
}


bool Controller::getVolumeVisibility() const
{
    if( volume == nullptr ) return false;
    return volume->getVisibility();
}


void Controller::updateVolume()
{

    csection_scene->updateVolume();
    scene3d->updateVolume();
    topview_scene->updateVolume();

    updateBoundingBoxRulesProcessor();


}


bool Controller::isVolumeResizable() const
{

    for( auto &it: objects )
    {
        Object* const& obj = ( it.second );
        if( obj->isEmpty() == false )
            return false;
    }

    return true;
}


void Controller::addObject()
{
    Object* const& obj = new Object();

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


void Controller::addObject( std::size_t id )
{
    Object* const& obj = new Object();

    int r = std::get<0>( current_color );
    int g = std::get<1>( current_color );
    int b = std::get<2>( current_color );

    obj->setColor( r, g, b );

    obj->setId( id );
    objects[ id ] = obj;
    current_object = id;

    enableDeletingCurves( false );

}


void Controller::addObjectToInterface()
{
    if ( isValidObject( current_object ) == false ) return;

    csection_scene->removeObjectTest();

    Object* const& obj = objects[ current_object ];
    csection_scene->addObject( obj );
    scene3d->addObject( obj );
    topview_scene->addObject( obj );

    int r, g, b;
    obj->getColor( r, g, b );
    object_tree->addObject( obj->getId(), r, g, b );


}


bool Controller::isValidObject( std::size_t id ) const
{
    auto search = objects.find( id );
    if( search != objects.end() )
        return true;
    else
        return false;
}


void Controller::setObjectType( const Object::Type& type )
{
    setObjectType( current_object, type );
}


void Controller::setObjectType( std::size_t id, const Object::Type& type )
{
    if( isValidObject( id ) == false ) return;
    Object* const& object = objects[ id ];
    object->setType( type );
}


Object::Type Controller::getObjectType()
{
    return getObjectType( current_object );
}


Object::Type Controller::getObjectType( std::size_t id )
{
    if( isValidObject( id ) == false ) return Object::Type::NONE;
    return objects[ id ]->getType();
}


void Controller::setObjectName( const std::string& name )
{
    setObjectName( current_object, name ) ;
}


void Controller::setObjectName( std::size_t id, const std::string& name )
{
    if( isValidObject( id ) == false ) return;
    Object* const& object = objects[ id ];
    object->setName( name );
}


std::string Controller::getObjectName()
{
    return getObjectName( current_object );
}


std::string Controller::getObjectName( std::size_t id )
{
    if( isValidObject( id ) == false ) return std::string();
    return objects[ id ]->getName();
}


void Controller::setObjectColor( int r, int g, int b )
{
    setObjectColor( current_object, r, g, b );
}


void Controller::getObjectColor( int& r, int& g, int& b )
{
    getObjectColor( current_object, r, g, b );
}


void Controller::setObjectColor( std::size_t id, int r, int g, int b )
{
    if( isValidObject( id ) == false ) return;
    Object* const& object = objects[ id ];
    object->setColor( r, g, b );

    object_tree->updateObjectColor( id, r, g, b );
    updateObject( id );
}


void Controller::getObjectColor( std::size_t id, int& r, int& g, int& b )
{
    if( isValidObject( id ) == false ) return;
    objects[ id ]->getColor( r, g, b );
}


void Controller::setObjectVisibility( bool status )
{
    setObjectVisibility( current_object, status );
}


bool Controller::getObjectVisibility()
{
    return getObjectVisibility( current_object );
}


void Controller::setObjectVisibility( std::size_t id,bool status )
{
    if( isValidObject( id ) == false ) return;
    Object* const& object = objects[ id ];
    object->setVisibility( status );

    updateObject( id );
}


bool Controller::getObjectVisibility( std::size_t id )
{
    if( isValidObject( id ) == false ) return false;
    return objects[ id ]->getVisibility();
}


void Controller::addCurveToObject( const Curve2D& curve )
{

    Object* const& object = objects[ current_object ];
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


bool Controller::testObjectSurface()
{
    if( isValidObject( current_object ) == false ) return false;


    Object* const& obj = objects[ current_object ];
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


void Controller::removeCurveFromObject( double depth )
{
    Object* const& object = objects[ current_object ];
    object->removeCrossSectionCurve( depth );


    bool status = object->isCurveAdmissible();
    enableCurve( status );

    status = object->isTrajectoryAdmissible();
    enableTrajectory( status );

    testObjectSurface();
    updateCrossSection();


}


void Controller::removeTrajectoryFromObject()
{
    Object* const& object = objects[ current_object ];
    object->removeTrajectoryCurve();


    bool status = object->isCurveAdmissible();
    enableCurve( status );

    status = object->isTrajectoryAdmissible();
    enableTrajectory( status );

}



void Controller::removeCurrentObject()
{
    object_tree->removeObject( current_object );
    delete objects[ current_object ];
    objects.erase( current_object );
}


void Controller::enableTrajectory( bool status )
{
    topview_scene->enableSketch( status );
}


void Controller::enableCurve( bool status )
{
    csection_scene->enableSketch( status );
}


void Controller::enableDeletingCurves( bool status  )
{
    csection_scene->enableDeletingCurves( status );
    topview_scene->enableDeletingCurves( status );
}


void Controller::addTrajectoryToObject( const Curve2D& curve )
{
    Object* const& object = objects[ current_object ];
    object->setTrajectoryCurve( curve );
    topview_scene->addObject( object );

    bool status = object->isCurveAdmissible();
    enableCurve( status );

    status = object->isTrajectoryAdmissible();
    enableTrajectory( status );
}


bool Controller::createObjectSurface()
{
    if( isValidObject( current_object ) == false ) return false;


    Object* const& obj = objects[ current_object ];
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
        obj->removeCrossSections();
        updateModel();
        addObject();
    }

    return surface_created;
}


void Controller::desactiveObjects()
{
    for( auto it: objects )
    {
        Object* obj = objects[ it.first ];
        obj->setVisibility( false );

        object_tree->setObjectHidden( it.first, true );
        updateObject( it.first );
    }


    if( isValidObject( current_object ) == true )
        objects[ current_object ]->setVisibility( true );


}


void Controller::updateObject( std::size_t id )
{
    csection_scene->updateObject( id );
    topview_scene->updateObject( id );
    scene3d->updateObject( id );
}


void Controller::updateActiveObjects()
{
    //deprecated

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


bool Controller::updateActiveCurve( std::size_t id )
{

    Object* obj = objects[ id ];


    std::vector< double > curve_vertices;
    std::vector< std::size_t > curve_edges;
    bool has_curve = rules_processor.getCrossSection( id, indexCrossSection( current_csection ) ,
                                                      curve_vertices, curve_edges );


    if( has_curve == false ) return false;


    bool testing = obj->isTesting();
    bool has_crosssection = obj->hasCurve( current_csection );


    if( testing == true )
    {
        if( has_crosssection == true ) return true;

        csection_scene->addObjectTest( curve_vertices, curve_edges );
    }
    else
    {
        obj->setCrossSectionCurve( current_csection, Model3DUtils::convertToCurve2D( curve_vertices ),
                                   curve_edges );
    }


    return true;



}


bool Controller::updateActiveSurface( std::size_t id )
{


    Object* obj = objects[ id ];
    bool testing = obj->isTesting();

    std::vector< double > surface_vertices;
    std::vector< std::size_t > surface_faces;

    bool has_surface = rules_processor.getMesh( id, surface_vertices, surface_faces );
    if( has_surface  == false ) return false;

    std::vector< double > surface_path;
    bool has_path = rules_processor.getExtrusionPath( id, surface_path );
    if( has_path == true )
        obj->setTrajectoryCurve( Model3DUtils::convertToCurve2D( surface_path ) );


    std::vector< double > surface_normals;
    rules_processor.getNormals( id, surface_normals );

    obj->setSurface( surface_vertices, surface_faces, testing );
    obj->setSurfaceNormals( surface_normals );
    obj->setVisibility( true );

    scene3d->updateObject( id );
    topview_scene->updateObject( id );
    return true;

}




void Controller::updateModel()
{

    std::vector< std::size_t > actives = rules_processor.getSurfaces();


    std::map< std::size_t, Object* >::iterator iterator_id = objects.begin();
    for( std::size_t i = 0; i < actives.size(); )
    {
        std::size_t id = actives[ i ];

        if( id == (*iterator_id).first )
        {
           activeObject( id );
            ++i;
            //active
        }
        else
        {
            //desactive
            desactiveObject( id );
        }

        ++iterator_id;

    }


    while ( iterator_id != objects.end() )
    {
        int ok = 0;
        desactiveObject( (*iterator_id).first );
        ++iterator_id;
        //desactive
    }

    updateCrossSection();
}



void Controller::activeObject( std::size_t id )
{
    Object* obj = objects[ id ];

    obj->setVisibility( true );
    object_tree->setObjectHidden( id, false );

    bool status = updateActiveSurface( id );
    if( status == false )
        desactiveObject( id );
}



void Controller::desactiveObject( std::size_t id )
{
    Object* obj = objects[ id ];
    obj->setVisibility( false );
    object_tree->setObjectHidden( id, true );
    updateObject( id );
}




void Controller::updateCrossSection()
{

    std::vector< std::size_t > actives = rules_processor.getSurfaces();

    for( std::size_t i = 0; i < actives.size(); ++i )
    {
        std::size_t id = actives[ i ];
        bool status = updateActiveCurve( id );

        if( status == true )
            csection_scene->reActiveObject( id );
        else
            std::cout << "There is no curve in this cross-section for the object "  << id
                      << std::endl << std::flush;

    }

}






void Controller::showObjectInCrossSection( std::size_t id )
{
    // deprecated
    csection_scene->reActiveObject( id );
}




void Controller::setCurrentCrossSection( double depth )
{
    if ( isValidCrossSection( depth ) == false ) return;

    current_csection = depth;
    csection_scene->setCurrentCrossSection( current_csection );
    topview_scene->moveCurrentCrossSection( current_csection );
    scene3d->moveCrossSection( current_csection );

    if ( objects.empty() == true ) return;
    updateCrossSection();


}


double Controller::getCurrentCrossSection() const
{
    return current_csection;
}


bool Controller::isValidCrossSection( double depth ) const
{
    if( volume == nullptr ) return false;

    double ox = 0.0f, oy = 0.0f, oz = 0.0f;
    volume->getOrigin( ox, oy, oz );

    if( std::fabs(  depth - oz ) <= volume->getDepth() )
        return true;
    else
        return false;
}


std::size_t Controller::setupCrossSectionDiscretization()
{
    if( volume == nullptr ) return 0;

    std::size_t discretization = rules_processor.getDepthResolution();
    csection_step = static_cast< double >( volume->getDepth()/discretization );
    return discretization;
}


std::size_t Controller::indexCrossSection( double value ) const
{
    return static_cast< std::size_t > ( value/csection_step );
}


double Controller::depthCrossSection( std::size_t index ) const
{
    return static_cast< double > ( index*csection_step );
}




void Controller::setCurrentColor( int r, int g, int b )
{
    current_color = std::make_tuple( r, g, b );

    if( csection_scene != nullptr )
        csection_scene->setCurrentColor( r, g, b );

    if( topview_scene != nullptr )
        topview_scene->setCurrentColor( r, g, b );

    setObjectColor( r, g, b );
}


void Controller::getCurrentColor( int& r, int& g, int& b ) const
{
    r = std::get<0>( current_color );
    g = std::get<1>( current_color );
    b = std::get<2>( current_color );
}




void Controller::initRulesProcessor()
{
    updateBoundingBoxRulesProcessor();
    rules_processor.removeAboveIntersection();
}


void Controller::updateBoundingBoxRulesProcessor()
{
    if( volume == nullptr ) return;

    double ox = 0.0f, oy = 0.0f, oz = 0.0f;
    volume->getOrigin( ox, oy, oz );

    rules_processor.setOrigin( ox, oy, oz );
    rules_processor.setLenght( volume->getWidth(), volume->getHeight(), volume->getDepth() );
    rules_processor.setMediumResolution();
}




Controller::StratigraphicRules Controller::getCurrentRule() const
{
    return current_rule;
}


void Controller::setCurrentRule( const Controller::StratigraphicRules& rule )
{

    current_rule = rule;

    if( rule == Controller::StratigraphicRules::REMOVE_ABOVE )
    {
        rules_processor.removeAbove();
    }
    else if( rule == Controller::StratigraphicRules::REMOVE_ABOVE_INTERSECTION )
    {
        rules_processor.removeAboveIntersection();
    }
    else if( rule == Controller::StratigraphicRules::REMOVE_BELOW )
    {
        rules_processor.removeBelow();
    }
    else if( rule == Controller::StratigraphicRules::REMOVE_BELOW_INTERSECTION )
    {
        rules_processor.removeBelowIntersection();
    }

}



void Controller::enableCreateAbove( bool status )
{
    setObjectsAsSelectable( selectable_upper, false );


    if( status == false )
        stopCreateAbove();
    else
        requestCreateAbove();

}


void Controller::stopCreateAbove()
{
    rules_processor.stopDefineAbove();
    setObjectSelected( boundering_above, false );

}


void Controller::requestCreateAbove()
{
    bool request_accept = rules_processor.requestCreateAbove( selectable_upper );
    if( request_accept == false ) return;

    current_region = RequestRegion::ABOVE;

    setObjectsAsSelectable( selectable_below, false );
    setObjectsAsSelectable( selectable_upper, true );

    csection_scene->setModeSelecting();
}



void Controller::enableCreateBelow( bool status )
{

    setObjectsAsSelectable( selectable_below, false );

    if( status == false )
        stopCreateBelow();
    else
        requestCreateBelow();

}


void Controller::stopCreateBelow()
{
    rules_processor.stopDefineBelow();
    setObjectSelected( boundering_below, false );

}


void Controller::requestCreateBelow()
{

    bool request_accept = rules_processor.requestCreateBelow( selectable_below );
    if( request_accept == false ) return;

    current_region = RequestRegion::BELOW;

    setObjectsAsSelectable( selectable_upper, false );
    setObjectsAsSelectable( selectable_below, true );

    csection_scene->setModeSelecting();
}



void Controller::setObjectsAsSelectable( std::vector< std::size_t >& indexes,
                                                    bool status )
{

    for( std::size_t id: indexes )
    {
        if( isValidObject( id ) == false ) continue;

        Object* const& obj = objects[ id ];
        obj->setSelectable( status );

        updateObject( id );
    }

    if( status == false )
        indexes.clear();
}


void Controller::setObjectSelected( std::size_t id, bool status )
{

    if( isValidObject( id ) == false ) return;

    Object* const& obj = objects[ id ];
    obj->setSelected( status );
    updateObject( id );

    csection_scene->setModeSketching();
}


void Controller::defineObjectSelected( std::size_t id )
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



void Controller::saveFile( const std::string& filename )
{
    rules_processor.saveFile( filename );
}

void Controller::loadFile( const std::string& filename )
{
    rules_processor.loadFile( filename );
    loadObjects();
}


void Controller::loadObjects()
{
    if( volume == nullptr ) return;

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
    updateModel();

}


std::vector< int > Controller::createVectorOfColors( std::size_t number_of_colors )
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

void Controller::loadStatus()
{
    isDefineAboveActive();
    isDefineBelowActive();
}


bool Controller::undo()
{
    bool undo_done = rules_processor.undo();
    if( undo_done == false ) return false;

    updateModel();
    return true;
}

bool Controller::redo()
{
    bool redo_done = rules_processor.redo();
    if( redo_done == false ) return false;

    updateActiveObjects();
    updateModel();
    return true;
}


bool Controller::canUndo()
{
    return rules_processor.canUndo();
}

bool Controller::canRedo()
{
    return rules_processor.canRedo();
}


bool Controller::isDefineAboveActive()
{
    std::size_t id;

    bool status = rules_processor.defineAboveIsActive( id );
    if( status == false )
    {
        setObjectSelected( boundering_above, false );
        return false;
    }

    current_region = RequestRegion::ABOVE;
    defineObjectSelected( id );
    return true;

}



bool Controller::isDefineBelowActive()
{
    std::size_t id;

    bool status = rules_processor.defineBelowIsActive( id );
    if( status == false )
    {
        setObjectSelected( boundering_below, false );
        return false;
    }

    current_region = RequestRegion::BELOW;
    defineObjectSelected( id );
    return true;
}


void Controller::clear()
{
    clearScenes();
    clearData();

    setDefaultValues();
}


void Controller::clearScenes()
{
    if( scene3d != nullptr )
        scene3d->clear();

    if( csection_scene != nullptr )
        csection_scene->clear();

    if( topview_scene != nullptr )
        topview_scene->clear();
}


void Controller::clearData()
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


void Controller::setDefaultValues()
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


