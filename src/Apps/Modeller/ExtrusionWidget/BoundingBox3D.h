#ifndef BOUNDINGBOX3D_HPP
#define BOUNDINGBOX3D_HPP

#include <QString>

#include <iostream>
#include <vector>


#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"


class BoundingBox3D
{

	public:
	
		BoundingBox3D();
		
		
		void resetBuffers();
		void initBuffers();
		void loadBuffers();
		
		void resetShaders();
		void initShaders();
		void reloadShaders();
		
				
        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& width, const int& height );
		
		
		inline void setCurrentDirectory( const std::string& directory ){ shader_directory = QString( directory ); }
		
    protected:

		Tucano::Shader* shader_boundingbox;
		
		GLuint va_boundingbox;
		GLuint vb_vertices;
		GLuint slot_vertices;
        GLuint number_of_lines;

        QString shader_directory;
		
		
};

#endif
