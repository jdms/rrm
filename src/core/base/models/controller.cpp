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
}


void Controller::addVolume()
{
    volume = new Volume();
    scene3d->addVolume( volume );

}


void Controller::addCrossSection( const CrossSection::Direction& dir_, double depth_ )
{
    actives_csections.addElement( depth_, new CrossSection( volume, dir_, depth_ ) );
    current_csection = depth_;

    scene3d->addCrossSection( getCurrentCrossSection() );
}



CrossSection* Controller::getCurrentCrossSection()
{
    return actives_csections.getElement( current_csection );
}





/*
void Controller::addObject()
{
    Object* obj = new Object();
    std::size_t index_ = obj->getIndex();

    objects[ index_ ] = obj;
    volume->addObject( index_ );
}


void Controller::addObjectCurve( Curve2D* const& curve_ )
{
    Object* const& obj = objects[ current_object ];
    obj->addCurve( current_csection, curve_ );
    active_csection[ current_csection ]->addObject( obj, curve_ );
}


void Controller::removeObjectCurve( std::size_t csection_ )
{
    Object* const& obj = objects[ current_object ];
    obj->removeCurve( current_csection );
    active_csection[ current_csection ]->removeObject( obj );
}


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
