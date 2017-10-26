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




void Controller::addCrossSection( const CrossSection::Direction& dir_, double depth_ )
{
    actives_csections.addElement( depth_, new CrossSection( volume, dir_, depth_ ) );
    setCurrentCrossSection( depth_ );

    scene3d->addCrossSection( getCurrentCrossSection() );
}


void Controller::removeCrossSection( const CrossSection::Direction& dir_, double depth_ )
{

    bool status_ = actives_csections.findElement( depth_ );
    if( status_ == false ) return;

    CrossSection* const& cs_ = actives_csections.getElement( depth_ );
    actives_csections.removeElement( cs_->getIndex() );

    scene3d->removeCrossSection( cs_ );
}


void Controller::setCurrentCrossSection( const double& depth_ )
{
    bool status_ = actives_csections.findElement( depth_ );
    if( status_ == false ) return;

    current_csection = depth_;
}


CrossSection* Controller::getCurrentCrossSection()
{
    return getCrossection( current_csection );
}


CrossSection* Controller::getCrossection( const double& depth_ )
{
    return actives_csections.getElement( depth_ );
}




void Controller::addObject()
{


    double w = 0, h = 0,  l = 0;
    double minx_ = 0, miny_ = 0, minz_ = 0;

    volume->getOrigin( minx_, miny_, minz_ );
    volume->getGeometry( w, h, l );

    double maxx_ = minx_ + w;
    double maxy_ = miny_ + h;
    double maxz_ = minz_ + l;


    Object* obj_ = new Object();
    current_object = obj_->getIndex();
    obj_->setMaxMin( maxx_, maxy_, maxz_, minx_, miny_, minz_ );

    objects.addElement( current_object, obj_ );
    volume->addObject( current_object, obj_ );


    object_tree->addObject( current_object, ObjectTreeItem::Type::STRATIGRAPHY,
                            obj_->getName(), 255, 0, 0 );
    scene3d->addObject( obj_ );
}


void Controller::addObjectCurve( PolyCurve curve_ )
{
    Object* const& obj_ = objects.getElement( current_object );
    obj_->addCurve( current_csection, curve_ );

    CrossSection* cs_ = getCurrentCrossSection();
    cs_->addObject( obj_->getIndex(),  &curve_ );

    createObjectSurface();
    scene3d->updateObject( obj_->getIndex() );
}


void Controller::removeObjectCurve( std::size_t csection_ )
{
    Object* const& obj_ = objects.getElement( current_object );
    obj_->removeCurve( csection_ );

    CrossSection* cs_ = getCurrentCrossSection();
    cs_->removeObjectCurve( obj_->getIndex() );
}


void  Controller::setObjectVisibility( std::size_t index_, bool status_ )
{
    Object* const& obj_ = objects.getElement( index_ );
    obj_->setVisible( status_ );
    scene3d->updateObject( index_ );

}


bool  Controller::getObjectVisibility( std::size_t index_ )
{
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
        std::size_t csection_id_ = it->first;
        PolyCurve sketch_ = it->second;

        CrossSection* csection_ = actives_csections.getElement( csection_id_ );
        Curve2D curve_ = vectorToCurve2D( sketch_.getVertices() );

        curves_.push_back( std::make_tuple( curve_, csection_->getDepth() ) );
    }


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

    updateModel();

    return true;

}






void Controller::updateModel()
{


    for ( Container< std::size_t, Object* >::Iterator it =  objects.begin(); it != objects.end(); ++it )
    {

        updateObjectSurfaces( it->first );

//        for ( Container< double, CrossSection* >::Iterator cs_it =  actives_csections.begin(); cs_it != actives_csections.end(); ++cs_it )
//        {

//            updateObjectCurveFromCrossSection( it->first, (cs_it->second)->getIndex() );

//        }

    }
}


void Controller::updateObjectCurveFromCrossSection( std::size_t object_id_, std::size_t csection_id_ )
{
    Object* obj_ = objects.getElement( object_id_ );
    CrossSection* csection_ = actives_csections.getElement( csection_id_ );

    std::vector< double > vertices_;
    std::vector< std::size_t > edges_;
    bool has_curve = rules_processor.getCrossSection( object_id_, indexCrossSection( csection_->getDepth() ) ,
                                                      vertices_, edges_ );

    if( has_curve == false ) return;

    obj_->addCurve( csection_id_, PolyCurve( vertices_, edges_ ) );

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

    obj_->setSurface( surface );
    obj_->setVisible( true );

    scene3d->updateObject( object_id_ );
}




void Controller::initRulesProcessor()
{
    updateBoundingBoxRulesProcessor();
    rules_processor.removeAboveIntersection();
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





/*

void Controller::buildObjectSurface()
{
    Object* const& obj = objects[ current_object ];


    //recuperar todas as curvas dos cross-sections

    //verificar se tem trajetoria: se sim, pegar

    //passar tudo para a rules_processor

    //se gerar a surface for falso, retorna

    // senao, remove todos os cross-sections do objeto

    // atualiza todos os objetos dos cross-sections "visiveis?"

    // retorna true;

}
*/
