#ifndef GRADIENTBACKGROUNDSHADER_HPP
#define GRADIENTBACKGROUNDSHADER_HPP


#include <QString>

#include <iostream>
#include <vector>


#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"


class GradientBackgroundShader
{

	public:
	
        GradientBackgroundShader();
		
		void resetBuffers();
		void initBuffers();		
		
		void resetShaders();
		void initShaders();
		void reloadShaders();
		
		
        void draw( int width, int height );
	
        inline void setCurrentDirectory( const std::string& directory ){ shader_directory = QString( directory.c_str() ); }
	
	
	protected:
	
	
		Tucano::Shader* shader_background;
		
		GLuint va_background;
		GLuint vb_background;
		
		QString shader_directory;
	
	
};



#endif

