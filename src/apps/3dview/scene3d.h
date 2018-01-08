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
class SurfaceShader;
class RegionShader;


#include "./core/models/scene.h"
#include "./core/models/container.h"



class Scene3d: public QObject, public Scene
{
    Q_OBJECT


    using CrossSectionsContainer = Container< double, PlaneShader* >;
    using ObjectsContainer = Container< std::size_t, SurfaceShader* >;
    using RegionsContainer = Container< std::size_t, RegionShader* >;



    public:


        Scene3d();


        virtual void addVolume( Volume* const& raw_ );
        virtual void updateVolume();
        virtual void clearVolume();



        void addOutputVolume( Volume* const& raw_ );
        void updateOutputVolume();
        void clearOutputVolume();


        void addRegion( Regions* const& raw_ );
        void updateRegion( std::size_t index_ );
        void updateRegions();
//        void clearRegion();

        void addMainCrossSection( CrossSection* const& raw_ );
        void updateMainCrossSection();

        virtual void addCrossSection( CrossSection* const& raw_ );
        void updateCrossSection( CrossSection* const& raw_ );
        void updateCrossSections();
        virtual void removeCrossSection( CrossSection* const& raw_ );


        virtual void addObject(  Object* const& raw_ );
        virtual void updateObject(  const std::size_t& index_ );
        void updateObjects();
        void removeObject(  const std::size_t& index_ );


        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w, const int& h );


        void clear();
        void clearData();

        void setCurrentDirectory( const QString& dir );
        void setOpenGLContext( QOpenGLContext* ctxt );

        virtual void setCurrentColor( int r, int g, int b ){}
        virtual void getCurrentColor( int& r, int& g, int& b ){}



    signals:

        void updateCanvas();


    private:


        QString shader_directory;
        QColor current_color;
        QOpenGLContext* context;
        QSurface* surface;

        VolumeShader* volume;
        VolumeShader* output_volume;

        PlaneShader* main_csection;

        CrossSectionsContainer csections;
        ObjectsContainer objects;
        RegionsContainer regions;


};

#endif // SCENE3D_H
