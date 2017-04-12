#ifndef OBJECTOPENGLWRAPPER_H
#define OBJECTOPENGLWRAPPER_H

#include <string>

#include "Object.h"
#include "Tucano/Shader.hpp"
#include "3dView/Model3DUtils.hpp"


class ObjectOpenGLWrapper
{
    public:

        ObjectOpenGLWrapper();


        inline void setShaderDirectory( const std::string& directory_ )
        {
            shader_directory = directory_;
        }

        inline void setObjectRaw( Object* const &obj_ )
        {
            object = obj_;
        }


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

            reloadBuffers();
        }

        void draw( const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_,
                  const int& h_ );


        inline void reloadShaders()
        {
            shader->reloadShaders();
        }

        inline void reloadBuffers()
        {
            reloadVertices( object->getSurfaceVertices() );
            reloadFaces( object->getSurfaceFaces() );
        }


        void reloadVertices( const std::vector< float >& vertices_ );
        void reloadFaces( const std::vector< std::size_t >& faces_sizet_ );
        void reloadColors( const std::vector< float >& colors_ );


    private:

        void initShaders();
        void initBuffers();

        void resetShaders();
        void resetBuffers();


        static const int NCOORD = 3;
        Object* object;


    protected:

        GLuint va_object;
        GLuint vb_vertices;
        GLuint vb_normals;
        GLuint vb_colors;
        GLuint vb_faces;

        GLuint nvertices;
        GLuint nfaces;

        std::string shader_directory;
        Tucano::Shader* shader;

};

#endif // OBJECTOPENGLWRAPPER_H
