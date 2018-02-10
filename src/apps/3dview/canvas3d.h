#ifndef CANVAS3D_H
#define CANVAS3D_H



#include <Tucano/Camera.hpp>
#include <Tucano/Trackball.hpp>
#include "scene3d.h"

#include <QOpenGLWidget>

class QMouseEvent;
class QWheelEvent;


class Canvas3d: public QOpenGLWidget
{
    Q_OBJECT

    public:

        Canvas3d();

        Scene3d* getScene() const;
        std::string sendImage( double zmin_, double zmax_, double width_, double height_  );




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


        Scene3d* scene3d;

        Tucano::Trackball camera;
        const int WHEEL_STEP = 120;

        int canvas_width;
        int canvas_height;

};

#endif // CANVAS3D_H
