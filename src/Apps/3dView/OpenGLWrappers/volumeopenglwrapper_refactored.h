#ifndef VOLUMEOPENGLWRAPPER_REFACTORED_H
#define VOLUMEOPENGLWRAPPER_REFACTORED_H

#include <string>

#include "Tucano/Shader.hpp"
#include "3dView/Model3DUtils.hpp"

class Volume;


class VolumeOpenGLWrapper_Refactored
{
    public:

        VolumeOpenGLWrapper_Refactored();


        void setShaderDirectory( const std::string& dir );


        void setRawVolume( Volume* const& vol );
        Volume* getRawVolume() const ;


        bool isVisible() const;

        void getOrigin( double& w, double& h, double& d ) const ;

        double getWidth() const ;
        double getHeight() const ;
        double getDepth() const ;

        void getMinimum( float& mx, float& my, float &mz ) const;
        void getMaximum( float& mx, float& my, float &mz ) const;


        void createVolumeBox();


        void updateGeometry();
        void defineNormals();


        void initShaders();
        void initBuffers();
        void resetShaders();
        void resetBuffers();
        void reloadShaders();
        void reloadBuffers( const std::vector< float >& wireframe );


        void init();
        void reset();
        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                   const int& h );

        void clear();
        void clearData();
        void setDefaultValues();



    private:

        Volume* raw;

        std::string shader_directory;

        GLuint va_volume;
        GLuint vb_volume_vertices;
        GLuint vb_volume_normals;
        GLuint number_of_vertices;

        Tucano::Shader* shader;

        Eigen::Vector3f minimum;
        Eigen::Vector3f maximum;


};

#endif // VOLUMEOPENGLWRAPPER_REFACTORED_H
