#ifndef SCENE3D_H
#define SCENE3D_H

#include <QObject>
#include <QColor>

#include "Eigen/Dense"
#include "OpenGLWrappers/coordinate_axes_3d.h"

class QString;
class QOpenGLContext;
class QSurface;

class Volume;
class VolumeOpenGLWrapper;
class CrossSectionOpenGLWrapper;
class Object;
class ObjectOpenGLWrapper;


class Scene3d: public QObject
{
    Q_OBJECT

    public:

        Scene3d();

        void addVolume( Volume* const& vol );

        void addCrossSection();
        void moveCrossSection( double depth );

        bool addObject( Object* const& obj );
        bool isAddedObject( std::size_t id );

        inline void setActiveObjects(){}

        inline void setObjectSelected(){}
        inline void setObjectsSelected(){}

        inline void setObjectSelectable(){}
        inline void setObjectsSelectable(){}

        void setCurrentColor( const QColor& color );

        void updateVolume();
        void updateCrossSection();
        void updateObject( std::size_t );
        inline void updateObjects(){}
        inline void updateScene(){}

        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w, const int& h );

        void clear();
        void clearData();

        void setCurrentDirectory( const QString& dir );
        void setOpenGLContext( QOpenGLContext* ctxt );


    signals:

        void updateCanvas();


    private:


        QString shader_directory;
        QColor current_color;

        CoordinateAxes3d axes;

        QOpenGLContext* context;
        QSurface* surface;

        VolumeOpenGLWrapper* volume;
        CrossSectionOpenGLWrapper* csection;

        std::map< std::size_t, ObjectOpenGLWrapper* > objects;

};

#endif // SCENE3D_H
