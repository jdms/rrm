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
