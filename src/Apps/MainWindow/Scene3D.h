#ifndef SCENE3D_H
#define SCENE3D_H


#include <vector>
#include <string>



#include <GL/glew.h>

#include "Volume.h"
#include "Object.h"
#include "Region1.h"
#include "CrossSection.h"
#include "VolumeOpenGLWrap.h"
#include "ObjectOpenGLWrapper.h"

#include <QObject>
#include <QOpenGLContext>


class Scene3D: public QObject
{

    Q_OBJECT

    public:

        Scene3D();

        inline void setCurrentDirectory( const std::string& path_ ){ current_directory = path_; }

        bool addVolume(  Volume* const& volume_ );

        bool addObject( Object* const& object_ );

        inline bool addRegion( const Region1* region_ ){ return true; }

        inline void setContext( QOpenGLContext* cnt_ )
        {
            context = cnt_;
            surfacegl = cnt_->surface();
        }


        void draw( const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_,
                   const int& h_ );



    protected:

        std::string current_directory;

        QOpenGLContext* context;
        QSurface* surfacegl;

        VolumeOpenGLWrap* input_volume;
        std::map< std::size_t, ObjectOpenGLWrapper* > object_list;




};

#endif // SCENE3D_H
