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
                                                                     /*axes.initShader( current_directory );
                                                                     axes.load();*/ }


        inline void savetoRasterImage( const QString& filename )
        {
            QStringList name_and_extension = filename.split('.', QString::SkipEmptyParts );

            QString filename_csection;

            if( name_and_extension.size() > 1 ){
                filename_csection = name_and_extension[ 0 ].append( "_model." );
                filename_csection.append( name_and_extension[1] );
            }
            else
                filename_csection = filename;


            QImage image = grabFramebuffer();
            image.save( filename_csection );
        }

        inline void savetoVectorImage( const QString& filename )
        {
            std::cout << "Not implemented yet to 3dView\n" << std::flush;
        }

        void wheelEvent( QWheelEvent *e );


    signals:
        void increaseSlider();
        void decreaseSlider();


    protected:

        void initializeGL();
        void resizeGL( int width , int height );
        void paintGL();

        void mousePressEvent( QMouseEvent *event );
        void mouseMoveEvent( QMouseEvent *event );
        void mouseReleaseEvent( QMouseEvent *event );


        void keyPressEvent( QKeyEvent * event );


    protected:

        Tucano::Trackball camera;
        Scene3D *scene;


        std::string current_directory;

};

#endif // CANVAS3D_H
