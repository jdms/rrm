#ifndef SURFACESHADER_H
#define SURFACESHADER_H

#include <iostream>
#include <string>
#include <vector>

#include "shader.h"


class SurfaceShader: public Shader
{

    public:

        SurfaceShader();


        void updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
                                                   const std::vector< GLfloat >& normals_,
                                                   const std::vector< GLuint >& faces_ );

        void updateColorBuffers( const std::vector< GLfloat >& colors_ );


        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
              const int& h );

        void clear();


    protected:


        void setDefaultValues();

        virtual void initShaders();
        virtual void initBuffers();

        virtual void resetShaders();
        virtual void resetBuffers();




    private:

        GLuint va_surface;
        GLuint vb_vertices;
        GLuint vb_normals;
        GLuint vb_colors;
        GLuint vb_faces;

        GLuint number_of_vertices;
        GLuint number_of_faces;

};

#endif // SURFACESHADER_H
