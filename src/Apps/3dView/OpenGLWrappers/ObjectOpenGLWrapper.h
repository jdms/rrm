#ifndef OBJECTOPENGLWRAPPER_H
#define OBJECTOPENGLWRAPPER_H

#include <string>


#include "Core/Geology/Models/Object.h"
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
            int red_ = 0, green_ = 0, blue_ = 0;
            object->getColor( red_, green_, blue_ );
            setConstantColor( red_, green_, blue_ );
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


        inline void clear()
        {
            resetShaders();
            resetBuffers();

            if( object != nullptr )
                object->clear();
            object = nullptr;
        }

        void draw( const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_,
                  const int& h_ );


        inline void reloadShaders()
        {
            shader->reloadShaders();
        }

        inline void reloadBuffers()
        {
            std::vector< double > vertices_ = object->getSurfaceVertices();

            int r_, g_, b_;
            object->getColor( r_, g_, b_ );
            setConstantColor( r_, g_, b_ );

            reloadVertices( vertices_ );
            reloadFaces( object->getSurfaceFaces() );
            reloadColors();
        }


        inline void setConstantColor( int r_, int g_, int b_ )
        {
            current_color.red = (float)(r_/255.f);
            current_color.green = (float)(g_/255.f);
            current_color.blue = (float)( b_/255.f );
        }

        void reloadVertices( const std::vector< double >& vertices_ );
        void reloadFaces( const std::vector< std::size_t >& faces_sizet_ );
        void reloadColors( std::vector< float > colors_ = std::vector< float >() );


        inline void setBoundingBox( float xmin_, float xmax_, float ymin_, float ymax_,
                                    float zmin_, float zmax_ )
        {
            minx = xmin_;
            miny = ymin_;
            minz = zmin_;
            maxx = xmax_;
            maxy = ymax_;
            maxz = zmax_;
        }

    private:

        void initShaders();
        void initBuffers();

        void resetShaders();
        void resetBuffers();


        struct Color
        {
            float red;
            float green;
            float blue;
        };

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

        Color current_color;

        float minx = -0.5f;
        float maxx = 0.5f;
        float miny = -0.5f;
        float maxy = 0.5f;;
        float minz = -0.5f;;
        float maxz = 0.5f;;

};

#endif // OBJECTOPENGLWRAPPER_H
