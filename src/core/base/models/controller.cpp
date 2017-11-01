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


void Controller::acceptVolumeDimensions( CrossSection::Direction dir_, double w_, double h_ )
{
    if( dir_ == CrossSection::Direction::Z )
        setVolumeDimensions( w_, h_, volume->getLenght() );
    else if( dir_ == CrossSection::Direction::X )
        setVolumeDimensions( volume->getWidth(), h_, w_ );
    else if( dir_ == CrossSection::Direction::Y )
        setVolumeDimensions( w_, volume->getHeight(), h_ );

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


bool Controller::setMainCrossSection( const CrossSection::Direction& dir_, double depth_ )
{
    bool status_ = addCrossSection( dir_, depth_ );
    if( status_ == false )
         return false;

    main_csection = getActiveCrossSection( depth_ );
    return true;

}

bool Controller::addCrossSection( const CrossSection::Direction& dir_, double depth_ )
{
    CrossSection* cs_ = new CrossSection( volume, dir_, depth_ ) ;
    bool status_ = actives_csections.addElement( depth_, cs_ );
    if( status_ == false ) return false;

    setCurrentCrossSection( depth_ );
    scene3d->addCrossSection( cs_ );
    return true;
}


bool Controller::removeCrossSection( const CrossSection::Direction& dir_, double depth_ )
{

    bool status_ = actives_csections.findElement( depth_ );
    if( status_ == false ) return false;

    actives_csections.removeElement( depth_ );

    CrossSection* const& cs_ = actives_csections.getElement( depth_ );
    scene3d->removeCrossSection( cs_ );

    return true;

}


CrossSection* Controller::getActiveCrossSection( const double& depth_ )
{
    return actives_csections.getElement( depth_ );
}

CrossSection* Controller::getCrossSection( const double& depth_ )
{
    return all_csections.getElement( depth_ );
}


void Controller::setCurrentCrossSection( const double& depth_ )
{

//    std::cout << "CrossSection depth: " << depth_ << ", id: " << indexCrossSection( depth_ )
//              << std::endl << std::flush;


    // its not needeed. One cross-section can be current and doesnt exist into vectors of cross-sections;
//    bool status_ = actives_csections.findElement( depth_ );
//    if( status_ == false ) return;

    current_csection = depth_;
    if( main_csection != nullptr )
        main_csection->setDepth( depth_ );
}


double Controller::getCurrentCrossSection()
{
    return current_csection;
}







bool Controller::addObject()
{

    double w = 0, h = 0,  l = 0;
    double minx_ = 0, miny_ = 0, minz_ = 0;


    volume->getOrigin( minx_, miny_, minz_ );
    volume->getGeometry( w, h, l );

    Object* obj_ = new Object();
    current_object = obj_->getIndex();
    obj_->setMaxMin( minx_ + w, miny_ + h, minz_ + l, minx_, miny_, minz_ );
    obj_->setColor( current_color.r, current_color.g, current_color.b );

    bool status_ = objects.addElement( current_object, obj_ );
    if( status_ == false ) return false;

    volume->addObject( current_object, obj_ );

    object_tree->addObject( current_object, ObjectTreeItem::Type::STRATIGRAPHY,
                            obj_->getName(), current_color.r, current_color.g, current_color.b );
    scene3d->addObject( obj_ );

    return true;
}


bool Controller::addObjectCurve( PolyCurve curve_ )
{

    if( objects.findElement( current_object ) == false )
        return false;

    Object* const& obj_ = objects.getElement( current_object );
    bool status_ = obj_->addCurve( current_csection, curve_ );
    if( status_ == false )
        return false;

    obj_->setEditable( true );

    CrossSection* cs_;

    if( actives_csections.findElement( current_csection ) == true )
        cs_ = actives_csections.getElement( current_csection );
    else if( all_csections.findElement( current_csection ) == true )
        cs_ = all_csections.getElement( current_csection );
    else
        cs_ = new CrossSection( volume, CrossSection::Direction::Z, current_csection );


    cs_->addObject( obj_->getIndex(),  &curve_ );


    if( all_csections.findElement( current_csection ) == false )
        all_csections.addElement( current_csection, cs_ );

    createObjectSurface();


    return true;

}


bool Controller::removeObjectCurve( double csection_ )
{
    if( objects.findElement( current_object ) == false )
        return false;

    Object* const& obj_ = objects.getElement( current_object );
    bool status_ = obj_->removeCurve( csection_ );
    if( status_ == false )
        return false ;


    if( all_csections.findElement( current_csection ) == false )
        return true;


    CrossSection* cs_ = getCrossSection( csection_ );
    cs_->removeObjectCurve( obj_->getIndex() );

    if( cs_->hasObjects() == false )
        all_csections.removeElement( csection_ );


    return true;
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


void Controller::getObjectColor( std::size_t index_,int& r_, int& g_, int& b_)
{
    if( objects.findElement( index_) == false )
        return;

    Object* const& obj_ = objects.getElement( index_ );
    obj_->getColor( r_, g_, b_ );
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


bool  Controller::getObjectVisibility( std::size_t index_ )
{
    if( objects.findElement( index_ ) == false )
        return false;

    Object* const& obj_ = objects.getElement( index_ );
    return obj_->isVisible();
}



Object* Controller::getCurrentObject()
{
    return objects.getElement( current_object );
}


std::size_t Controller::getIndexCurrentObject() const
{
    return current_object;
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

        CrossSection* csection_ = actives_csections.getElement( csection_id_ );
        Curve2D curve_ = vectorToCurve2D( sketch_.getVertices() );

        curves_.push_back( std::make_tuple( curve_, csection_->getDepth() ) );
    }

    rules_processor.removeAbove();

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






void Controller::updateModel()
{

    std::vector< std::size_t > actives_ = rules_processor.getSurfaces();

    for ( Container< std::size_t, Object* >::Iterator it =  objects.begin(); it != objects.end(); ++it )
    {
        Object* obj_ = (it->second);
        obj_->setActive( false );
        object_tree->setObjectVisibility( it->first, false );
        scene3d->updateObject( it->first );
    }

    for ( std::size_t id_: actives_ )
    {

        updateObjectSurfaces( id_ );

        for ( Container< double, CrossSection* >::Iterator cs_it =  actives_csections.begin(); cs_it != actives_csections.end(); ++cs_it )
        {
            updateObjectCurveFromCrossSection( id_, cs_it->first );
        }
    }

}


void Controller::updateObjectCurveFromCrossSection( std::size_t object_id_, double csection_id_ )
{
    Object* obj_ = objects.getElement( object_id_ );
    CrossSection* csection_ = actives_csections.getElement( csection_id_ );

    std::vector< double > vertices_;
    std::vector< std::size_t > edges_;
    bool has_curve = rules_processor.getCrossSection( object_id_, indexCrossSection( csection_id_ ) ,
                                                      vertices_, edges_ );

    if( has_curve == false )
    {
//        std::cout << "Remove curve of object " << object_id_ << ", from csection " << csection_id_ <<
//                     std::endl << std::flush;

        obj_->removeCurve( csection_id_ );
        csection_->removeObjectCurve( object_id_ );
        return;
    }


    PolyCurve curve_ = PolyCurve( vertices_, edges_ );


    obj_->updateCurve( csection_id_, curve_ );
    csection_->addObject( object_id_, &curve_ );

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
    object_tree->setObjectVisibility( object_id_, true );

    scene3d->updateObject( object_id_ );
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


std::size_t Controller::setupCrossSectionDiscretization()
{
    if( volume == nullptr ) return 0;

    std::size_t disc_ = rules_processor.getDepthResolution();
    csection_step = static_cast< double >( volume->getLenght()/disc_ );
    return disc_;
}


std::size_t Controller::indexCrossSection( double value_ ) const
{
    return static_cast< std::size_t > ( value_/csection_step );
}


double Controller::depthCrossSection( std::size_t index_ ) const
{
    return static_cast< double > ( index_*csection_step );
}




