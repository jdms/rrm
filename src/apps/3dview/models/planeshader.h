#ifndef PLANESHADER_H
#define PLANESHADER_H


#include <iostream>
#include <string>
#include <vector>

#include "./core/models/shader.h"

class CrossSection;

class PlaneShader: public Shader
{
    public:

        PlaneShader();
        PlaneShader( CrossSection* const& raw_ );

        void setCrossSection( CrossSection* const& raw_ );

        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                                const int& h );

        void update();
        void clear();


    protected:


        void updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
                                    const std::vector< GLfloat >& normals_ );



        void setDefaultValues();
        void createPlane();
        std::vector< float > definePlaneNormals() const;


        virtual void initShaders();
        virtual void initBuffers();

        virtual void resetShaders();
        virtual void resetBuffers();



    private:

        GLuint va_plane;
        GLuint vb_vertices;
        GLuint vb_normals;

        GLuint number_of_vertices;

        CrossSection* csection;
};

#endif // PLANESHADER_H
