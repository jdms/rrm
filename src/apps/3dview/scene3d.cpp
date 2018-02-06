#include "scene3d.h"


#include "./core/models/object.h"
#include "./models/volumeshader.h"
#include "./models/planeshader.h"
#include "./models/surfaceshader.h"
#include "./models/regionshader.h"


#include <QString>
#include <QOpenGLContext>
#include <QSurface>


Scene3d::Scene3d()
{
    volume = nullptr;
    output_volume = nullptr;
    main_csection = nullptr;
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

    if( main_csection != nullptr )
    {
        main_csection->update();
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


void Scene3d::addRegion( Regions* const& raw_ )
{
    context->makeCurrent( surface );

    std::size_t index_ = raw_->getIndex();
    regions.addElement( index_, new RegionShader( raw_ ) );

    emit updateCanvas();

}


void Scene3d::updateRegion( std::size_t index_ )
{
    context->makeCurrent( surface );

    if( regions.findElement( index_ ) == false ) return;
    RegionShader* region_ = regions.getElement( index_ );
    region_->update();

    emit updateCanvas();

}

void Scene3d::updateRegions()
{
    for ( RegionsContainer::Iterator it =  regions.begin(); it != regions.end(); ++it )
    {

        RegionShader* region_ = regions.getElement( it->first );
        if( region_ == nullptr ) continue;

        region_->update();
    }
    emit updateCanvas();
}


void Scene3d::addMainCrossSection( CrossSection* const& raw_ )
{
    context->makeCurrent( surface );

    main_csection = new PlaneShader( raw_ );
    emit updateCanvas();
}



void Scene3d::addCrossSection( CrossSection* const& raw_ )
{
    context->makeCurrent( surface );

    csections.addElement( raw_->getDepth(), new PlaneShader( raw_ ) );

    emit updateCanvas();
}


void Scene3d::updateCrossSection( CrossSection* const& raw_ )
{
    context->makeCurrent( surface );

    if( csections.findElement( raw_->getDepth() ) == false  ) return;
    PlaneShader* csection_ = csections.getElement( raw_->getDepth() );
    csection_->update();

    emit updateCanvas();
}


void Scene3d::updateCrossSections()
{
    context->makeCurrent( surface );

    for ( CrossSectionsContainer::Iterator it =  csections.begin(); it != csections.end(); ++it )
    {
        PlaneShader* csection_ = csections.getElement( it->first );
        if( csection_ == nullptr ) continue;

        csection_->update();
    }

    emit updateCanvas();

}


void Scene3d::updateMainCrossSection()
{
    context->makeCurrent( surface );

    if( main_csection == nullptr ) return;
    main_csection->update();

    emit updateCanvas();
}


void Scene3d::removeCrossSection( CrossSection* const& raw_ )
{
    context->makeCurrent( surface );

    if( csections.findElement( raw_->getDepth() ) == false  ) return;
    csections.removeElement( raw_->getDepth() );
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

void Scene3d::updateObjects()
{
    for ( ObjectsContainer::Iterator it =  objects.begin(); it != objects.end(); ++it )
    {
        SurfaceShader* obj_ = objects.getElement( it->first );
        if( obj_ == nullptr ) continue;
        obj_->enableDrawingEdges();
        obj_->update();
    }

}

void Scene3d::removeObject( const std::size_t &index_ )
{

    if( objects.findElement( index_ ) == false ) return;

    SurfaceShader* obj_ = objects.getElement( index_ );
    obj_->clear();
    delete obj_;
    obj_ = nullptr;

    objects.removeElement( index_ );

    emit updateCanvas();
}



void Scene3d::setHeightMap( double zmin_, double zmax_ )
{

    for ( ObjectsContainer::Iterator it =  objects.begin(); it != objects.end(); ++it )
    {
        SurfaceShader* obj_ = objects.getElement( it->first );
        if( obj_ == nullptr ) continue;

        obj_->setHeightMap( zmin_, zmax_);

    }

}



void Scene3d::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                               const int& h )
{

    if( volume != nullptr )
        volume->draw( V, P, w, h );

    if( main_csection != nullptr )
        main_csection->draw( V, P, w, h );


    for ( CrossSectionsContainer::Iterator it =  csections.begin(); it != csections.end(); ++it )
    {
        (it->second)->draw( V, P, w, h );
    }

    for ( ObjectsContainer::Iterator it =  objects.begin(); it != objects.end(); ++it )
    {
        (it->second)->draw( V, P, w, h );
    }

    for ( RegionsContainer::Iterator it =  regions.begin(); it != regions.end(); ++it )
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


    for ( RegionsContainer::Iterator it =  regions.begin(); it != regions.end(); ++it )
    {

        RegionShader* region_ = regions.getElement( it->first );
        if( region_ == nullptr ) continue;

        region_->clear();
        delete region_;
        region_ = nullptr;
    }
    regions.clear();


    if( volume != nullptr )
    {
        volume->clear();
        delete volume;
        volume = nullptr;
    }

    if( output_volume != nullptr )
    {
        output_volume->clear();
        delete output_volume;
        output_volume = nullptr;
    }

    if( main_csection != nullptr )
    {
        main_csection->clear();
        delete main_csection;
        main_csection = nullptr;
    }

    clearData();
    emit updateCanvas();
}


void Scene3d::clearData()
{
    volume = nullptr;
    output_volume = nullptr;
    main_csection = nullptr;
    current_color = Qt::red;
    shader_directory.clear();

}

