#ifndef CANVAS3D_H
#define CANVAS3D_H


#include "Scene3D.h"

#include <QtWidgets/QOpenGLWidget>
#include <QOpenGLContext>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>

#include<Tucano/Trackball.hpp>


class Canvas3D: public QOpenGLWidget
{
        Q_OBJECT

    public:

        Canvas3D();

        inline void setScene( Scene3D* sc_ ){ scene = sc_;  connect( scene, SIGNAL( updateCanvas() ),
                                                                     this, SLOT( update() ) ); }
        inline void shareContext()
        {
            scene->setContext( context() );
        }


    protected:

        void initializeGL();
        void resizeGL( int width , int height );
        void paintGL();


        void reloadShaders();
        void resetBuffers();


        void mousePressEvent( QMouseEvent *event );
        void mouseMoveEvent( QMouseEvent *event );
        void mouseReleaseEvent( QMouseEvent *event );
        void wheelEvent( QWheelEvent *e );

        void keyPressEvent( QKeyEvent * event );


    protected:

        Tucano::Trackball camera;

        Scene3D *scene;


};

#endif // CANVAS3D_H
