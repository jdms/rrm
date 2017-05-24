#ifndef CANVAS3D_REFACTORED_H
#define CANVAS3D_REFACTORED_H


#include "Scene3D.h"
#include <QOpenGLWidget>


class Canvas3d_Refactored: public QOpenGLWidget
{
    Q_OBJECT

    public:

        Canvas3d_Refactored();

    protected:

        void initializeGL();
        void resizeGL( int width, int height );
        void paintGL();

};

#endif // CANVAS3D_REFACTORED_H
