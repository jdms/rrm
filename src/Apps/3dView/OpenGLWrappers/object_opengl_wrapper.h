#ifndef OBJECTOPENGLWRAPPER_H
#define OBJECTOPENGLWRAPPER_H

#include <string>
#include <vector>

#include "Tucano/Shader.hpp"
#include "3dView/model3d_utils.hpp"


class Object;


class ObjectOpenGLWrapper
{

    public:

        ObjectOpenGLWrapper();

        void setShaderDirectory( const std::string& dir );


        void setRawObject( Object* const& obj );
        Object* getRawObject() const ;

        void setMinimum( float mx, float my, float mz );
        void setMaximum( float mx, float my, float mz );


        void setConstantColor( float r, float g, float b );


        bool isVisible() const;
        bool isTesting();

        void initShaders();
        void initBuffers();
        void resetShaders();
        void resetBuffers();

        void reloadShaders();
        void reloadBuffers();


        void reloadGeometry( const std::vector< GLfloat >& vertices,
                             const std::vector< GLfloat >& normals,
                             const std::vector< GLuint >& faces );

        void reloadColors( const std::vector< GLfloat >& colors );


        std::vector< GLfloat > normalizedVertices();


        void init();
        void reset();
        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                   const int& h );

        void clear();
        void clearData();
        void setDefaultValues();

        void singlePassWireFrame();
        void rendering();


    private:

        Object* raw;

        GLuint va_object;
        GLuint vb_vertices;
        GLuint vb_normals;
        GLuint vb_colors;
        GLuint vb_faces;

        GLuint number_of_faces;
        GLuint number_of_vertices;

        Tucano::Shader* shader;
        std::string shader_directory;

        Eigen::Vector3f minimum;
        Eigen::Vector3f maximum;

        bool singlepass;
        bool testing;

};

#endif // OBJECTOPENGLWRAPPER_H
