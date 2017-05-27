#include "scene3d_refactored.h"


#include "OpenGLWrappers/volumeopenglwrapper_refactored.h"
#include "OpenGLWrappers/crosssectionopenglwrapper_refactored.h"


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


void Scene3d_refactored::addCrossSection()
{
    if( csection != nullptr )
        delete csection;

    context->makeCurrent( surface );

    csection = new CrossSectionOpenGLWrapper_Refactored();
    csection->setShaderDirectory( shader_directory.toStdString() );
    csection->init();

    float mx, my, mz;
    float Mx, My, Mz;

    volume->getMinimum( mx, my, mz );
    volume->getMaximum( Mx, My, Mz );

    csection->setMinimum( mx, my, mz );
    csection->setMaximum( Mx, My, Mz );
    csection->setDepth( volume->getDepth() );

    emit updateCanvas();
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
