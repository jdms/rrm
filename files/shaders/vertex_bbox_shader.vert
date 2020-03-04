#version 330 core

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

uniform mat4 mmatrix;
uniform mat4 vmatrix;
uniform mat4 pmatrix;

uniform float scale = 1.0f;

void main( void )
{
    vec3 scaled_position = scale*position;
    gl_Position = pmatrix * vmatrix * mmatrix * vec4( scaled_position, 1.0 );
}
