#include "scene3d.h"


#include "./core/base/models/object.h"
#include "./core/base/models/volumeshader.h"
#include "./core/base/models/planeshader.h"
#include "./core/base/models/surfaceshader.h"


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


void Scene3d::updateVolume()
{
    if( volume == nullptr ) return;

    volume->update();
    for ( CrossSectionsContainer::Iterator it =  csections.begin(); it != csections.end(); ++it )
    {
        (it->second)->update();
    }

    emit updateCanvas();

}

void Scene3d::clearVolume()
{
    if( volume != nullptr )
        delete volume;
    volume = nullptr;
}


void Scene3d::addOutputVolume( Volume* const& raw_ )
{
    context->makeCurrent( surface );

    clearOutputVolume();
    output_volume = new VolumeShader( raw_ );

    emit updateCanvas();

}


void Scene3d::updateOutputVolume()
{
    if( output_volume == nullptr ) return;

    output_volume->update();
    emit updateCanvas();

}

void Scene3d::clearOutputVolume()
{
    if( output_volume != nullptr )
        delete output_volume;
    output_volume = nullptr;
}



void Scene3d::addCrossSection( CrossSection* const& raw_ )
{
    context->makeCurrent( surface );

    std::size_t index_ = raw_->getIndex();
    csections.addElement( index_, new PlaneShader( raw_ ) );

    emit updateCanvas();
}


void Scene3d::updateCrossSection( CrossSection* const& raw_ )
{
    context->makeCurrent( surface );

    PlaneShader* csection_ = csections.getElement( raw_->getIndex() );
    csection_->update();

    emit updateCanvas();
}


void Scene3d::removeCrossSection( CrossSection* const& raw_ )
{
    context->makeCurrent( surface );

    csections.removeElement( raw_->getIndex() );
    emit updateCanvas();
}


void Scene3d::addObject(  Object* const& raw_ )
{
    context->makeCurrent( surface );

    objects.addElement( raw_->getIndex(), new SurfaceShader( raw_ ) );
    emit updateCanvas();

}

void Scene3d::updateObject( const std::size_t &index_ )
{

    SurfaceShader* obj_ = objects.getElement( index_ );
    obj_->update();

    emit updateCanvas();
}


void Scene3d::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                               const int& h )
{

    if( volume != nullptr )
        volume->draw( V, P, w, h );

    for ( CrossSectionsContainer::Iterator it =  csections.begin(); it != csections.end(); ++it )
    {
        (it->second)->draw( V, P, w, h );
    }

    for ( ObjectsContainer::Iterator it =  objects.begin(); it != objects.end(); ++it )
    {
        (it->second)->draw( V, P, w, h );
    }

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

    for ( CrossSectionsContainer::Iterator it =  csections.begin(); it != csections.end(); ++it )
    {
        PlaneShader* csection_ = csections.getElement( it->first );
        if( csection_ == nullptr ) continue;

        csection_->clear();
        delete csection_;
        csection_ = nullptr;
    }
    csections.clear();

    for ( ObjectsContainer::Iterator it =  objects.begin(); it != objects.end(); ++it )
    {

        SurfaceShader* obj_ = objects.getElement( it->first );
        if( obj_ == nullptr ) continue;

        obj_->clear();
        delete obj_;
        obj_ = nullptr;
    }
    objects.clear();

    if( volume != nullptr )
    {
        volume->clear();
        delete volume;
        volume = nullptr;
    }


//    clearData();
    emit updateCanvas();
}


void Scene3d::clearData()
{

//    volume = nullptr;
    current_color = Qt::red;
    shader_directory.clear();

}

