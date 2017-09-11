#include "scene3d.h"


#include "./core/geology/models/object.h"
#include "opengl_wrappers/volume_opengl_wrapper.h"
#include "opengl_wrappers/crosssection_opengl_wrapper.h"
#include "opengl_wrappers/object_opengl_wrapper.h"


#include <QString>
#include <QOpenGLContext>
#include <QSurface>


Scene3d::Scene3d()
{
    volume = nullptr;
    csection = nullptr;
}



void Scene3d::addVolume( Volume* const& vol )
{
    if( volume != nullptr )
        delete volume;

    context->makeCurrent( surface );

    volume = new VolumeOpenGLWrapper();
    volume->setShaderDirectory( shader_directory.toStdString() );
    volume->init();
    volume->setRawVolume( vol );

    addCrossSection();

    emit updateCanvas();
}


void Scene3d::updateVolume()
{
    if( volume == nullptr ) return;
    volume->updateGeometry();

    updateCrossSection();

    emit updateCanvas();
}


void Scene3d::addCrossSection()
{
    if( csection != nullptr )
        delete csection;

    context->makeCurrent( surface );

    csection = new CrossSectionOpenGLWrapper();
    csection->setShaderDirectory( shader_directory.toStdString() );
    csection->init();

    double mx, my, mz;
    volume->getOrigin( mx, my, mz );
    csection->setMinimum( static_cast<float>(mx), static_cast<float>(my), static_cast<float>(mz) );

    double Mx = volume->getWidth(), My = volume->getHeight(), Mz = volume->getDepth();
    csection->setMaximum( static_cast<float>(Mx), static_cast<float>(My), static_cast<float>(Mz) );

    csection->updateGeometry();
    emit updateCanvas();
}


void Scene3d::updateCrossSection()
{
    if( csection == nullptr ) return;

    context->makeCurrent( surface );

    double mx, my, mz;
    volume->getOrigin( mx, my, mz );
    csection->setMinimum( static_cast<float>(mx), static_cast<float>(my), static_cast<float>(mz) );

    double Mx = volume->getWidth(), My = volume->getHeight(), Mz = volume->getDepth();
    csection->setMaximum( static_cast<float>(Mx), static_cast<float>(My), static_cast<float>(Mz) );

    csection->updateGeometry();
    emit updateCanvas();
}


void Scene3d::moveCrossSection( double depth )
{
    if( csection == nullptr ) return;

    context->makeCurrent( surface );
    csection->setDepth( depth );
    emit updateCanvas();
}



bool Scene3d::addObject( Object* const& obj )
{

    if( isAddedObject( obj->getId() ) == true ) return false;

    context->makeCurrent( surface );

    ObjectOpenGLWrapper* object = new ObjectOpenGLWrapper();
    object->setShaderDirectory( shader_directory.toStdString() );
    object->init();


    double mx, my, mz;
    volume->getOrigin( mx, my, mz );
    object->setMinimum( static_cast< float >(mx), static_cast< float >(my),
                        static_cast< float >(mz) );

    double Mx = volume->getWidth(), My = volume->getHeight(), Mz = volume->getDepth();
    object->setMaximum( static_cast< float >(Mx), static_cast< float >(My),
                        static_cast< float >(Mz) );


    object->setRawObject( obj );
    objects[ obj->getId() ] = object;

    return true;
}


void Scene3d::updateObject( std::size_t id )
{
    if( isAddedObject( id ) == false ) return;

    ObjectOpenGLWrapper* const& object = objects[ id ];
    object->reloadBuffers();
    emit updateCanvas();

}



bool Scene3d::isAddedObject( std::size_t id )
{
    auto search = objects.find( id );
    if( search == objects.end() ) return false;
    return true;
}




void Scene3d::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                               const int& h )
{

//    if( show_axis == true )
//    {
        axes.draw( V, P, w, h );
//    }

    if( volume != nullptr )
        volume->draw( V, P, w, h );


    for( auto& it : objects )
        (it.second)->draw( V, P, w, h );


    if( csection != nullptr )
        csection->draw( V, P, w, h );
}


void Scene3d::setCurrentColor( const QColor& color )
{
    current_color = color;
}


void Scene3d::setCurrentDirectory( const QString& dir )
{
    shader_directory = dir;
}


void Scene3d::setOpenGLContext( QOpenGLContext* ctxt )
{
    context = ctxt;
    surface = ctxt->surface();

    axes.initShader( shader_directory.toStdString() );
    axes.load();
    axes.setNonCentered();
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

    if( volume != nullptr )
    {
        volume->clear();
        delete volume;
    }
    volume = nullptr;


    if( csection != nullptr )
    {
        csection->clear();
        delete csection;
    }
    csection = nullptr;

    for( auto& it: objects )
    {
        ( it.second )->clear();
        delete ( it.second );
    }
    objects.clear();
}

