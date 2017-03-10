#ifndef CANVAS3D_H
#define CANVAS3D_H

#include <GL/glew.h>

#include <QtWidgets/QOpenGLWidget>
#include <QOpenGLContext>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>


#include "Scene3D.h"


class Canvas3D: public QOpenGLWidget
{
        Q_OBJECT

    public:

        Canvas3D();

        inline void setScene( Scene3D* sc_ ){ scene = sc_; }

    protected:

        void initializeGL();
        void resizeGL( int width , int height );
        void paintGL();

    protected:

        Scene3D *scene;

};

#endif // CANVAS3D_H
