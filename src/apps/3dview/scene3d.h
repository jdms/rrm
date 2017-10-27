#ifndef SCENE3D_H
#define SCENE3D_H

#include <QObject>
#include <QColor>

#include "Eigen/Dense"

class QString;
class QOpenGLContext;
class QSurface;

//class VolumeShader;
//class PlaneShader;
//class SurfaceShader;


#include "./core/base/models/scene.h"
#include "./core/base/models/container.h"



class Scene3d: public QObject, public Scene
{
    Q_OBJECT


//    using CrossSectionsContainer = Container< std::size_t, PlaneShader* >;
//    using ObjectsContainer = Container< std::size_t, SurfaceShader* >;


    public:


        Scene3d();


        virtual void addVolume( Volume* const& raw_ );
        virtual void updateVolume();
        virtual void clearVolume();


        virtual void addCrossSection( CrossSection* const& raw_ );
        virtual void removeCrossSection( CrossSection* const& raw_ );


        virtual void addObject(  Object* const& raw_ );
        virtual void updateObject(  const std::size_t& index_ );


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
        QOpenGLContext* context;
        QSurface* surface;

//        VolumeShader* volume;
//        CrossSectionsContainer csections;
//        ObjectsContainer objects;


};

#endif // SCENE3D_H
