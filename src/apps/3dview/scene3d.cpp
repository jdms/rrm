#include "scene3d.h"


#include "./core/base/models/object.h"
#include "./core/base/models/volumeshader.h"
#include "./core/base/models/planeshader.h"


#include <QString>
#include <QOpenGLContext>
#include <QSurface>


Scene3d::Scene3d()
{
    volume = nullptr;
}



void Scene3d::addVolume( Volume* const& raw_ )
{
    context->makeCurrent( surface );

    clearVolume();
    volume = new VolumeShader( raw_ );

    emit updateCanvas();

}

void Scene3d::clearVolume()
{
    if( volume != nullptr )
        delete volume;
    volume = nullptr;
}


void Scene3d::addCrossSection( CrossSection* const& raw_ )
{
    std::size_t index_ = raw_->getIndex();
    csections.addElement( index_, new PlaneShader( raw_ ) );

    emit updateCanvas();
}


void Scene3d::removeCrossSection( CrossSection* const& raw_ )
{
    std::size_t index_ = raw_->getIndex();
    csections.removeElement( index_ );

    emit updateCanvas();
}




void Scene3d::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                               const int& h )
{
    if( volume != nullptr )
        volume->draw( V, P, w, h );

    for ( CrossSectionsContainer::Iterator it =  csections.begin(); it != csections.end(); ++it )
        (it->second)->draw( V, P, w, h );

}




void Scene3d::setCurrentDirectory( const QString& dir )
{
    shader_directory = dir;
}


void Scene3d::setOpenGLContext( QOpenGLContext* ctxt )
{
    context = ctxt;
    surface = ctxt->surface();
}



void Scene3d::clear()
{
    clearData();
    current_color = Qt::red;
    emit updateCanvas();
}


void Scene3d::clearData()
{
    current_color = Qt::red;

}

