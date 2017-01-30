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


#ifndef VIEW3DCANVAS_H
#define VIEW3DCANVAS_H

#include <GL/glew.h>

#include<Tucano/Trackball.hpp>
#include<Tucano/Shader.hpp>
#include<Tucano/BoundingBox3.hpp>

#include <iostream>

#include <QtWidgets/QOpenGLWidget>
#include <QOpenGLContext>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QDir>
#include <QCoreApplication>



#include "MainWindow/Scene.h"
#include "GradientBackgroundShader.hpp"


/**!
 * @brief OpenGL window for 3D data visualization. Currently implements the extrusion.
 */

class View3DCanvas: public QOpenGLWidget
{
        Q_OBJECT


    public:


        explicit View3DCanvas ( QWidget* parent = 0 );
        ~View3DCanvas();


        inline void setScene( Scene* sc ){

            scene = sc;
			scene->setCurrentDirectory(shader_directory.toStdString());

            connect( scene, SIGNAL( updatedScene() ), this, SLOT( update() ) );
            connect( scene, &Scene::initContext, this, &View3DCanvas::initContextScene );
            connect( scene, &Scene::updateContext, this, &View3DCanvas::updateContextScene );

        }


    public slots:


        void clear();
        void initContextScene();
        void updateContextScene();



    signals:

        void openglInitialized();


    protected:


        virtual void initializeGL();
        void resizeGL( int width , int height );
        void paintGL();


        void reloadShaders();
        void resetBuffers();


        void mousePressEvent ( QMouseEvent *event );
        void mouseMoveEvent ( QMouseEvent *event );
        void mouseReleaseEvent ( QMouseEvent *event );
        void wheelEvent ( QWheelEvent *e );

        void keyPressEvent ( QKeyEvent * event );

        void setCurrentDirectory();




    private:


        Scene* scene;

        Tucano::Trackball camera;
        GradientBackgroundShader* background;

        QString shader_directory;

        std::vector <Surface*> surfaces;

};


#endif

