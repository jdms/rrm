#ifndef CROSSSECTIONOPENGLWRAPPER_H
#define CROSSSECTIONOPENGLWRAPPER_H

#include <string>


#include "Core/Geology/Models/Object.h"
#include "Tucano/Shader.hpp"
#include "3dView/Model3DUtils.hpp"


class CrossSectionOpenGLWrapper
{
    public:

        CrossSectionOpenGLWrapper();

        inline void setShaderDirectory( const std::string& directory_ )
        {
            shader_directory = directory_;
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

            reloadBuffers( 0.0 );
        }

        inline void clear()
        {
            resetShaders();
            resetBuffers();

//            if( object != nullptr )
//                object->clear();
//            object = nullptr;
        }

        void draw( const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_,
                  const int& h_ );


        inline void reloadShaders()
        {
            shader->reloadShaders();
        }

        inline void reloadBuffers( double depth_ )
        {
            std::vector< double > plane_ =
            {
                maxx, maxy, depth_,
                minx, maxy, depth_,
                maxx, miny, depth_,
                minx, miny, depth_
            };

            nvertices = plane_.size()/NCOORD;

            Eigen::Affine3f matrix = Model3DUtils::normalizePointCloud( minx, maxx, miny, maxy, minz, maxz );
            std::vector< float > unit_vertices_;

            for( size_t i = 0; i < nvertices; ++i )
            {
                Eigen::Vector3f v( plane_[ 3*i ], plane_[ 3*i + 1 ], plane_[ 3*i + 2 ] );
                v = matrix*v;
                unit_vertices_.push_back( v.x() );
                unit_vertices_.push_back( v.y() );
                unit_vertices_.push_back( v.z() );
                unit_vertices_.push_back( 1.0f );
            }

            nvertices = unit_vertices_.size();

            glBindBuffer ( GL_ARRAY_BUFFER, vb_vertices );
            glBufferData ( GL_ARRAY_BUFFER, unit_vertices_.size() * sizeof ( float ), unit_vertices_.data() ,
                           GL_STATIC_DRAW );
            glBindBuffer ( GL_ARRAY_BUFFER, 0 );

        }



        void reloadVertices( const std::vector< double >& vertices_ );


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


        static const int NCOORD = 3;
//        CrossSection* csection;

    protected:

        GLuint va_csection;
        GLuint vb_vertices;

        GLuint nvertices;

        std::string shader_directory;
        Tucano::Shader* shader;

        float minx = -0.5f;
        float maxx = 0.5f;
        float miny = -0.5f;
        float maxy = 0.5f;
        float minz = -0.5f;
        float maxz = 0.5f;



};

#endif // CROSSSECTIONOPENGLWRAPPER_H
