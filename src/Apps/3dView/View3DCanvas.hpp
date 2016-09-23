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



#include "Mainwindow/Scene.h"
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
