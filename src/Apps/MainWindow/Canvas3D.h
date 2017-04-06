#ifndef CANVAS3D_H
#define CANVAS3D_H


#include "Scene3D.h"

#include <QtWidgets/QOpenGLWidget>
#include <QOpenGLContext>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>




class Canvas3D: public QOpenGLWidget
{
        Q_OBJECT

    public:

        Canvas3D();

        inline void setScene( Scene3D* sc_ ){ scene = sc_;  scene->setContext( this->context() ); }

    protected:

        void initializeGL();
        void resizeGL( int width , int height );
        void paintGL();

    protected:

        Scene3D *scene;

};

#endif // CANVAS3D_H
