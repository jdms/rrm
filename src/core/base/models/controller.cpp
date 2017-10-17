#include "controller.h"

#include "volume.h"
#include "object.h"



void Controller::setScene3d( Scene3d* const& sc_ )
{
    scene3d = sc_;
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
}


void Controller::setVolumeDimensions( const double& width_, const double& height_, const double& length_ )
{
    volume->setGeometry( width_, height_, length_ );
    scene3d->updateVolume();
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
    Object* obj_ = new Object();
    current_object = obj_->getIndex();

    objects.addElement( current_object, obj_ );
    volume->addObject( current_object, obj_ );
}


void Controller::addObjectCurve( PolyCurve curve_ )
{
    Object* const& obj_ = objects.getElement( current_object );
    obj_->addCurve( current_csection, curve_ );

    CrossSection* cs_ = getCurrentCrossSection();
    cs_->addObject( obj_->getIndex(),  &curve_ );
}


void Controller::removeObjectCurve( std::size_t csection_ )
{
    Object* const& obj_ = objects.getElement( current_object );
    obj_->removeCurve( csection_ );

    CrossSection* cs_ = getCurrentCrossSection();
    cs_->removeObjectCurve( obj_->getIndex() );
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
