#ifndef CANVAS3D_REFACTORED_H
#define CANVAS3D_REFACTORED_H


#include "Scene3D.h"
#include "scene3d_refactored.h"

#include <QOpenGLWidget>



class Canvas3d_Refactored: public QOpenGLWidget
{
    Q_OBJECT

    public:

        Canvas3d_Refactored();

        Scene3d_refactored* getScene() const;


    protected:

        void initializeGL();
        void resizeGL( int width, int height );
        void paintGL();


        void createScene();
        void shareOpenGLContext();


    private:


        Scene3d_refactored* scene3d;

};

#endif // CANVAS3D_REFACTORED_H
