
#include "scene3d_refactored.h"


#include <QString>
#include <QOpenGLContext>
#include <QSurface>


Scene3d_refactored::Scene3d_refactored()
{

}


void Scene3d_refactored::setCurrentColor( const QColor& color )
{
    current_color = color;
}


void Scene3d_refactored::setCurrentDirectory( const QString& dir )
{
    shader_directory = dir;
}


void Scene3d_refactored::setOpenGLContext( QOpenGLContext* const& ctxt )
{
    context = ctxt;
    surface = ctxt->surface();
}
