/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



#ifndef CANVAS3D_H
#define CANVAS3D_H



#include <Tucano/Camera.hpp>
#include <Tucano/Trackball.hpp>
#include "scene3d.h"

#include <QOpenGLWidget>

class QMouseEvent;
class QWheelEvent;


class Canvas3d: public QOpenGLWidget
{
    Q_OBJECT

    public:

        Canvas3d();

        Scene3d* getScene() const;
        std::string sendImage( double zmin_, double zmax_, double width_, double height_  );




    public slots:

        void screenshot();


    signals:

        void increaseSlider();
        void decreaseSlider();


    protected:

        void initializeGL();
        void resizeGL( int width, int height );
        void paintGL();

        void createScene();
        void shareOpenGLContext();

        void savetoRasterImage( const QString& filename );
        void savetoVectorImage( const QString& filename );


        void mousePressEvent( QMouseEvent *event );
        void mouseMoveEvent( QMouseEvent *event );
        void mouseReleaseEvent( QMouseEvent *event );
        void wheelEvent( QWheelEvent *event );


    private:


        Scene3d* scene3d;

        Tucano::Trackball camera;
        const int WHEEL_STEP = 120;

        int canvas_width;
        int canvas_height;

};

#endif // CANVAS3D_H
