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



#ifndef BOUNDARY_HPP
#define BOUNDARY_HPP

class Boundary
{
	public: 
	
        Boundary():origin_x( 0.0f ), origin_y( 0.0f ), origin_z( 0.0f ), width( 0.0f ), height( 0.0f ), depth( 0.0f ){}

        void edit( const float& x, const float& y, const float& z, const float& w, const float& h, const float& d ){ origin_x = x; origin_y = y; origin_z = z;
                                                 width = w; height = h; depth = d;      }
		

        inline float getX(){ return origin_x; }
        inline float getY(){ return origin_y; }
        inline float getZ(){ return origin_z; }
        inline float getWidth(){ return width; }
        inline float getHeight(){ return height; }
        inline float getDepth(){ return depth; }
        inline void getOrigin( float& x, float &y, float& z ){ x = origin_x; y = origin_y; z = origin_z; }


    protected:

        float origin_x;
        float origin_y;
        float origin_z;

        float width;
        float height;
        float depth;
		
};


#endif
