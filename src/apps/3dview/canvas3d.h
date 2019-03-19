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
 * @file canvas3d.h
 * @author Clarissa C. Marques
 * @brief File containing the Canvas3d class
 */


#ifndef CANVAS3D_H
#define CANVAS3D_H



#include <Tucano/Camera.hpp>
#include <Tucano/Trackball.hpp>
#include "scene3d.h"

#include <QOpenGLWidget>


#include "coordinate_axes_3d.h"

class QMouseEvent;
class QWheelEvent;

/**
 *   The holder to the 3d scene.
 *   An OpenGL canvas to show the scene. It inherits from QOpenGLWidget.
 */
class Canvas3d: public QOpenGLWidget
{
    Q_OBJECT

    public:


        /**
       * Constructor.
       */
        Canvas3d();


       /**
       * Method to retrieves the scene.
       * @see Scene3d
       * @return std::shared_ptr< Scene3d > A shared pointer to the scene associated to this canvas.
       */
        std::shared_ptr< Scene3d > getScene() const;


        /**
       * Method to return the path of the image taken as height map.
       * @param zmin the the minimum value of the z coordinare to compute the height map colors
       * @param zmax the the maximum value of the z coordinare to compute the height map colors
       * @param width the width dimension of the image
       * @param height the height dimension of the image
       * @return std::string The path pf the created image
       */
        std::string sendImage( double zmin_, double zmax_, double width_, double height_  );


    public slots:


        /**
        * Method(slot) to take a screenshot of the scene.
        * @return void.
        */
        void screenshot();


    signals:


        /**
        * Signal(slot) sent to indicate the cross-section slide should be increased.
        * @return void.
        */
        void increaseSlider();


        /**
        * Signal(slot) sent to indicate the cross-section slide should be decreased.
        * @return void.
        */
        void decreaseSlider();


    protected:


        /**
        * Virtual method(slot) to initialize the OpenGL
        * @return Void.
        */
        void initializeGL();


        /**
        * Method (slot) to resize and update the camera and the viewport.
        * This virtual method(slot) is automatically called when the OpenGL canvas is resized.
        * @param width the width of the canvas
        * @param height the height of the canvas
        * @return void.
        */
        void resizeGL( int width, int height );


        /**
        * Method (slot) to render the OpenGL scene.
        * @return void.
        */
        void paintGL();


        /**
        * Method (slot) to create the 3d scene.
        * @see Scene3d
        * @return void.
        */
        void createScene();


        /**
        * Method (slot) to share the OpenGL context with the scene.
        * @return void.
        */
        void shareOpenGLContext();


        /**
        * Method(slot) to save the screenshot as a raster image.
        * @param filename name of the image file to be saved
        * @return void.
        */
        void savetoRasterImage( const QString& filename );


        /**
        * Method(slot) to save the screenshot as a vector image.
        * @param filename name of the image file to be saved
        * @return void.
        */
        void savetoVectorImage( const QString& filename );


        /**
        * Virtual method to handle the events when a mouse button is pressed.
        * @param event captures the mouse event to identify which operation was performed and if and which button is pressed
        * @see QMouseEvent
        * @return void.
        */
        void mousePressEvent( QMouseEvent *event );


        /**
        * Virtual method to handle the events when the mouse is moved.
        * @param event captures the mouse event to identify which operation was performed and if and which button is pressed
        * @see QMouseEvent
        * @return void.
        */
        void mouseMoveEvent( QMouseEvent *event );


        /**
        * Virtual method to handle the events when a mouse button is released.
        * @param event captures the mouse event to identify which operation was performed and if and which button is pressed
        * @see QMouseEvent
        * @return void.
        */
        void mouseReleaseEvent( QMouseEvent *event );


        /**
        * Virtual method to handle the wheel movement of the mouse.
        * It is tipically used when a zoom is performed.
        * @param event captures the mouse event to identify which operation was performed and how much and which direction the
        * wheel was moved
        * @see QWheelEvent
        * @return void.
        */
        void wheelEvent( QWheelEvent *event );


    private:


        std::shared_ptr< Scene3d > scene3d;                                         /**< Shared pointer to the 3d scene representation. */

        Tucano::Trackball camera;                                                   /**< OpenGL camera  */

        const int WHEEL_STEP = 120;                                                 /**< Constant used to compute the zoom */

        int canvas_width;                                                           /**< Width of the canvas */
        int canvas_height;                                                          /**< Height of the canvas */

        CoordinateAxes3d* axes = nullptr;                                           /**< Wrapper instance used to represent the axes in the 3d scene.  */


};

#endif // CANVAS3D_H
