#include <algorithm>
#include <random>

#include "controller.h"

#include "volume.h"
#include "object.h"



void Controller::setScene3d( Scene3d* const& sc_ )
{
    scene3d = sc_;
}


void Controller::setObjectTree( ObjectTree* const& ot_ )
{
    object_tree = ot_;
}


void Controller::setCurrentColor( int r, int g, int b )
{
    current_color.r = r;
    current_color.g = g;
    current_color.b = b;

    setObjectColor( current_object, r, g, b );

}


void Controller::getCurrentColor( int& r, int& g, int& b ) const
{
    r = current_color.r;
    g = current_color.g;
    b = current_color.b;
}


void Controller::init()
{
    addVolume();
    addObject();
}




void Controller::addVolume()
{
    volume = new Volume();

    scene3d->addVolume( volume );
    object_tree->addInputVolume();

    initRulesProcessor();

}


bool Controller::isVolumeResizable() const
{
    if( volume == nullptr ) return false;
    return volume->isResizable();

}


void Controller::setVolumeOrigin( double ox_, double oy_, double oz_ )
{
    volume->setOrigin( ox_, oy_, oz_ );
}


void Controller::getVolumeOrigin( double& ox_, double& oy_, double& oz_ ) const
{
    volume->getOrigin( ox_, oy_, oz_ );
}


void Controller::setVolumeDimensions( const double& width_, const double& height_, const double& length_ )
{

    volume->setGeometry( width_, height_, length_ );
    scene3d->updateVolume();


    for ( Container< std::size_t, Object* >::Iterator it =  objects.begin(); it != objects.end(); ++it )
    {
        Object* item_ = objects.getElement( it->first );
        if( item_ == nullptr ) continue;

        double w = 0, h = 0,  l = 0;
        double ox_ = 0, oy_ = 0, oz_ = 0;

        volume->getOrigin( ox_, oy_, oz_ );
        volume->getGeometry( w, h, l );

        item_->setMaxMin( ox_ + w, oy_ + h, oz_ + l, ox_, oy_, oz_ );
    }

    updateBoundingBoxRulesProcessor();

}


void Controller::getVolumeDimensions( double& width_, double& height_, double& length_ ) const
{
    volume->getGeometry( width_, height_, length_ );
}


void Controller::setVolumeName( const std::string& name_ )
{
    volume->setName( name_ );
}


const std::string& Controller::getVolumeName() const
{
    return volume->getName();
}


void Controller::setVolumeVisibility( bool status_ )
{
    volume->setVisible( status_ );
    main_csection->setVisible( status_ );
    scene3d->updateVolume();
}


bool Controller::getVolumeVisibility() const
{
    return volume->isVisible();
}





void Controller::setupCrossSectionDiscretization( std::size_t& disc_, double& step_ )
{
    if( volume == nullptr ) return;

    disc_ = rules_processor.getDepthResolution();
    step_ = static_cast< double >( volume->getLenght()/disc_ );
    csection_step = step_;
}


void Controller::addMainCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    main_csection = new CrossSection( volume, dir_, depth_ );
    scene3d->addMainCrossSection( main_csection );

    setCurrentCrossSection( depth_ );
}


CrossSection* Controller::getMainCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_ ) const
{
    return main_csection;
}


void Controller::addTopViewCrossSection( double depth_ )
{
    topview_csection = new CrossSection( volume, Settings::CrossSection::CrossSectionDirections::Y, depth_ );
//    scene3d->addCrossSection( main_csection );

}


CrossSection* Controller::getTopViewCrossSection() const
{
    return topview_csection;
}


void Controller::setTopViewImage( std::string file_, double ox_, double oy_, double x_, double y_ )
{
    topview_csection->setImage( file_, ox_, oy_, x_, y_ );
}



void Controller::removeTopViewImage()
{
    topview_csection->clearImage();
}



void Controller::setImageCrossSection( double depth_, const std::string& path_, double ox_, double oy_, double x_, double y_ )
{

    ImageData image_data;
    image_data.file = path_;
    image_data.ox = ox_;
    image_data.oy = oy_;
    image_data.x = x_;
    image_data.y = y_;

    csections_background[ depth_ ] = image_data;

    CrossSection* csection_ = getCrossSection( depth_ );
    if( csection_ != nullptr )
    {
        csection_->setImage( path_, ox_, oy_, x_, y_ );
    }
    if( main_csection->getDepth() == depth_ )
    {
        main_csection->setImage( path_, ox_, oy_, x_, y_ );
    }

}



bool Controller::hasImageCrossSection( double depth_ )
{
    if( csections_background.find( depth_ ) == csections_background.end() )
        return false;
    return true;
}


bool Controller::clearImageCrossSection( double depth_ )
{

    if( csections_background.find( depth_ ) == csections_background.end() )
        return false;


    CrossSection* csection_ = getCrossSection( depth_ );
    if( csection_ != nullptr )
    {
        csection_->clearImage();
    }
    if( main_csection->getDepth() == depth_ )
    {
        main_csection->clearImage();
    }


    csections_background.erase( depth_ );
    return true;


}


bool Controller::getImageCrossSection( double depth_, std::string& path_, double& ox_, double& oy_, double& x_, double& y_ )
{

    if( hasImageCrossSection( depth_ ) == false ) return false;

    ImageData image_data = csections_background[ depth_ ];
    path_  = image_data.file ;
    ox_    = image_data.ox;
    oy_    = image_data.oy;
    x_ = image_data.x;
    y_ = image_data.y;

    return true;
}



void Controller::setCurrentCrossSection( double depth_ )
{

    current_csection = depth_;
    if( main_csection == nullptr ) return;
    main_csection->setDepth( depth_ );

    updateCurrentCrossSection();
    scene3d->updateMainCrossSection();

    std::cout << "Current cross-section: " << depth_ << std::endl << std::flush;

}


void Controller::updateCurrentCrossSection()
{
    std::string path_;
    double ox_ = 0.0;
    double oy_ = 0.0;
    double x_ = 100.0;
    double y_ = 100.0;

    if( getImageCrossSection( current_csection, path_, ox_, oy_, x_, y_ ) == true )
    {
        main_csection->setImage( path_, ox_, oy_, x_, y_ );
    }
    else
    {
        main_csection->clearImage();
    }


    std::vector< std::size_t > actives_ = rules_processor.getSurfaces();
    for ( std::size_t id_: actives_ )
    {
        updateObjectCurveFromCrossSection( id_, current_csection );
    }
}


CrossSection* Controller::getCrossSection( const double& depth_ ) const
{
    if( all_csections.findElement( depth_ ) == true )
        return all_csections.getElement( depth_ );
    else if( fixed_csections.findElement( depth_ ) == true )
        return fixed_csections.getElement( depth_ );
    else
        return nullptr;
}


CrossSection* Controller::getCurrentCrossSection() const
{
    return getCrossSection( current_csection );
}




bool Controller::addObject( std::size_t index_ )
{

    Object* obj_ = new Object();

    if( index_ != 9999 )
    {
        obj_->setIndex( index_ );
        current_object = index_;
    }
    else
        current_object = obj_->getIndex();


    double w = 0, h = 0,  l = 0;
    double ox_ = 0, oy_ = 0, oz_ = 0;

    volume->getOrigin( ox_, oy_, oz_ );
    volume->getGeometry( w, h, l );


    obj_->setMaxMin( ox_ + w, oy_ + h, oz_ + l, ox_, oy_, oz_ );
    obj_->setColor( current_color.r, current_color.g, current_color.b );

    bool status_ = objects.addElement( current_object, obj_ );
    if( status_ == false ) return false;

    volume->addObject( current_object, obj_ );

    object_tree->addObject( current_object, Settings::Objects::ObjectType::STRATIGRAPHY,
                            obj_->getName(), current_color.r, current_color.g, current_color.b );
    object_tree->setObjectVisibility( current_object, false );
    scene3d->addObject( obj_ );

    return true;
}


void Controller::setObjectName( std::size_t index_, const std::string& name_ )
{
    if( objects.findElement( index_) == false )
        return;

    Object* const& obj_ = objects.getElement( index_ );
    obj_->setName( name_ );
}


std::string Controller::getObjectName( std::size_t index_) const
{
    if( objects.findElement( index_) == false )
        return std::string("");
    Object* const& obj_ = objects.getElement( index_ );
    return obj_->getName();
}


void  Controller::setObjectVisibility( std::size_t index_, bool status_ )
{
    if( objects.findElement( index_ ) == false )
        return;

    Object* const& obj_ = objects.getElement( index_ );
    obj_->setVisible( status_ );
    scene3d->updateObject( index_ );

}


void Controller::setObjectColor( std::size_t index_, int r_, int g_, int b_)
{
    if( objects.findElement( index_) == false )
        return;

    Object* const& obj_ = objects.getElement( index_ );
    obj_->setColor( r_, g_, b_ );

    scene3d->updateObject( index_ );
    object_tree->updateObjectColor( index_, r_, g_, b_ );
}


void Controller::saveObjectInformation( std::size_t index_, const std::string & text_ )
{
    if( objects.findElement( index_ ) == false )
        return;

    Object* const& obj_ = objects.getElement( index_ );
    obj_->saveInformation( text_ );
}


const std::string& Controller::getObjectInformation( std::size_t index_ ) const
{
    if( objects.findElement( index_ ) == false )
        return "";

    Object* const& obj_ = objects.getElement( index_ );
    return obj_->getInformation();
}


bool Controller::addObjectCurve( PolyCurve curve_, double depth_ )
{

    if( objects.findElement( current_object ) == false )
        return false;

    if( curve_.isEmpty() == true ) return false;

    Object* const& obj_ = objects.getElement( current_object );
    if( obj_->hasCurve( depth_ ) == false )
    {
        bool status_ = obj_->addCurve( depth_, curve_ );
        if( status_ == false )
            return false;
    }
    else
        obj_->updateCurve( depth_, curve_ );


    obj_->setEditable( true );
    obj_->setVisible( true );

    object_tree->setObjectVisibility( current_object, true );



//    bool status_ = main_csection->addObject( obj_->getIndex(), &curve_ );


    CrossSection* cs_ = new CrossSection( volume, Settings::CrossSection::CrossSectionDirections::Z, depth_ );
    cs_->addObject( obj_->getIndex(), &curve_ );

    volume->addCrossSection( cs_->getIndex(), cs_ );

    if( all_csections.findElement( depth_ ) == false )
        all_csections.addElement( depth_, cs_ );


    createPreviewSurface();


    return true;

}


bool Controller::removeObjectCurve( std::size_t index_, double depth_ )
{
    if( objects.findElement( index_ ) == false )
        return false;

    Object* const& obj_ = objects.getElement( index_ );
    obj_->removeCurve( depth_ );

    CrossSection* csection_ = getCurrentCrossSection();
    if( csection_ == nullptr ) return true;

    csection_->removeObjectCurve( index_ );
    createPreviewSurface();

    return true;

}


bool Controller::removeObjectCurve( double depth_ )
{
    return removeObjectCurve( current_object, depth_ );
}


bool Controller::addObjectTrajectory( PolyCurve curve_ )
{

    if( objects.findElement( current_object ) == false )
        return false;

    Object* const& obj_ = objects.getElement( current_object );
    bool status_ = obj_->addTrajectory( curve_ );


    if( status_ == false ) return false;

    obj_->setEditable( true );
    obj_->setVisible( true );
    object_tree->setObjectVisibility( current_object, true );

    return true;

}


void Controller::removeObjectTrajectory()
{

    if( objects.findElement( current_object ) == false )
        return;

    Object* const& obj_ = objects.getElement( current_object );
    obj_->removeTrajectory();

}




Object* Controller::getCurrentObject() const
{
    return getObject( current_object );
}


Object* Controller::getObject( std::size_t index_ ) const
{
    if( objects.findElement( index_) == false )
        return nullptr;
    return objects.getElement( index_ );
}





bool Controller::createPreviewSurface()
{
    Object* const& obj_ = objects.getElement( current_object );
    Object::CrossSectionsContainer cs_ = obj_->getCrossSectionCurves();

    std::vector< std::tuple< Curve2D, double > > curves_;
    for ( Object::CrossSectionsContainer::Iterator it =  cs_.begin(); it != cs_.end(); ++it )
    {
        double csection_id_ = it->first;
        PolyCurve sketch_ = it->second;

        if( all_csections.findElement( csection_id_ ) == false )
            continue;

        Curve2D curve_ = vectorToCurve2D( sketch_.getVertices() );
        curves_.push_back( std::make_tuple( curve_, csection_id_ ) );
    }


    bool surface_created = rules_processor.testSurface( current_object, curves_ );
    if( surface_created == false )
    {
        obj_->removeSurface();
        obj_->setVisible( false );
        scene3d->updateObject( current_object );
        updateObjectInFixedCrossSections( current_object );
        return false;
    }

    std::vector< double > vertices_;
    std::vector< std::size_t > faces_;

    bool has_surface = rules_processor.getMesh( current_object, vertices_, faces_ );
    if( has_surface  == false ) return false;


    std::vector< double > normals_;
    rules_processor.getNormals( current_object, normals_ );


    Surface surface;
    surface.setVertices( vertices_ );
    surface.setFaces( faces_ );
    surface.setNormals( normals_ );

    obj_->setActive( true );
    obj_->setSurface( surface );
    obj_->setVisible( true );

    scene3d->updateObject( current_object );
    updatePreviewCurves( obj_, current_csection );
    updateObjectInFixedCrossSections( current_object );


    return true;

}


bool Controller::createObjectSurface()
{

    Object* const& obj_ = objects.getElement( current_object );
    Object::CrossSectionsContainer cs_ = obj_->getCrossSectionCurves();

    std::vector< std::tuple< Curve2D, double > > curves_;
    for ( Object::CrossSectionsContainer::Iterator it =  cs_.begin(); it != cs_.end(); ++it )
    {
        double csection_id_ = it->first;
        PolyCurve sketch_ = it->second;

        if( all_csections.findElement( csection_id_ )  == false ) continue;

        CrossSection* csection_ = all_csections.getElement( csection_id_ );
        Curve2D curve_ = vectorToCurve2D( sketch_.getVertices() );

        curves_.push_back( std::make_tuple( curve_, csection_id_ ) );
    }

    applyStratigraphicRule();


    bool surface_created = false;
    if( obj_->hasTrajectory() == true )
    {
        Curve2D curve_ = std::get<0>( curves_[ 0 ] );
        Curve2D traj_ = vectorToCurve2D( obj_->getTrajectory().getVertices() );
        double depth_ = std::get<1>( curves_[ 0 ] );

        surface_created = rules_processor.extrudeAlongPath( current_object, curve_, depth_, traj_ );
    }

    else
    {
        surface_created = rules_processor.createSurface( current_object, curves_ );
    }


    if( surface_created == false ) return false;

    obj_->removeCrossSectionCurves();
    obj_->removeTrajectory();
    obj_->setEditable( false );

    updateModel();

    return true;

}






void Controller::initRulesProcessor()
{
    updateBoundingBoxRulesProcessor();
    rules_processor.truncate();
}


void Controller::updateBoundingBoxRulesProcessor()
{
    if( volume == nullptr ) return;

    double ox = 0.0, oy = 0.0, oz = 0.0;
    volume->getOrigin( ox, oy, oz );

    rules_processor.setOrigin( ox, oy, oz );
    rules_processor.setLenght( volume->getWidth(), volume->getHeight(), volume->getLenght() );
//    rules_processor.setMediumResolution();
    setMeshResolution( Controller::MeshResolution::LOW );
}


void Controller::getCurveFromRulesProcessor( Object* obj_, double csection_depth_ )
{

    std::size_t object_id_ = obj_->getIndex();


    std::vector< double > vertices_;
    std::vector< std::size_t > edges_;
    bool has_curve = rules_processor.getCrossSection( object_id_, indexCrossSection( csection_depth_ ), vertices_, edges_ );
    if( has_curve == false ) return;


    PolyCurve curve_ = PolyCurve( vertices_, edges_ );
    obj_->updateCurve( csection_depth_, curve_ );


    if( all_csections.findElement( csection_depth_ ) == true )
    {
        CrossSection* csection_ = all_csections.getElement( csection_depth_ );
        csection_->addObject( object_id_, &curve_ );
    }

}





void Controller::updateObjectCurveFromCrossSection( std::size_t object_id_, double csection_depth_ )
{
    Object* obj_ = objects.getElement( object_id_ );
    if( obj_->isDone() == true )
    {

        obj_->removeCurve( csection_depth_ );
        if( all_csections.findElement( csection_depth_ ) == true )
        {
            CrossSection* csection_ = all_csections.getElement( csection_depth_ );
            csection_->removeObjectCurve( object_id_ );
        }

        getCurveFromRulesProcessor( obj_, csection_depth_ );

    }
    else
        updatePreviewCurves( obj_, csection_depth_ );
}


void Controller::updatePreviewCurves( Object* obj_, double csection_depth_ )
{

    bool has_user_curve = obj_->hasCurve( csection_depth_ );
    has_user_curve &= all_csections.findElement( csection_depth_ );

    if( ( has_user_curve == true  ) || ( preview_enabled == false ) ) return;

    getCurveFromRulesProcessor( obj_, csection_depth_ );

}


void Controller::updateObjectSurfaces( std::size_t object_id_ )
{

    Object* obj_ = objects.getElement( object_id_ );

    std::vector< double > vertices_;
    std::vector< std::size_t > faces_;

    bool has_surface = rules_processor.getMesh( object_id_, vertices_, faces_ );
    if( has_surface  == false ) return;

    std::vector< double > normals_;
    rules_processor.getNormals( object_id_, normals_ );


    Surface surface;
    surface.setVertices( vertices_ );
    surface.setFaces( faces_ );
    surface.setNormals( normals_ );


    std::vector< double > path_;
    bool has_path = rules_processor.getExtrusionPath( object_id_, path_ );
    if( has_path == true )
    {
        obj_->removeTrajectory();
        obj_->addTrajectory( PolyCurve( path_ ) );
    }

    obj_->setActive( true );
    obj_->setSurface( surface );
    obj_->setVisible( true );
    obj_->setDone( true );
    object_tree->setObjectVisibility( object_id_, true );

    scene3d->updateObject( object_id_ );
}


void Controller::updateObjectInFixedCrossSections( std::size_t id_ )
{
    for ( Container< double, CrossSection* >::Iterator cs_it =  fixed_csections.begin(); cs_it != fixed_csections.end(); ++cs_it )
    {
        if( cs_it->first == current_csection ) continue;
        updateObjectCurveFromCrossSection( id_, cs_it->first );
    }

}


void Controller::updateModel()
{

    std::vector< std::size_t > actives_ = rules_processor.getSurfaces();

    setActiveAllObjects( false );

    for ( std::size_t id_: actives_ )
    {
        updateObjectSurfaces( id_ );
        updateObjectCurveFromCrossSection( id_, current_csection );
        updateObjectInFixedCrossSections( id_ );
    }

}


void Controller::setActiveAllObjects( bool status_ )
{
    for ( Container< std::size_t, Object* >::Iterator it =  objects.begin(); it != objects.end(); ++it )
    {
        Object* obj_ = (it->second);
        obj_->setActive( status_ );
        object_tree->setObjectVisibility( it->first, status_ );
        scene3d->updateObject( it->first );
    }
}





void Controller::setRemoveAbove()
{
    current_rule = Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE;
}


void Controller::setRemoveAboveIntersection()
{
    current_rule = Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE_INTERSECTION;
}


void Controller::setRemoveBelow()
{
    current_rule = Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW;
}


void Controller::setRemoveBelowIntersection()
{
    current_rule = Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW_INTERSECTION;
}

void Controller::setTruncate()
{
    current_rule = Settings::Stratigraphy::StratigraphicRules::TRUNCATE;
}


void Controller::applyStratigraphicRule()
{
    if( current_rule == Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE )
        rules_processor.removeAbove();
    else if( current_rule == Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE_INTERSECTION )
        rules_processor.removeAboveIntersection();
    else if( current_rule == Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW )
        rules_processor.removeBelow();
    else if( current_rule == Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW_INTERSECTION )
        rules_processor.removeBelowIntersection();
    else if( current_rule == Settings::Stratigraphy::StratigraphicRules::TRUNCATE )
        rules_processor.truncate();
}




bool Controller::enableCreateAbove( bool status_ )
{
//    setObjectsAsSelectable( selectable_upper, false );

    if( status_ == false )
    {
        stopCreateAbove();
        return false;
    }

    return requestCreateAbove();

}


void Controller::stopCreateAbove()
{
    std::cout << "Stop create above accepted" << std::endl << std::flush;
    rules_processor.stopDefineAbove();
    setObjectsAsSelectable( selectable_upper, false );
    setObjectAsSelected( index_upper_boundary, false );
}


bool Controller::requestCreateAbove()
{

    bool request_ = rules_processor.requestCreateAbove( selectable_upper );

    if( request_ == true )
    {
        std::cout << "Request create accepted" << std::endl << std::flush;

        setObjectsAsSelectable( selectable_bottom, false );
        setObjectsAsSelectable( selectable_upper, true );

        boundering_region = Settings::Objects::BounderingRegion::ABOVE;

    }
    else
        std::cout << "Request create denied" << std::endl << std::flush;

    return request_ ;

}





bool Controller::enableCreateBelow( bool status_ )
{


//    setObjectsAsSelectable( selectable_bottom, false );

    if( status_ == false )
    {
        stopCreateBelow();
        return false;
    }

    return requestCreateBelow();

}


void Controller::stopCreateBelow()
{
    std::cout << "Stop create below accepted" << std::endl << std::flush;
    rules_processor.stopDefineBelow();
    setObjectsAsSelectable( selectable_bottom, false );
    setObjectAsSelected( index_bottom_boundary, false );

}


bool Controller::requestCreateBelow()
{

    bool request_ = rules_processor.requestCreateBelow( selectable_bottom );

    if( request_ == true )
    {
        std::cout << "Request create accepted" << std::endl << std::flush;

        setObjectsAsSelectable( selectable_upper, false );
        setObjectsAsSelectable( selectable_bottom, true );
        boundering_region = Settings::Objects::BounderingRegion::BELOW;

    }
    else
        std::cout << "Request create denied" << std::endl << std::flush;

    return request_ ;

}




bool Controller::isDefineAboveObjectSelected()
{

    return selectable_upper.empty();
}


bool Controller::isDefineBelowObjectSelected()
{
    return selectable_bottom.empty();
}



void Controller::setObjectAsBoundering( std::size_t index_ )
{

    if( objects.findElement( index_ ) == false ) return;


    setObjectAsSelected( index_, true );

    if( boundering_region == Settings::Objects::BounderingRegion::ABOVE )
    {
        index_upper_boundary = index_;
        rules_processor.defineAbove( index_ );
        setObjectsAsSelectable( selectable_upper, false );
    }
    else if( boundering_region == Settings::Objects::BounderingRegion::BELOW )
    {
        index_bottom_boundary = index_;
        rules_processor.defineBelow( index_ );
        setObjectsAsSelectable( selectable_bottom, false );
    }

}


void Controller::setObjectsAsSelectable( std::vector< std::size_t >& indexes_,
                                                    bool status_ )
{

    for( std::size_t id_: indexes_ )
    {
        if( objects.findElement( id_ ) == false ) continue;

        Object* obj_ = objects.getElement( id_ );
        obj_->setSelectable( status_ );
    }

    if( status_ == false )
        indexes_.clear();
}


void Controller::setObjectAsSelected( std::size_t index_, bool status_ )
{

    if( objects.findElement( index_ ) == false ) return;

    Object* obj_ = objects.getElement( index_ );
    obj_->setSelected( status_ );
}




void Controller::saveFile( const std::string& filename )
{
    rules_processor.saveFile( filename );
    saveObjectsMetaData( filename );
}


bool Controller::saveObjectsMetaData( const std::string& filename )
{

    std::string complete_filename = filename + ".json";
    QFile save_file( QString( complete_filename.c_str() ) );

    if ( !save_file.open( QIODevice::WriteOnly ) ) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject metadatas;

    QJsonArray objects_array_;
    for( auto it: objects )
    {
        const Object* obj_ = it.second;
        QJsonObject object_;
        obj_->write( object_ );
        objects_array_.append( object_ );
    }
    metadatas["objects"] = objects_array_;


    QJsonDocument save_doc( metadatas );
    save_file.write( save_doc.toJson() );

    return true;
}


void Controller::loadFile( const std::string& filename )
{
    rules_processor.loadFile( filename );
    loadObjects( filename );
}


void Controller::loadObjects( const std::string& filename )
{
    if( volume == nullptr ) return;

    double ox, oy, oz;
    rules_processor.getOrigin( ox, oy, oz );
    volume->setOrigin( ox, oy, oz );

    double width, height, depth;
    rules_processor.getLenght( width, height, depth );
    setVolumeDimensions( width, height, depth );


    if( objects.findElement( current_object ) == true )
    {
        objects.removeElement( current_object );
        volume->removeObject( current_object );
        object_tree->removeObject( current_object );
        scene3d->removeObject( current_object );

        Object::resetAllObjects();
        CrossSection::resetAllCrossSections();
    }


    std::string complete_filename = filename + ".json";

    QFile load_file( QString( complete_filename.c_str() ) );
    if ( !load_file.open( QIODevice::ReadOnly ) ) {
        loadObjectNoMetaDatas();
    }
    else
        loadObjectMetaDatas( load_file );

    addObject();
    updateModel();

}

void Controller::loadObjectNoMetaDatas()
{
    std::random_device rd;
    std::mt19937 eng( rd() );
    std::uniform_int_distribution< size_t > distr( 0, 255 );

    std::vector< std::size_t > actives = rules_processor.getSurfaces();
    for( auto id: actives )
    {
        int r_ = distr( eng );
        int g_ = distr( eng );
        int b_ = distr( eng );

        addObject( id );
        setObjectColor( id, r_, g_, b_ );
    }

}


void Controller::loadObjectMetaDatas( QFile& load_file )
{

    std::random_device rd;
    std::mt19937 eng( rd() );
    std::uniform_int_distribution< size_t > distr( 0, 255 );

    QByteArray save_data = load_file.readAll();
    QJsonDocument load_doc( QJsonDocument::fromJson(save_data) );
    const QJsonObject &json = load_doc.object();

    if ( json.contains("objects") && json["objects"].isArray() )
    {
        QJsonArray objects_array_ = json["objects"].toArray();

        std::vector< std::size_t > actives = rules_processor.getSurfaces();

        int obj_id_ = 0;
        for( auto id: actives )
        {
            int r_= distr( eng );
            int g_= distr( eng );
            int b_= distr( eng );

            addObject( id );

            if( obj_id_ < objects_array_.size() )
            {
                QJsonObject object_ = objects_array_[obj_id_].toObject();

                Object* obj_ = objects.getElement( id );
                obj_->read( object_ );
                obj_->getColor( r_, g_, b_ );

                obj_id_++;
            }
            else
            {
                setObjectColor( id, r_, g_, b_ );
            }

            scene3d->updateObject( id );
            object_tree->updateObjectColor( id, r_, g_, b_ );

        }
    }
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
    std::size_t index_;

    bool status_ = rules_processor.defineAboveIsActive( index_ );
    if( status_ == false )
    {
        setObjectAsSelected( index_upper_boundary, false );
        return false;
    }

    boundering_region = Settings::Objects::BounderingRegion::ABOVE;
    setObjectAsBoundering( index_ );
    return true;

}


bool Controller::isDefineBelowActive()
{
    std::size_t index_;

    bool status_ = rules_processor.defineBelowIsActive( index_ );
    if( status_ == false )
    {
        setObjectAsSelected( index_bottom_boundary, false );
        return false;
    }

    boundering_region = Settings::Objects::BounderingRegion::BELOW;
    setObjectAsBoundering( index_ );
    return true;
}




void Controller::getOutputVolume( std::map< std::size_t, Volume::Color >& regions_map_  )
{

    for ( Container< std::size_t, Regions* >::Iterator it =  regions.begin(); it != regions.end(); ++it )
    {
        Regions* item_ = regions.getElement( it->first );
        if( item_ == nullptr ) continue;

        item_->clear();
        delete item_;
        item_ = nullptr;
    }
    regions.clear();

    scene3d->clearOutputVolume();
    object_tree->removeOutputVolume();


    double w = 0, h = 0,  l = 0;
    double ox_ = 0, oy_ = 0, oz_ = 0;

    volume->getOrigin( ox_, oy_, oz_ );
    volume->getGeometry( w, h, l );


    std::vector< double > vertices_;
    std::vector< std::vector< std::size_t > > regions_;
    rules_processor.getTetrahedralMesh( vertices_, regions_ );

    Volume* vol1_ = new Volume();
    vol1_->setVertices( vertices_ );
    vol1_->setOrigin( ox_, oy_, oz_ );
    vol1_->setGeometry( w, h, l );
    scene3d->addOutputVolume( vol1_ );
    object_tree->addOutputVolume();





    std::random_device rd;
    std::mt19937 eng( rd() );
    std::uniform_int_distribution< size_t > distr( 0, 255 );


    std::size_t number_of_regions = regions_.size();
    std::vector< int > colors_ = rules_processor.getRegionsColor( number_of_regions );

    for( std::size_t i = 0; i < number_of_regions; ++i )
    {
        Volume::Color color_;
        color_.r = colors_[ 3*i ];
        color_.g = colors_[ 3*i + 1 ];
        color_.b = colors_[ 3*i + 2 ];

//        color_.r = distr( eng );
//        color_.g = distr( eng );
//        color_.b = distr( eng );


        Regions* region_ = new Regions();

        region_->setIndex( i );
        region_->setVertices( vertices_ );
        region_->setTetrahedralCells( regions_[ i ] );
        region_->setColor( color_.r, color_.g, color_.b );
        region_->setMaxMin( ox_ + w, oy_ + h, oz_ + l, ox_, oy_, oz_ );

        regions.addElement( i, region_ );
        scene3d->addRegion( region_ );
        object_tree->addRegion( i, region_->getName(), color_.r, color_.g, color_.b );

        vol1_->addRegion( i, regions_[ i ], color_ );

        regions_map_[ i ] = color_;
    }





}


void Controller::setRegionName( std::size_t index_, const std::string& name_ )
{
    if( regions.findElement( index_) == false )
        return;

    Regions* const& region_ = regions.getElement( index_ );
//    region_->setName( name_ );
}


void Controller::setRegionVisibility( std::size_t index_, bool status_ )
{
    if( regions.findElement( index_ ) == false )
        return;

    Regions* const& region_ = regions.getElement( index_ );
    region_->setVisible( status_ );
    scene3d->updateRegion( index_ );
}


void Controller::setRegionColor( std::size_t index_, int r_, int g_, int b_ )
{

    if( regions.findElement( index_ ) == false )
        return;

    Regions* const& region_ = regions.getElement( index_ );
    region_->setColor( r_, g_, b_ );
    scene3d->updateRegion( index_ );
}



void Controller::getRegionColor( std::size_t index_, int& r_, int& g_, int& b_ )
{

    if( regions.findElement( index_ ) == false )
        return;

    Regions* const& region_ = regions.getElement( index_ );
    region_->getColor( r_, g_, b_ );
}


void Controller::hideRegions()
{
    if( regions.empty() == true ) return;

    for( auto id: regions )
        setRegionVisibility( id.first, false );

    object_tree->hideOutputVolume();
    scene3d->updateRegions();




}


void Controller::clear()
{

    if( scene3d != nullptr )
    {
        scene3d->clear();
    }


    if( object_tree != nullptr )
        object_tree->clear();


    if( volume != nullptr )
    {
        volume->clear();
        delete volume;
        volume = nullptr;
    }


    if( main_csection != nullptr )
    {
        main_csection->clear();
        delete main_csection;
        main_csection = nullptr;
    }

    if( topview_csection != nullptr )
    {
        topview_csection->clear();
        delete topview_csection;
        topview_csection = nullptr;
    }


    csections_background.clear();


    for ( Container< double, CrossSection* > ::Iterator it =  all_csections.begin(); it != all_csections.end(); ++it )
    {
        CrossSection* item_ = all_csections.getElement( it->first );
        if( item_ == nullptr ) continue;

        item_->clear();
        delete item_;
        item_ = nullptr;
    }
    all_csections.clear();


    for ( Container< std::size_t, Object* >::Iterator it =  objects.begin(); it != objects.end(); ++it )
    {
        Object* item_ = objects.getElement( it->first );
        if( item_ == nullptr ) continue;

        item_->clear();
        delete item_;
        item_ = nullptr;
    }
    objects.clear();


    for ( Container< std::size_t, Regions* >::Iterator it =  regions.begin(); it != regions.end(); ++it )
    {
        Regions* item_ = regions.getElement( it->first );
        if( item_ == nullptr ) continue;

        item_->clear();
        delete item_;
        item_ = nullptr;
    }
    regions.clear();


    Object::resetAllObjects();
    CrossSection::resetAllCrossSections();

    selectable_upper.clear();
    selectable_bottom.clear();

    rules_processor.clear();

    current_color.r = 255;
    current_color.g = 0;
    current_color.b = 0;

    current_object = 0;
    current_csection = 500.0;
    csection_step = 1.0;

    index_upper_boundary = 0;
    index_bottom_boundary = 0;

    boundering_region = Settings::Objects::BounderingRegion::NONE;

}


bool Controller::addFixedCrossSection( double depth_ )
{
    CrossSection* cs_ = new CrossSection( volume, Settings::CrossSection::CrossSectionDirections::Z, depth_ );
    bool status_ = fixed_csections.addElement( depth_, cs_ );
    if( status_ == false ) return false;

    setCurrentCrossSection( depth_ );
    scene3d->addCrossSection( cs_ );
    return true;
}



bool Controller::removeFixedCrossSection( double depth_ )
{
    if( fixed_csections.findElement( depth_ ) == false ) return false;

    CrossSection* cs_ = fixed_csections.getElement( depth_ );
    scene3d->removeCrossSection( cs_ );


    cs_->clear();
    delete cs_;
    cs_ = nullptr;

    fixed_csections.removeElement( depth_ );
    return true;

//    setCurrentCrossSection( main_csection->getDepth() );
//    scene3d->removeCrossSection( cs_ );

}



void Controller::setSurfacesMeshes( std::vector< TriangleMesh >& triangles_meshes,
                                    std::vector< CurveMesh >& left_curves,
                                    std::vector< CurveMesh >& right_curves,
                                    std::vector< CurveMesh > & front_curves,
                                    std::vector< CurveMesh >& back_curves )
{
    rules_processor.setPLCForSimulation(triangles_meshes, left_curves, right_curves, front_curves, back_curves);

/*     std::vector< std::size_t > actives = rules_processor.getSurfaces(); */

/*     for( auto it: actives ) */
/*     { */

/*         if( objects.findElement( it ) == false ) continue; */

/*         TriangleMesh t; */
/*         /1* std::vector< double > surface_vertices; *1/ */
/*         /1* std::vector< std::size_t > surface_faces; *1/ */


/*         /1* bool has_surface = rules_processor.getMesh( it, surface_vertices, surface_faces ); *1/ */
/*         bool has_surface = rules_processor.getMesh( it, t.vertex_list, t.face_list ); */

/*         if( has_surface  == false ) continue; */


/*         /1* t.face_list = surface_faces; *1/ */
/*         /1* t.vertex_list = surface_vertices; *1/ */

/*         // */
/*         // This loop changes the y-z coordinates of the vertices as RRM */
/*         // understands the y coordinate as height and the z coordinate as */
/*         // length, but Zhao's convention is the opposite. */
/*         // */
/*         double y, z; */
/*         for ( size_t i = 0; i < t.vertex_list.size()/3; ++i ) */
/*         { */
/*             y = t.vertex_list[3*i + 1]; */
/*             z = t.vertex_list[3*i + 2]; */

/*             t.vertex_list[3*i + 1] = z; */
/*             t.vertex_list[3*i + 2] = y; */
/*         } */

/*         triangles_meshes.push_back( t ); */
/*     } */

/*     std::vector< std::vector<double> > lb_vertex_lists, rb_vertex_lists, fb_vertex_lists, bb_vertex_lists; */
/*     std::vector< std::vector<std::size_t> >lb_edge_lists, rb_edge_lists, fb_edge_lists, bb_edge_lists; */

/*     rules_processor.getLeftBoundaryCrossSectionCurve(lb_vertex_lists, lb_edge_lists); */
/*     rules_processor.getRightBoundaryCrossSectionCurve(rb_vertex_lists, rb_edge_lists); */
/*     rules_processor.getFrontBoundaryCrossSectionCurve(fb_vertex_lists, fb_edge_lists); */
/*     rules_processor.getBackBoundaryCrossSectionCurve(bb_vertex_lists, bb_edge_lists); */

/*     for ( int i = 0; i < lb_vertex_lists.size(); ++i ) */
/*     { */
/*         CurveMesh cm_lb, cm_rb, cm_fb, cm_bb; */

/*         std::copy( lb_vertex_lists[i].begin(), lb_vertex_lists[i].end(), std::back_inserter(cm_lb.vertex_list) ); */
/*         std::copy( lb_edge_lists[i].begin(), lb_edge_lists[i].end(), std::back_inserter(cm_lb.edge_list) ); */

/*         std::copy( rb_vertex_lists[i].begin(), rb_vertex_lists[i].end(), std::back_inserter(cm_rb.vertex_list) ); */
/*         std::copy( rb_edge_lists[i].begin(), rb_edge_lists[i].end(), std::back_inserter(cm_rb.edge_list) ); */

/*         std::copy( fb_vertex_lists[i].begin(), fb_vertex_lists[i].end(), std::back_inserter(cm_fb.vertex_list) ); */
/*         std::copy( fb_edge_lists[i].begin(), fb_edge_lists[i].end(), std::back_inserter(cm_fb.edge_list) ); */

/*         std::copy( bb_vertex_lists[i].begin(), bb_vertex_lists[i].end(), std::back_inserter(cm_bb.vertex_list) ); */
/*         std::copy( bb_edge_lists[i].begin(), bb_edge_lists[i].end(), std::back_inserter(cm_bb.edge_list) ); */

/*         left_curves.push_back( cm_lb ); */
/*         right_curves.push_back( cm_rb ); */
/*         front_curves.push_back( cm_fb ); */
/*         back_curves.push_back( cm_bb ); */
/*     } */
}



std::size_t Controller::indexCrossSection( double value_ ) const
{
    return static_cast< std::size_t > ( value_/csection_step );
}


double Controller::depthCrossSection( std::size_t index_ ) const
{
    return static_cast< double > ( index_*csection_step );
}

std::vector<int> Controller::getTetrahedronsRegions( const std::vector< float >& vertices, const std::vector< unsigned int >& edges, const std::vector< unsigned int >& faces )
{

    std::vector< double > points;
    for( auto v: vertices )
        points.push_back( static_cast< double >( v ) );

    std::vector< std::size_t > elements_;
    for( auto e: faces )
        elements_.push_back( static_cast< std::size_t >( e ) );

    std::vector<int> regions_;
    rules_processor.getRegionsForSimulationTetrahedralMesh( points, elements_, regions_ );
    return regions_;
}

void Controller::setMeshResolution( const Controller::MeshResolution& resolution_ )
{
    if( resolution_ == Controller::MeshResolution::LOW )
    {
        rules_processor.setLowResolution();
    }
    else if( resolution_ == Controller::MeshResolution::MEDIUM )
    {
        rules_processor.setMediumResolution();
    }
    else if( resolution_ == Controller::MeshResolution::HIGH )
    {
        rules_processor.setHighResolution();
    }
}


void Controller::setPreviewEnabled( bool status_ )
{
    preview_enabled = status_;
    if( preview_enabled == true ) return;

    Object* obj_ = getCurrentObject();
    obj_->clearPreviewCurves();

}

bool Controller::isPreviewEnabled() const
{
    return preview_enabled;
}

void Controller::exportToIrapGrid()
{

    IrapGridExporter exporter;
    std::vector< float > points;

    std::vector< std::size_t > actives_ = rules_processor.getSurfaces();
    for ( std::size_t id_: actives_ )
    {
        QString surface_filename = QString( "Surface %1.IRAPG").arg( id_ );

        std::vector< double > points_list;
        std::vector< bool > valid_points;
        std::size_t nu;
        std::size_t nv;

        rules_processor.getQuadMesh( id_, points_list, valid_points, nu, nv );

        double xmax = points_list[ 0 ], xmin = points_list[ 0 ];
        double ymax = points_list[ 1 ], ymin = points_list[ 1 ];
        double zmax = points_list[ 2 ], zmin = points_list[ 2 ];

        std::size_t number_elements = nu*nv;

        for( std::size_t i = 0; i < number_elements; ++i )
        {

            double x = points_list[ 3*i + 0 ];
            double y = points_list[ 3*i + 1 ];
            double z = points_list[ 3*i + 2 ];

            if( x >= xmax ) xmax = x;
            else if( x < xmin ) xmin = x;

            if( y >= ymax ) ymax = y;
            else if( y < ymin ) ymin = y;


            if( z >= zmax ) zmax = z;
            else if( z < zmin ) zmin = z;

            points.push_back( static_cast<float>( z ) );

        }

        float dx = (float)( xmax - xmin )/nu;
        float dy = (float)( ymax - ymin )/nv;

        exporter.setBoundingBox( (float )xmin, (float)xmax, (float)ymin, (float) ymax, (float) zmin, (float) zmax );
        exporter.setVectorValues( points );
        exporter.setSize( (int) nu,  (int) nv );
        exporter.setSpacing( dx, dy );

        std::string name_ = getObjectName( id_ ).append( ".IRAPG" );
        if( name_.empty() == true )
            exporter.writeGridData( surface_filename.toStdString() );
        else
            exporter.writeGridData( name_ );

        exporter.clearData();

        points.clear();
        surface_filename.clear();
    }

}
