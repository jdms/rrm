#ifndef VOLUMESHADER_H
#define VOLUMESHADER_H


#include <iostream>
#include <string>
#include <vector>

#include "shader.h"
#include "volume.h"


class VolumeShader: public Shader
{

    public:


        VolumeShader();
        VolumeShader( Volume* const& raw_ );


        void setVolume( Volume* const& raw_ );



        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                                 const int& h );

        void updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
                                    const std::vector< GLfloat >& normals_ );

        void clear();


    protected:


        void setDefaultValues();
        void createVolumeMesh();
        std::vector< float > defineVolumeNormals() const;


        virtual void initShaders();
        virtual void initBuffers();

        virtual void resetShaders();
        virtual void resetBuffers();




    private:

        GLuint va_volume;
        GLuint vb_vertices;
        GLuint vb_normals;

        GLuint number_of_vertices;

        Volume* raw;

};

#endif // VOLUMESHADER_H
