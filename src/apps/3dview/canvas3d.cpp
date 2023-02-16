/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015                                                       *
 * UofC - University of Calgary                                             *
 *                                                                          *
 * This file is part of RRM Software.                                       *
 *                                                                          *
 * RRM is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * RRM is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.             *
 ****************************************************************************/

/**
 * @file canvas3d.cpp
 * @author Clarissa C. Marques
 * @brief File containing the Canvas3d class
 */


#include <iostream>
#include <stdlib.h>

#include "canvas3d.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QFileDialog>
#include <QOffscreenSurface>
#include <QScreen>
#include <QPixmap>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFramebufferObject>


Canvas3d::Canvas3d()
{
    createScene();
    canvas_width = width();
    canvas_height = height();
}


void Canvas3d::initializeGL()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if( err != GLEW_OK )
    {
        std::cout << "glewInit failed: " << glewGetErrorString( err ) << std::endl;
        exit( EXIT_FAILURE );
    }

    makeCurrent();
    glClearColor( 1.0f , 1.0 , 1.0 , 1.0f );
    glEnable( GL_MULTISAMPLE );

    camera.setPerspectiveMatrix ( 60.0 , (float) width()/(float)height(), 0.1f , 100.0f );
    shareOpenGLContext();
}


void Canvas3d::resizeGL( int width, int height )
{

    glViewport( 0 , 0 , (float) width , (float)height );
    camera.setViewport( Eigen::Vector2f( width, (float)height ) );
    camera.setPerspectiveMatrix( camera.getFovy(), (float) width/(float)height, 0.1f , 100.0f );


}


void Canvas3d::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // calling a method to render the scene
    if( scene3d != nullptr )
        scene3d->draw( camera.getViewMatrix(), camera.getProjectionMatrix(), width(), height() );

    if( axes != nullptr )
        axes->draw( camera.getViewMatrix(), camera.getProjectionMatrix(), width(), height() );


}


void Canvas3d::createScene()
{
    scene3d = std::make_shared< Scene3d >();
    connect( scene3d.get(), SIGNAL( updateCanvas() ), this, SLOT( update() ) );
}


std::shared_ptr< Scene3d > Canvas3d::getScene() const
{
    return scene3d;
}


std::string Canvas3d::sendImage( double zmin_, double zmax_, double width_, double height_  )
{
    // this method is suppose the create an image using a top-view camera from the surfaces with height map color scheme
    // the return is a string because it was the path of the image file
    return std::string();
}



void Canvas3d::screenshot()
{
    QString selectedFilter;
    QString name_of_file = QFileDialog::getSaveFileName( nullptr, tr( "Save Image" ), "./screenshots/",
                                                         tr( "PNG (*.png);;SVG (*.svg)" ),
                                                         &selectedFilter );

    if( selectedFilter == "PNG (*.png)" )
    {
        savetoRasterImage( name_of_file );
    }
    else if ( selectedFilter == "SVG (*.svg)" )
    {
        savetoVectorImage( name_of_file );
    }
}



void Canvas3d::savetoRasterImage( const QString& filename )
{
    QStringList name_and_extension = filename.split('.', QString::SkipEmptyParts );

    QString filename_csection;

    if( name_and_extension.size() > 1 ){
        filename_csection = name_and_extension[ 0 ].append( "_model." );
        filename_csection.append( name_and_extension[1] );
    }
    else
        filename_csection = filename;

    // getting an image from the framebuffer
    QImage image = grabFramebuffer();
    image.save( filename_csection );
}

void Canvas3d::savetoVectorImage( const QString& filename )
{
    std::cout << "Not implemented yet to 3dview\n" << std::flush;
}



void Canvas3d::shareOpenGLContext()
{
    // sharing the opengl context, created by Qt, with the scene 3d
    scene3d->setOpenGLContext( context() );

    QString arg_( QDir::separator() );
    QString path_ = QDir::currentPath();

    // defining the current shaders directory
    QString shaders_path_( path_ );
    shaders_path_.append( arg_ ).append( "shaders" ).append( arg_ );
    QString models_path_( path_ );
    models_path_.append( arg_ ).append( "objects" ).append( arg_ );


    QDir dir_;
    bool status_ = dir_.cd( shaders_path_ );
    if( status_ == true )
        std::cout << "Shader Path exists " << std::endl << std::flush;

    else
    {
        std::cout << "Shader Path not exists " << std::endl << std::flush;
        return;
    }

    dir_.cdUp();

    // defining the directory where the axes model is placed
    status_ = dir_.cd( models_path_ );
    if( status_ == true )
        std::cout << "Objects Path exists " << std::endl << std::flush;

    else
    {
        std::cout << "Objects Path doesn't exist " << std::endl << std::flush;
        return;
    }


    // initializing the axes
    axes = new CoordinateAxes3d();
    axes->initShader( shaders_path_.toStdString() );
    axes->load( models_path_.toStdString() );

    // the axes can be placed on the center or on the left-bottom corner of the canvas
    axes->setNonCentered();

}




void Canvas3d::mousePressEvent( QMouseEvent *event )
{
    makeCurrent();

    Eigen::Vector2f screen_pos( event->x(), event->y() );

    if ( event->modifiers() & Qt::ShiftModifier )
    {
        if ( event->button() == Qt::LeftButton )
        {
            camera.translateCamera( screen_pos );
        }
    }
    else
    {
        if ( event->button() == Qt::LeftButton )
        {
            camera.rotateCamera( screen_pos );
        }
    }

    QOpenGLWidget::mousePressEvent( event );
    update();

}


void Canvas3d::mouseMoveEvent( QMouseEvent *event )
{

    makeCurrent();

    Eigen::Vector2f screen_pos( event->x() , event->y() );


    if ( ( event->modifiers() & Qt::ShiftModifier )  )
    {
        if ( event->buttons() & Qt::LeftButton )
        {
            camera.translateCamera( screen_pos );
        }

    }
    else
    {
        if ( event->buttons() & Qt::LeftButton )
        {
            camera.rotateCamera( screen_pos );
        }

    }

    QOpenGLWidget::mouseMoveEvent( event );
    update();
}


void Canvas3d::mouseReleaseEvent( QMouseEvent *event )
{

    makeCurrent();

    if ( event->button ( ) == Qt::LeftButton )
    {
        camera.endTranslation();
        camera.endRotation();
    }

    QOpenGLWidget::mouseReleaseEvent( event );
    update();

}


void Canvas3d::wheelEvent( QWheelEvent *event )
{

    float pos = event->delta()/float( WHEEL_STEP );


    if( event->modifiers() & Qt::ControlModifier )
    {
        if ( pos > 0 )
        {
            emit decreaseSlider();
        }

        else if ( pos < 0 )
        {
            emit increaseSlider();
        }
    }
    else
    {
        if ( pos > 0 )
        {
            camera.increaseZoom ( 1.05f );
        }

        else if( pos < 0 )
        {
            camera.increaseZoom ( 1.0f/1.05f );
        }
    }

    QOpenGLWidget::wheelEvent( event );
    update();
}
