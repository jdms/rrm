#ifndef SCENE3D_H
#define SCENE3D_H


#include <vector>
#include <string>



#include <GL/glew.h>

#include "Core/Geology/Models/Volume.h"
#include "Core/Geology/Models/Object.h"
#include "Core/Geology/Models/Region1.h"
#include "Core/Geology/Models/CrossSection.h"
#include "OpenGLWrappers/VolumeOpenGLWrap.h"
#include "OpenGLWrappers/ObjectOpenGLWrapper.h"
#include "OpenGLWrappers/CrossSectionOpenGLWrapper.h"

#include "OpenGLWrappers/CoordinateAxes.h"

#include <QObject>
#include <QOpenGLContext>
#include <QColor>


class Scene3D: public QObject
{

    Q_OBJECT

    public:

        Scene3D();

        inline void setCurrentDirectory( const std::string& path_ ){ current_directory = path_;

                                                                      }

        void addVolume(  Volume* const& volume_ );

        void createCrossSection();

        bool addObject( Object* const& object_ );

        inline bool addRegion( const Region1* region_ ){ return true; }

        inline void setContext( QOpenGLContext* cnt_ )
        {
            context = cnt_;
            surfacegl = cnt_->surface();

            axes.initShader( current_directory );
            axes.load();
            axes.setNonCentered();

        }


        void draw( const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_,
                   const int& h_ );

        inline void moveCrossSection( double depth_ ){ input_volume->updateCrossSection( depth_ ); emit updateCanvas();
                                                       /*cross_section->reloadBuffers( depth_ );*/ }
        inline void updateCrossSection()
        {
//            if( input_volume == nullptr ) return;
//            if( cross_section == nullptr ) return;

//            double ox_ = 0.0f, oy_ = 0.0f, oz_ = 0.0f;
//            double w_ = 0.0f, h_ = 0.0f, d_ = 0.0f;

//            input_volume->getOrigin( ox_, oy_, oz_ );
//            input_volume->getDimensions( w_, h_, d_ );

//            cross_section->setBoundingBox( ( float ) ox_, ( float ) (ox_ + w_), ( float ) oy_, ( float ) (oy_ + h_),
//                                          ( float ) oz_, ( float ) (oz_ + d_) );
        }

        inline void showAxis( bool status_ ){ show_axis = status_; }

        inline void updateVolumeDimensions(){ input_volume->updateDimensions(); }

    signals:

        void updateCanvas();


    public slots:

        void updateScene();
        void clearScene();

        inline void setCurrentColor( const QColor& c_ ){ current_color = c_; }
        inline void setCurrentColor( int red_, int green_, int blue_ ){
                                     current_color = QColor( red_, green_, blue_ ); }

        inline void centerAxes( bool status_ )
        {
            if( status_ == false )
            {
                axes.setNonCentered();
                updateCanvas();
                return;
             }

            Eigen::Vector3f min = input_volume->getMinimum();
            axes.setCenter( min.x(), min.y(), min.z() );
            updateCanvas();
        }

    protected:

        std::string current_directory;

        QOpenGLContext* context;
        QSurface* surfacegl;

        VolumeOpenGLWrap* input_volume;
        CrossSectionOpenGLWrapper* cross_section;
        std::map< std::size_t, ObjectOpenGLWrapper* > object_list;

        QColor current_color;

        CoordinateAxes axes;
        bool show_axis = true;



};

#endif // SCENE3D_H
