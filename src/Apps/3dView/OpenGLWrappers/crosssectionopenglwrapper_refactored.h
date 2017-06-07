#ifndef CROSSSECTIONOPENGLWRAPPER_REFACTORED_H
#define CROSSSECTIONOPENGLWRAPPER_REFACTORED_H

#include <string>

#include "Tucano/Shader.hpp"
#include "3dView/Model3DUtils.hpp"


class CrossSectionOpenGLWrapper_Refactored
{
    public:

        CrossSectionOpenGLWrapper_Refactored();

        void setShaderDirectory( const std::string& dir );

        void setDepth( double d );
        double getDepth() const ;

        bool isVisible() const;
        void setVisible( bool status );



        void setMinimum( float mx, float my, float mz );
        void setMaximum( float mx, float my, float mz );


        void createPlane();


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

        std::string shader_directory;

        GLuint va_csection;
        GLuint vb_csection_lines;
        GLuint vb_csection_normals;
        GLuint number_of_vertices;

        Tucano::Shader* shader;

        Eigen::Vector3f minimum;
        Eigen::Vector3f maximum;

        float depth;
        bool is_visible;


};

#endif // CROSSSECTIONOPENGLWRAPPER_REFACTORED_H
