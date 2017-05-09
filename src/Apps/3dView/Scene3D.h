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
        }


        void draw( const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_,
                   const int& h_ );

        inline void updateCrossSection( double depth_ ){ cross_section->reloadBuffers( depth_ ); }



    signals:

        void updateCanvas();


    public slots:

        void updateScene();
        void clearScene();

        inline void setCurrentColor( const QColor& c_ ){ current_color = c_; }
        inline void setCurrentColor( int red_, int green_, int blue_ ){
                                     current_color = QColor( red_, green_, blue_ ); }


    protected:

        std::string current_directory;

        QOpenGLContext* context;
        QSurface* surfacegl;

        VolumeOpenGLWrap* input_volume;
        CrossSectionOpenGLWrapper* cross_section;
        std::map< std::size_t, ObjectOpenGLWrapper* > object_list;

        QColor current_color;






};

#endif // SCENE3D_H
