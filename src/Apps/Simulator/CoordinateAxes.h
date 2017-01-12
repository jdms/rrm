#ifndef COORDINATEAXES_H
#define COORDINATEAXES_H

#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"
#include "Tucano/BoundingBox3.hpp"

#include "tinyobjloader/tiny_obj_loader.h"

class CoordinateAxes
{
    public:

		CoordinateAxes(); 
		~CoordinateAxes();

        void init();
        void load();

		void draw(const Eigen::Quaternion<float>& _orientation, const Eigen::Vector4f& _viewport);

        void reloadShader();

        void initShader( std::string directory );

		void resetBuffers();

    private:

		void requestOpenGLResources();
		void releaseOpenGLResources();

        Tucano::Shader* shader_axes; // create a shader to shader_axis, since there will be shaders only to axes
		Eigen::Affine3f viewMatrix_;
		Eigen::Matrix4f projetionMatrix;

        GLuint vertex_array_coneaxes;
			GLuint vertex_buffer_faces_indices;
			GLuint vertex_buffer_vertices;
			GLuint vertex_buffer_normals;
			GLuint vertex_buffer_colors;

		int indices_size_;

        std::string current_directory;

};

#endif // COORDINATEAXES_H
