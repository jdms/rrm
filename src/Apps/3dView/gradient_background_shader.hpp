/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



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

		void init();
		
		
        void draw( const int& width, const int& height );
	
        inline void setCurrentDirectory( const std::string& directory ){ shader_directory = QString( directory.c_str() ); }
	
	
	protected:
	
	
		Tucano::Shader* shader_background;
		
		GLuint va_background;
		GLuint vb_background;
		
		QString shader_directory;
	
	
};



#endif

