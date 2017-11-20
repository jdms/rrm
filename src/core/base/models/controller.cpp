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

//    setObjectColor( current_object, r, g, b );

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
    if( volume != nullptr ) return false;
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
}



void Controller::addMainCrossSection( const CrossSection::Direction& dir_, double depth_ )
{
    main_csection = new CrossSection( volume, dir_, depth_ );
    scene3d->addCrossSection( main_csection );

    setCurrentCrossSection( depth_ );
}


void Controller::setCurrentCrossSection( double depth_ )
{

    current_csection = depth_;
    if( main_csection == nullptr ) return;
    main_csection->setDepth( depth_ );

//    updateCurrentCrossSection();
    scene3d->updateCrossSection( main_csection );

    std::cout << "Current cross-section: " << depth_ << std::endl << std::flush;

}







bool Controller::addObject()
{

    Object* obj_ = new Object();
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

    object_tree->addObject( current_object, ObjectTreeItem::Type::STRATIGRAPHY,
                            obj_->getName(), current_color.r, current_color.g, current_color.b );
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





void Controller::initRulesProcessor()
{
    updateBoundingBoxRulesProcessor();
    rules_processor.removeAbove();
}


void Controller::updateBoundingBoxRulesProcessor()
{
    if( volume == nullptr ) return;

    double ox = 0.0, oy = 0.0, oz = 0.0;
    volume->getOrigin( ox, oy, oz );

    rules_processor.setOrigin( ox, oy, oz );
    rules_processor.setLenght( volume->getWidth(), volume->getHeight(), volume->getLenght() );
    rules_processor.setMediumResolution();
}




//bool Controller::setMainCrossSection( const CrossSection::Direction& dir_, double depth_ )
//{
//    bool status_ = addCrossSection( dir_, depth_ );
//    if( status_ == false )
//         return false;

//    main_csection = getCrossSection( depth_ );
//    return true;

//}

//bool Controller::addCrossSection( const CrossSection::Direction& dir_, double depth_ )
//{

//    CrossSection* cs_ = new CrossSection( volume, dir_, depth_ ) ;
//    bool status_ = actives_csections.addElement( depth_, cs_ );
//    if( status_ == false ) return false;


//    setCurrentCrossSection( depth_ );
//    scene3d->addCrossSection( cs_ );
//    return true;
//}


//bool Controller::removeCrossSection( const CrossSection::Direction& dir_, double depth_ )
//{

////    bool status_ = actives_csections.findElement( depth_ );
////    if( status_ == false ) return false;

////    actives_csections.removeElement( depth_ );

////    CrossSection* const& cs_ = actives_csections.getElement( depth_ );
////    scene3d->removeCrossSection( cs_ );

//    return true;

//}


//CrossSection* Controller::getCrossSection( const double& depth_ )
//{
//    if( actives_csections.findElement( depth_ ) == true )
//        return  actives_csections.getElement( depth_ );
//    else if( all_csections.findElement( depth_ ) == true )
//        return  all_csections.getElement( depth_ );
//    else
//        return nullptr;
//}




//double Controller::getCurrentCrossSection()
//{
//    return current_csection;
//}



//void Controller::updateCurrentCrossSection()
//{
//    std::vector< std::size_t > actives_ = rules_processor.getSurfaces();

//    for ( std::size_t id_: actives_ )
//    {
//        std::cout << "updating active " << id_ << std::endl << std::flush;
//        updateObjectCurveFromCrossSection( id_, current_csection );
//    }
//}


//void Controller::addTopViewCrossSection()
//{
//    topview_csection = new CrossSection( volume, CrossSection::Direction::Y,  volume->getHeight() );

//}


//CrossSection* Controller::getTopViewCrossSection()
//{
//    return topview_csection;

//}





//bool Controller::addObject( std::size_t index_ )
//{

//    Object* obj_ = new Object();
//    obj_->setIndex( index_ );
//    current_object = index_;

//    double w = 0, h = 0,  l = 0;
//    double minx_ = 0, miny_ = 0, minz_ = 0;

//    volume->getOrigin( minx_, miny_, minz_ );
//    volume->getGeometry( w, h, l );


//    obj_->setMaxMin( minx_ + w, miny_ + h, minz_ + l, minx_, miny_, minz_ );
//    obj_->setColor( current_color.r, current_color.g, current_color.b );

//    bool status_ = objects.addElement( current_object, obj_ );
//    if( status_ == false ) return false;

//    volume->addObject( current_object, obj_ );

//    object_tree->addObject( current_object, ObjectTreeItem::Type::STRATIGRAPHY,
//                            obj_->getName(), current_color.r, current_color.g, current_color.b );
//    scene3d->addObject( obj_ );

//    return true;
//}


//bool Controller::addObjectCurve( PolyCurve curve_ )
//{

//    if( objects.findElement( current_object ) == false )
//        return false;

//    Object* const& obj_ = objects.getElement( current_object );
//    bool status_ = obj_->addCurve( current_csection, curve_ );
//    if( status_ == false )
//        return false;

//    obj_->setEditable( true );
//    obj_->setVisible( true );


//    createPreviewSurface();

//    CrossSection* cs_ = getCrossSection( current_csection );
//    if( cs_ == nullptr )
//        cs_ = new CrossSection( volume, CrossSection::Direction::Z, current_csection );

//    cs_->addObject( obj_->getIndex(),  &curve_ );
//    volume->addCrossSection( cs_->getIndex(), cs_ );


//    if( all_csections.findElement( current_csection ) == false )
//        all_csections.addElement( current_csection, cs_ );


//    return true;

//}


//bool Controller::removeObjectCurve( double csection_ )
//{
//    if( objects.findElement( current_object ) == false )
//        return false;

//    Object* const& obj_ = objects.getElement( current_object );
//    bool status_ = obj_->removeCurve( csection_ );
//    if( status_ == false )
//        return false ;


////    if( all_csections.findElement( current_csection ) == false )
////        return true;


////    CrossSection* cs_ = getCrossSection( csection_ );
////    cs_->removeObjectCurve( obj_->getIndex() );

////    if( cs_->hasObjects() == false )
////        all_csections.removeElement( csection_ );


//    return true;
//}


//bool Controller::addObjectTrajectory( PolyCurve curve_ )
//{

//    if( objects.findElement( current_object ) == false )
//        return false;

//    Object* const& obj_ = objects.getElement( current_object );
//    bool status_ = obj_->addTrajectory( curve_ );
//    return status_;

//}


//void Controller::removeObjectTrajectory()
//{

//    if( objects.findElement( current_object ) == false )
//        return;

//    Object* const& obj_ = objects.getElement( current_object );
//    obj_->removeTrajectory();

//}


//void Controller::setObjectColor( std::size_t index_, int r_, int g_, int b_)
//{
//    if( objects.findElement( index_) == false )
//        return;

//    Object* const& obj_ = objects.getElement( index_ );
//    obj_->setColor( r_, g_, b_ );
//    scene3d->updateObject( index_ );
//    object_tree->updateObjectColor( index_, r_, g_, b_ );
//}


//void Controller::getObjectColor( std::size_t index_,int& r_, int& g_, int& b_)
//{
//    if( objects.findElement( index_) == false )
//        return;

//    Object* const& obj_ = objects.getElement( index_ );
//    obj_->getColor( r_, g_, b_ );
//}



//void Controller::setObjectName( std::size_t index_, const std::string& name_ )
//{
//    if( objects.findElement( index_) == false )
//        return;

//    Object* const& obj_ = objects.getElement( index_ );
//    obj_->setName( name_ );
//}


//std::string Controller::getObjectName( std::size_t index_ )
//{
//    if( objects.findElement( index_) == false )
//        return std::string();

//    Object* const& obj_ = objects.getElement( index_ );
//    return obj_->getName();
//}



//void  Controller::setObjectVisibility( std::size_t index_, bool status_ )
//{
//    if( objects.findElement( index_ ) == false )
//        return;

//    Object* const& obj_ = objects.getElement( index_ );
//    obj_->setVisible( status_ );
//    scene3d->updateObject( index_ );

//}


//bool  Controller::getObjectVisibility( std::size_t index_ )
//{
//    if( objects.findElement( index_ ) == false )
//        return false;

//    Object* const& obj_ = objects.getElement( index_ );
//    return obj_->isVisible();
//}


//void Controller::saveObjectInformation( std::size_t index_, const std::string & text_ )
//{
//    if( objects.findElement( index_ ) == false )
//        return;

//    Object* const& obj_ = objects.getElement( index_ );
//    obj_->saveInformation( text_ );
//}

//const std::string& Controller::getObjectInformation( std::size_t index_ )
//{
//    if( objects.findElement( index_ ) == false )
//        return std::string();

//    Object* const& obj_ = objects.getElement( index_ );
//    return obj_->getInformation();
//}


//void Controller::clearObjectInformation( std::size_t index_ )
//{
//    if( objects.findElement( index_ ) == false )
//        return;

//    Object* const& obj_ = objects.getElement( index_ );
//    obj_->clearInformation();
//}


//Object* Controller::getCurrentObject()
//{
//    return objects.getElement( current_object );
//}


//std::size_t Controller::getIndexCurrentObject() const
//{
//    return current_object;
//}



//bool Controller::createObjectSurface()
//{

//    Object* const& obj_ = objects.getElement( current_object );
//    Object::CrossSectionsContainer cs_ = obj_->getCrossSectionCurves();

//    std::vector< std::tuple< Curve2D, double > > curves_;
//    for ( Object::CrossSectionsContainer::Iterator it =  cs_.begin(); it != cs_.end(); ++it )
//    {
//        double csection_id_ = it->first;
//        PolyCurve sketch_ = it->second;

//        CrossSection* csection_ = all_csections.getElement( csection_id_ );
//        Curve2D curve_ = vectorToCurve2D( sketch_.getVertices() );

//        curves_.push_back( std::make_tuple( curve_, csection_id_ ) );
//    }

//    applyStratigraphicRule();

//    bool surface_created = false;
//    if( obj_->hasTrajectory() == true )
//    {
//        Curve2D curve_ = std::get<0>( curves_[ 0 ] );
//        Curve2D traj_ = vectorToCurve2D( obj_->getTrajectory().getVertices() );
//        double depth_ = std::get<1>( curves_[ 0 ] );

//        surface_created = rules_processor.extrudeAlongPath( current_object, curve_, depth_, traj_ );
//    }

//    else
//    {
//        surface_created = rules_processor.createSurface( current_object, curves_ );
//    }


//    if( surface_created == false ) return false;

//    obj_->removeCrossSectionCurves();
//    obj_->removeTrajectory();
//    obj_->setEditable( false );

//    updateModel();

//    return true;

//}



//bool Controller::createPreviewSurface()
//{
//    Object* const& obj_ = objects.getElement( current_object );
//    Object::CrossSectionsContainer cs_ = obj_->getCrossSectionCurves();

//    std::vector< std::tuple< Curve2D, double > > curves_;
//    for ( Object::CrossSectionsContainer::Iterator it =  cs_.begin(); it != cs_.end(); ++it )
//    {
//        double csection_id_ = it->first;
//        PolyCurve sketch_ = it->second;

//        Curve2D curve_ = vectorToCurve2D( sketch_.getVertices() );
//        curves_.push_back( std::make_tuple( curve_, csection_id_ ) );
//    }


//    bool surface_created = rules_processor.testSurface( current_object, curves_ );
//    if( surface_created == false ) return false;

//    std::vector< double > vertices_;
//    std::vector< std::size_t > faces_;

//    bool has_surface = rules_processor.getMesh( current_object, vertices_, faces_ );
//    if( has_surface  == false ) return false;


//    std::vector< double > normals_;
//    rules_processor.getNormals( current_object, normals_ );


//    Surface surface;
//    surface.setVertices( vertices_ );
//    surface.setFaces( faces_ );
//    surface.setNormals( normals_ );

//    obj_->setActive( true );
//    obj_->setSurface( surface );
//    obj_->setVisible( true );

//    scene3d->updateObject( current_object );
//    return true;

//}


//void Controller::updateModel()
//{

//    std::vector< std::size_t > actives_ = rules_processor.getSurfaces();

//    for ( Container< std::size_t, Object* >::Iterator it =  objects.begin(); it != objects.end(); ++it )
//    {
//        Object* obj_ = (it->second);
//        obj_->setActive( false );
//        object_tree->setObjectVisibility( it->first, false );
//        scene3d->updateObject( it->first );
//    }

//    for ( std::size_t id_: actives_ )
//    {
//        updateObjectSurfaces( id_ );

//        for ( Container< double, CrossSection* >::Iterator cs_it =  actives_csections.begin(); cs_it != actives_csections.end(); ++cs_it )
//        {
//            updateObjectCurveFromCrossSection( id_, cs_it->first );
//        }

//        updateObjectCurveFromCrossSection( id_, current_csection );

//    }

//}


//void Controller::updateObjectCurveFromCrossSection( std::size_t object_id_, double csection_id_ )
//{
//    Object* obj_ = objects.getElement( object_id_ );

//    std::vector< double > vertices_;
//    std::vector< std::size_t > edges_;
//    bool has_curve = rules_processor.getCrossSection( object_id_, indexCrossSection( csection_id_ ) ,
//                                                      vertices_, edges_ );

//    if( has_curve == false )
//    {
//        obj_->removeCurve( csection_id_ );
//        return;
//    }



//    PolyCurve curve_ = PolyCurve( vertices_, edges_ );
//    obj_->updateCurve( csection_id_, curve_ );


//    if( all_csections.findElement( csection_id_ ) == false )
//        return;

//    CrossSection* csection_ = all_csections.getElement( csection_id_ );
//    if( has_curve == true )
//        csection_->addObject( object_id_, &curve_ );
//    else
//        csection_->removeObjectCurve( object_id_ );



//}


//void Controller::updateObjectSurfaces( std::size_t object_id_ )
//{

//    Object* obj_ = objects.getElement( object_id_ );

//    std::vector< double > vertices_;
//    std::vector< std::size_t > faces_;

//    bool has_surface = rules_processor.getMesh( object_id_, vertices_, faces_ );
//    if( has_surface  == false ) return;


//    std::vector< double > normals_;
//    rules_processor.getNormals( object_id_, normals_ );


//    Surface surface;
//    surface.setVertices( vertices_ );
//    surface.setFaces( faces_ );
//    surface.setNormals( normals_ );


//    std::vector< double > path_;
//    bool has_path = rules_processor.getExtrusionPath( object_id_, path_ );
//    if( has_path == true )
//    {
//        obj_->removeTrajectory();
//        obj_->addTrajectory( PolyCurve( path_ ) );
//    }

//    obj_->setActive( true );
//    obj_->setSurface( surface );
//    obj_->setVisible( true );
//    object_tree->setObjectVisibility( object_id_, true );

//    scene3d->updateObject( object_id_ );
//}




//void Controller::initRulesProcessor()
//{
//    updateBoundingBoxRulesProcessor();
//    rules_processor.removeAbove();
//}


//void Controller::updateBoundingBoxRulesProcessor()
//{
//    if( volume == nullptr ) return;

//    double ox = 0.0, oy = 0.0, oz = 0.0;
//    volume->getOrigin( ox, oy, oz );

//    rules_processor.setOrigin( ox, oy, oz );
//    rules_processor.setLenght( volume->getWidth(), volume->getHeight(), volume->getLenght() );
//    rules_processor.setMediumResolution();
//}


//std::size_t Controller::setupCrossSectionDiscretization()
//{
//    if( volume == nullptr ) return 0;

//    std::size_t disc_ = rules_processor.getDepthResolution();
//    csection_step = static_cast< double >( volume->getLenght()/disc_ );
//    return disc_;
//}


//std::size_t Controller::indexCrossSection( double value_ ) const
//{
//    return static_cast< std::size_t > ( value_/csection_step );
//}


//double Controller::depthCrossSection( std::size_t index_ ) const
//{
//    return static_cast< double > ( index_*csection_step );
//}




//void Controller::setRemoveAbove()
//{
//    current_rule = StratigraphicRules::REMOVE_ABOVE;
//}

//void Controller::setRemoveAboveIntersection()
//{
//    current_rule = StratigraphicRules::REMOVE_ABOVE_INTERSECTION;
//}

//void Controller::setRemoveBelow()
//{
//    current_rule = StratigraphicRules::REMOVE_BELOW;
//}

//void Controller::setRemoveBelowIntersection()
//{
//    current_rule = StratigraphicRules::REMOVE_BELOW_INTERSECTION;
//}


//void Controller::applyStratigraphicRule()
//{
//    if( current_rule == StratigraphicRules::REMOVE_ABOVE )
//        rules_processor.removeAbove();
//    else if( current_rule == StratigraphicRules::REMOVE_ABOVE_INTERSECTION )
//        rules_processor.removeAboveIntersection();
//    else if( current_rule == StratigraphicRules::REMOVE_BELOW )
//        rules_processor.removeBelow();
//    else if( current_rule == StratigraphicRules::REMOVE_BELOW_INTERSECTION )
//        rules_processor.removeBelowIntersection();
//}




//bool Controller::enableCreateAbove( bool status_ )
//{
//    setObjectsAsSelectable( selectable_upper, false );

//    if( status_ == false )
//    {
//        stopCreateAbove();
//        return false;
//    }

//    return requestCreateAbove();

//}


//void Controller::stopCreateAbove()
//{
//    std::cout << "Stop create above accepted" << std::endl << std::flush;
//    rules_processor.stopDefineAbove();
//    setObjectAsSelected( index_upper_boundary, false );
//}


//bool Controller::requestCreateAbove()
//{

//    bool request_ = rules_processor.requestCreateAbove( selectable_upper );

//    if( request_ == true )
//    {
//        std::cout << "Request create accepted" << std::endl << std::flush;

//        setObjectsAsSelectable( selectable_bottom, false );
//        setObjectsAsSelectable( selectable_upper, true );

//        boundering_region = BounderingRegion::ABOVE;

//    }
//    else
//        std::cout << "Request create denied" << std::endl << std::flush;

//    return request_ ;

//}



//bool Controller::enableCreateBelow( bool status_ )
//{

//    setObjectsAsSelectable( selectable_upper, false );

//    if( status_ == false )
//    {
//        stopCreateBelow();
//        return false;
//    }

//    return requestCreateBelow();

//}


//void Controller::stopCreateBelow()
//{
//    std::cout << "Stop create below accepted" << std::endl << std::flush;
//    rules_processor.stopDefineBelow();
//    setObjectAsSelected( index_bottom_boundary, false );

//}


//bool Controller::requestCreateBelow()
//{

//    bool request_ = rules_processor.requestCreateBelow( selectable_bottom );

//    if( request_ == true )
//    {
//        std::cout << "Request create accepted" << std::endl << std::flush;

//        setObjectsAsSelectable( selectable_upper, false );
//        setObjectsAsSelectable( selectable_bottom, true );
//        boundering_region = BounderingRegion::BELOW;

//    }
//    else
//        std::cout << "Request create denied" << std::endl << std::flush;

//    return request_ ;

//}



//void Controller::setObjectsAsSelectable( std::vector< std::size_t >& indexes_,
//                                                    bool status_ )
//{

//    for( std::size_t id_: indexes_ )
//    {
//        if( objects.findElement( id_ ) == false ) continue;

//        Object* obj_ = objects.getElement( id_ );
//        obj_->setSelectable( status_ );
//    }

//    if( status_ == false )
//        indexes_.clear();
//}


//void Controller::setObjectAsSelected( std::size_t index_, bool status_ )
//{

//    if( objects.findElement( index_ ) == false ) return;

//    Object* obj_ = objects.getElement( index_ );
//    obj_->setSelected( status_ );
//}


//void Controller::getObjectsAsUpperBoundering( std::vector< std::size_t >& indexes_)
//{
//    indexes_ = selectable_upper;
//}


//void Controller::setObjectAsBoundering( std::size_t index_ )
//{

//    if( objects.findElement( index_ ) == false ) return;


//    setObjectAsSelected( index_, true );

//    if( boundering_region == BounderingRegion::ABOVE )
//    {
//        index_upper_boundary = index_;
//        rules_processor.defineAbove( index_ );
//        setObjectsAsSelectable( selectable_upper, false );
//    }
//    else if( boundering_region == BounderingRegion::BELOW )
//    {
//        index_bottom_boundary = index_;
//        rules_processor.defineBelow( index_ );
//        setObjectsAsSelectable( selectable_bottom, false );
//    }

//}


//bool Controller::undo()
//{
//    bool undo_done = rules_processor.undo();
//    if( undo_done == false ) return false;

//    updateModel();
//    return true;
//}

//bool Controller::redo()
//{
//    bool redo_done = rules_processor.redo();
//    if( redo_done == false ) return false;

//    updateModel();
//    return true;
//}


//bool Controller::canUndo()
//{
//    return rules_processor.canUndo();
//}

//bool Controller::canRedo()
//{
//    return rules_processor.canRedo();
//}


//bool Controller::isDefineAboveActive()
//{
//    std::size_t index_;

//    bool status_ = rules_processor.defineAboveIsActive( index_ );
//    if( status_ == false )
//    {
//        setObjectAsSelected( index_upper_boundary, false );
//        return false;
//    }

//    boundering_region = BounderingRegion::ABOVE;
//    setObjectAsBoundering( index_ );
//    return true;

//}



//bool Controller::isDefineBelowActive()
//{
//    std::size_t index_;

//    bool status_ = rules_processor.defineBelowIsActive( index_ );
//    if( status_ == false )
//    {
//        setObjectAsSelected( index_bottom_boundary, false );
//        return false;
//    }

//    boundering_region = BounderingRegion::BELOW;
//    setObjectAsBoundering( index_ );
//    return true;
//}



//void Controller::getOutputVolume()
//{
//    std::vector< double > vertices_;
//    std::vector< std::vector< std::size_t > > regions_;
//    rules_processor.getTetrahedralMesh( vertices_, regions_ );

//    Volume* vol1_ = new Volume();
//    vol1_->setVertices( vertices_ );

//    std::random_device rd;
//    std::mt19937 eng( rd() );
//    std::uniform_int_distribution< size_t > distr( 0, 255 );




//    std::size_t number_of_regions = regions_.size();
//    for( std::size_t i = 0; i < number_of_regions; ++i )
//    {
//        Volume::Color color_;
//        color_.r = distr( eng );
//        color_.b = distr( eng );
//        color_.g = distr( eng );

//        vol1_->addRegion( i, regions_[ i ], color_ );
//    }
//    scene3d->addOutputVolume( vol1_ );
//}


//void Controller::saveFile( const std::string& filename )
//{
//    rules_processor.saveFile( filename );
//}

//void Controller::loadFile( const std::string& filename )
//{
//    rules_processor.loadFile( filename );
//    loadObjects();
//}


//void Controller::loadObjects()
//{
//    if( volume == nullptr ) return;

//    double ox, oy, oz;
//    double width, height, depth;

//    rules_processor.getOrigin( ox, oy, oz );
//    rules_processor.getLenght( width, height, depth );


//    volume->setOrigin( ox, oy, oz );
//    setVolumeDimensions( width, height, depth );

//    if( objects.findElement( current_object ) == true )
//    {
//        objects.removeElement( current_object );
////        scene3d->removeObject( current_object );
//    }

//    std::vector< std::size_t > actives = rules_processor.getSurfaces();
//    std::size_t number_of_objects = actives.size();
//    std::vector< int > colors = createVectorOfColors( number_of_objects );

//    std::size_t i = 0;
//    for( auto id: actives )
//    {
//        addObject( id );
//        setObjectColor( id, colors[ 3*i ], colors[ 3*i + 1 ], colors[ 3*i + 2 ] );
////        addObjectToInterface();
//        ++i;
//    }

//    addObject();
//    updateModel();

//}


//std::vector< int > Controller::createVectorOfColors( std::size_t number_of_colors )
//{

//    std::vector< int > colors;
//    colors.resize( 3*number_of_colors );

//    for( std::size_t i = 0; i < number_of_colors; ++i )
//    {
//        std::random_device rd;
//        std::mt19937 eng( rd() );
//        std::uniform_int_distribution< size_t > distr( 0, 255 );
//        colors[ 3*i ] = distr( eng );
//        colors[ 3*i + 1 ] = distr( eng );
//        colors[ 3*i + 2 ] = distr( eng );
//    }

//    return colors;
//}


//void Controller::clear()
//{

//    scene3d->clear();
//    volume->clear();


//    std::vector< double > diff;
//    for (  Container< double, CrossSection* >::Iterator it =  all_csections.begin(); it != all_csections.end(); ++it )
//    {
//        CrossSection* &csection_ = all_csections.getElement( it->first );
//        if( csection_ == nullptr ) continue;

//        if( actives_csections.findElement( it->first ) == true )
//        {
//            diff.push_back( it->first );
//            continue;
//        }

//        csection_->clear();
//        all_csections.deleteElement( it->first );

//    }
//    all_csections.clear();


//    for (  auto d: diff )
//    {
//        CrossSection* &csection_ = actives_csections.getElement( d );
//        if( csection_ == nullptr ) continue;

//        csection_->clear();
//        actives_csections.deleteElement( d );

//    }
//    actives_csections.clear();



//    if( main_csection != nullptr )
//    {
//        main_csection = nullptr;
//    }

//    if( topview_csection != nullptr )
//    {
//        topview_csection->clear();
//        delete topview_csection;
//        topview_csection = nullptr;
//    }

//    object_tree->clear();


//    for (  Container< std::size_t, Object* >::Iterator it =  objects.begin(); it != objects.end(); ++it )
//    {
//        Object* &obj_ = objects.getElement( it->first );
//        if( obj_ == nullptr ) continue;
//        objects.deleteElement( it->first );
//    }
//    objects.clear();



//    selectable_upper.clear();
//    selectable_bottom.clear();

//    rules_processor.clear();

//    initializeData();

//}


//void Controller::initializeData()
//{
//    main_csection = nullptr;
//    topview_csection = nullptr;
//    volume = nullptr;
//    current_color.r = 255;
//    current_color.g = 0;
//    current_color.b = 0;

//    current_object = 0;
//    current_csection = 500.0;

//    csection_step = 0.0;
//    current_rule = StratigraphicRules::REMOVE_ABOVE;
//    boundering_region = BounderingRegion::ABOVE;

//    index_upper_boundary = 0;
//    index_bottom_boundary = 0;

//}
