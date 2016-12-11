#ifndef COORDINATEAXES_H
#define COORDINATEAXES_H


#include <QtCore/QDebug>
#include <QtWidgets/QApplication>
#include <QDir>

#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"
#include "Tucano/BoundingBox3.hpp"

#include "auxiliar.h"


class CoordinateAxes
{
    public:

        CoordinateAxes();

        void init();
        void load();

		void draw(const Eigen::Affine3f& V, const Eigen::Matrix4f& P);

        void reloadShader();
        void initShader( std::string directory );

        void resetBuffers();
        void deleteShaders();


    protected:



        void loadConeAxes();
        void loadCylinderAxes();


    private:

        Tucano::Shader* shader_axes; // create a shader to shader_axis, since there will be shaders only to axes

        GLuint va_coneaxes;
        GLuint bf_faces_coneaxes;
        GLint nfaces_cone;

        GLuint va_cylinderaxes;
        GLuint bf_faces_cylinderaxes;
        GLint nfaces_cylinder;

        std::string current_directory;

};

#endif // COORDINATEAXES_H
