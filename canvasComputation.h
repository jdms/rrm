#ifndef CANVASCOMPUTATION_H
#define CANVASCOMPUTATION_H

#include <QtOpenGL/QGLWidget>
#include <QOpenGLFunctions>

class CanvasComputation: public QGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT


    public:

        CanvasComputation( QGLFormat format, QWidget *parent = 0 );

    protected:

        void initializeGL();
        void resizeGL( int width, int height );
        void paintGL();


};

#endif // CANVASCOMPUTATION_H
