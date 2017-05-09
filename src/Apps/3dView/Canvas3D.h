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

        inline void setCurrentDirectory( const std::string& path_ ){ current_directory = path_;
                                                                     axes.initShader( current_directory );
                                                                     axes.load(); }

        inline void showAxis( bool status_ ){ show_axis = status_; update(); }



    protected:

        void initializeGL();
        void resizeGL( int width , int height );
        void paintGL();

        void mousePressEvent( QMouseEvent *event );
        void mouseMoveEvent( QMouseEvent *event );
        void mouseReleaseEvent( QMouseEvent *event );
        void wheelEvent( QWheelEvent *e );

        void keyPressEvent( QKeyEvent * event );


    protected:

        Tucano::Trackball camera;
        Scene3D *scene;

        bool show_axis = true;
        CoordinateAxes axes;

        std::string current_directory;

};

#endif // CANVAS3D_H
