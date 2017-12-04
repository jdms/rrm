#ifndef VOLUMEMESHSHADER_H
#define VOLUMEMESHSHADER_H


#include <iostream>
#include <string>
#include <vector>

#include "shader.h"
#include "region.h"

class RegionShader: public Shader
{

    public:

        RegionShader();
        RegionShader( Region* const& raw_ );


        void setRegion( Region* const& raw_ );

        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
              const int& h );

        void update();
        void clear();


    protected:


        void setDefaultValues();
        void loadBuffers();


        void updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
                                                   const std::vector< GLfloat >& normals_,
                                                   const std::vector< GLuint >& faces_ );

        void updateColorBuffers( const std::vector< GLfloat >& colors_ );


        void updateColorBuffers( std::size_t nvertices_, int r_, int g_, int b_ );


        virtual void initShaders();
        virtual void initBuffers();

        virtual void resetShaders();
        virtual void resetBuffers();




    private:

        GLuint va_volume;
        GLuint vb_vertices;
        GLuint vb_normals;
        GLuint vb_colors;
        GLuint vb_faces;

        GLuint number_of_vertices;
        GLuint number_of_faces;

        Region* raw;

};


#endif // VOLUMEMESHSHADER_H
