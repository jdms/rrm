#ifndef CANVAS3D_REFACTORED_H
#define CANVAS3D_REFACTORED_H


#include "Scene3D.h"
#include "scene3d_refactored.h"

#include <QOpenGLWidget>

class QMouseEvent;
class QWheelEvent;


class Canvas3d_Refactored: public QOpenGLWidget
{
    Q_OBJECT

    public:

        Canvas3d_Refactored();

        Scene3d_refactored* getScene() const;




    public slots:

        void screenshot();


    signals:

        void increaseSlider();
        void decreaseSlider();


    protected:

        void initializeGL();
        void resizeGL( int width, int height );
        void paintGL();

        void createScene();
        void shareOpenGLContext();

        void savetoRasterImage( const QString& filename );
        void savetoVectorImage( const QString& filename );


        void mousePressEvent( QMouseEvent *event );
        void mouseMoveEvent( QMouseEvent *event );
        void mouseReleaseEvent( QMouseEvent *event );
        void wheelEvent( QWheelEvent *event );


    private:


        Scene3d_refactored* scene3d;

        Tucano::Trackball camera;
        const int WHEEL_STEP = 120;

};

#endif // CANVAS3D_REFACTORED_H
