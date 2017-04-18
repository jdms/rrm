#ifndef VOLUMEOPENGLWRAP_H
#define VOLUMEOPENGLWRAP_H

#include <string>


#include "Volume.h"
#include "Tucano/Shader.hpp"
#include "3dView/Model3DUtils.hpp"



class VolumeOpenGLWrap
{
    public:

        VolumeOpenGLWrap();


        inline void setShaderDirectory( const std::string& directory_ )
        {
            shader_directory = directory_;
        }

        void setVolumeRaw( Volume* const &vol_ );

        inline void init()
        {
            initShaders();
            initBuffers();
        }

        inline void reset()
        {
            resetShaders();
            resetBuffers();

            init();
            createVolumeBox();
        }

        void draw( const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_,
                  const int& h_ );

        inline void reloadShaders()
        {
            shader->reloadShaders();
        }

        inline void reloadBuffers( const std::vector< float >& wireframe_ )
        {
            nlines = (GLuint) wireframe_.size()/NCOORD;

            glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
            glBufferData ( GL_ARRAY_BUFFER , wireframe_.size() * sizeof ( GLfloat ) ,
                                             wireframe_.data() , GL_STATIC_DRAW );
            glBindBuffer ( GL_ARRAY_BUFFER , 0 );

        }

        inline void update(){ createVolumeBox(); }

    private:

        void initShaders();
        void initBuffers();

        void resetShaders();
        void resetBuffers();

        void createVolumeBox();

        static const int NCOORD = 3;
        Volume* volume;

    protected:

        GLuint va_volume;
        GLuint vb_vertices;
        GLuint nlines;

        std::string shader_directory;
        Tucano::Shader* shader;

};

#endif // VOLUMEOPENGLWRAP_H
