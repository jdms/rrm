#include "scene3d_refactored.h"


#include "./Core/Geology/Models/object_refactored.h"
#include "OpenGLWrappers/volumeopenglwrapper_refactored.h"
#include "OpenGLWrappers/crosssectionopenglwrapper_refactored.h"
#include "OpenGLWrappers/objectopenglwrapper_refactored.h"


#include <QString>
#include <QOpenGLContext>
#include <QSurface>


Scene3d_refactored::Scene3d_refactored()
{
    volume = nullptr;
    csection = nullptr;
}



void Scene3d_refactored::addVolume( Volume* const& vol )
{
    if( volume != nullptr )
        delete volume;

    context->makeCurrent( surface );

    volume = new VolumeOpenGLWrapper_Refactored();
    volume->setShaderDirectory( shader_directory.toStdString() );
    volume->init();
    volume->setRawVolume( vol );

    addCrossSection();

    emit updateCanvas();
}


void Scene3d_refactored::updateVolume()
{
    if( volume == nullptr ) return;
    volume->updateGeometry();

     emit updateCanvas();
}


void Scene3d_refactored::addCrossSection()
{
    if( csection != nullptr )
        delete csection;

    context->makeCurrent( surface );

    csection = new CrossSectionOpenGLWrapper_Refactored();
    csection->setShaderDirectory( shader_directory.toStdString() );
    csection->init();

    double mx, my, mz;
    volume->getOrigin( mx, my, mz );
    csection->setMinimum( static_cast<float>(mx), static_cast<float>(my), static_cast<float>(mz) );

    double Mx = volume->getWidth(), My = volume->getHeight(), Mz = volume->getDepth();
    csection->setMaximum( static_cast<float>(Mx), static_cast<float>(My), static_cast<float>(Mz) );

    emit updateCanvas();
}


void Scene3d_refactored::updateCrossSection()
{
    if( csection != nullptr )
        delete csection;

    context->makeCurrent( surface );

    double mx, my, mz;
    volume->getOrigin( mx, my, mz );
    csection->setMinimum( static_cast<float>(mx), static_cast<float>(my), static_cast<float>(mz) );

    double Mx = volume->getWidth(), My = volume->getHeight(), Mz = volume->getDepth();
    csection->setMaximum( static_cast<float>(Mx), static_cast<float>(My), static_cast<float>(Mz) );

    emit updateCanvas();
}


void Scene3d_refactored::moveCrossSection( double depth )
{
    if( csection == nullptr ) return;

    context->makeCurrent( surface );
    csection->setDepth( depth );
    emit updateCanvas();
}



bool Scene3d_refactored::addObject( Object_Refactored* const& obj )
{

    if( isAddedObject( obj->getId() ) == true ) return false;

    context->makeCurrent( surface );

    ObjectOpenGLWrapper_Refactored* object = new ObjectOpenGLWrapper_Refactored();
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


void Scene3d_refactored::updateObject( std::size_t id )
{
    if( isAddedObject( id ) == false ) return;

    ObjectOpenGLWrapper_Refactored* const& object = objects[ id ];
    object->reloadBuffers();
    emit updateCanvas();

}



bool Scene3d_refactored::isAddedObject( std::size_t id )
{
    auto search = objects.find( id );
    if( search == objects.end() ) return false;
    return true;
}




void Scene3d_refactored::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                               const int& h )
{

    if( volume != nullptr )
        volume->draw( V, P, w, h );


    for( auto& it : objects )
        (it.second)->draw( V, P, w, h );


    if( csection != nullptr )
        csection->draw( V, P, w, h );
}


void Scene3d_refactored::setCurrentColor( const QColor& color )
{
    current_color = color;
}


void Scene3d_refactored::setCurrentDirectory( const QString& dir )
{
    shader_directory = dir;
}


void Scene3d_refactored::setOpenGLContext( QOpenGLContext* ctxt )
{
    context = ctxt;
    surface = ctxt->surface();
}



void Scene3d_refactored::clear()
{
    clearData();
    current_color = Qt::red;
    emit updateCanvas();
}


void Scene3d_refactored::clearData()
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

