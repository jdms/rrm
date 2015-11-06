#ifndef CANVAS3D_H
#define CANVAS3D_H

#include <QtOpenGL/QGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_0>

class Canvas3D: public QGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT


    public:

        Canvas3D( QGLFormat format, QWidget *parent = 0 );

    protected:

        void initializeGL();
        void resizeGL( int width, int height );
        void paintGL();

};

#endif // CANVAS3D_H
