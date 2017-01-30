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


#ifndef BOUNDINGBOX3D_HPP
#define BOUNDINGBOX3D_HPP


#include <iostream>
#include <vector>


#include <QString>
#include <QVector3D>


#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"


#include "./src/Core/Geology/Models/BoundaryRenderable.h"



class BoundingBox3D: public BoundaryRenderable
{

	public:
	

        BoundingBox3D( float min_x, float min_y, float min_z, float w, float h, float d = 0.5f );
		
        void initData();
        void init();
        void create();


		void resetBuffers();
		void initBuffers();
        void loadBuffers( const std::vector< float >& wireframe );
		
		void resetShaders();
		void initShaders();
		void reloadShaders();
		
				
        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w, const int& h );
		
		
        inline void setCurrentDirectory( const std::string& directory ){ shader_directory = QString( directory.c_str() ); }
        inline bool initialized(){ return is_initialized;}
		
        inline float getWidth(){ return width; }
        inline float getHeight(){ return height; }
        inline float getDepth(){ return depth;  }

        inline void getMinimum( float& min_x, float& min_y, float& min_z )
        {
            min_x = minx;
            min_y = miny;
            min_z = minz;
        }

        inline void getDimension( float& w, float& h, float& d )
        {
            w = width;
            h = height;
            d = depth;
        }


        void clear();
        void update();


    protected:


        float width;
        float height;
        float depth;

		Tucano::Shader* shader_boundingbox;
		
		GLuint va_boundingbox;
		GLuint vb_vertices;
		GLuint slot_vertices;
        GLuint number_of_lines;

        QString shader_directory;
        bool is_initialized;


        float minx;
        float miny;
        float minz;
		
};

#endif
