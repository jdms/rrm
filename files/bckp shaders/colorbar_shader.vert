#version 410 core

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

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 in_color;


uniform mat4 mmatrix;
uniform mat4 vmatrix;
uniform mat4 pmatrix;

out vec4 color;

void main( void )
{

    color = vec4( in_color, 1.0f );
    gl_Position = pmatrix * vmatrix /* mmatrix*/ * vec4( position, 1.0 );

}
