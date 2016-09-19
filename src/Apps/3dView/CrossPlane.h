#ifndef CROSSPLANE_HPP
#define CROSSPLANE_HPP

#include <QString>

#include <iostream>
#include <vector>


#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"


class CrossPlane
{

	public:
	
		CrossPlane();
		
		void resetBuffers();
		void initBuffers();
		void loadBuffers();
		
		void resetShaders();
		void initShaders();
		void reloadShaders();
		
		
		void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& width, const int& height );
	
        inline void setCurrentDirectory( const std::string& directory ){ shader_directory = QString( directory.c_str() ); }
	
	
	protected:
	
	
		Tucano::Shader* shader_plane;
		
        GLuint va_plane;
        GLuint vb_vertices;
        GLuint slot_vertices;
		GLuint number_of_lines;
		
		QString shader_directory;
	
	
};



#endif
