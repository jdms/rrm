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

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 texcoord;


void main(void)
{
    gl_Position = vec4( 1.0, 1.0, 0.0, 1.0 );
    texcoord = vec2( 1.0, 1.0 );
    EmitVertex();

    gl_Position = vec4(-1.0, 1.0, 0.0, 1.0 );
    texcoord = vec2( 0.0, 1.0 );
    EmitVertex();

    gl_Position = vec4( 1.0,-1.0, 0.0, 1.0 );
    texcoord = vec2( 1.0, 0.0 );
    EmitVertex();

    gl_Position = vec4(-1.0,-1.0, 0.0, 1.0 );
    texcoord = vec2( 0.0, 0.0 );
    EmitVertex();

    EndPrimitive();
}
