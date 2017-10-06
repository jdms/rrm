#ifndef SCENE3D_H
#define SCENE3D_H

#include <QObject>
#include <QColor>

#include "Eigen/Dense"

class QString;
class QOpenGLContext;
class QSurface;

class VolumeShader;
class PlaneShader;


#include "./core/base/models/scene.h"
#include "./core/base/models/container.h"



class Scene3d: public QObject, public Scene
{
    Q_OBJECT


    using CrossSectionsContainer = Container< std::size_t, PlaneShader* >;




    public:


        Scene3d();


        virtual void addVolume( Volume* const& raw_ );
        virtual void clearVolume();


        virtual void addCrossSection( CrossSection* const& raw_ );
        virtual void removeCrossSection( CrossSection* const& raw_ );



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

        VolumeShader* volume;
        CrossSectionsContainer csections;


};

#endif // SCENE3D_H
