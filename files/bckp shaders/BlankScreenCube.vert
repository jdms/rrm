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

layout(location = 0) in vec3 vertices;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec3 colors;

uniform float scale = 1.0f;

out VertexData
{
	vec4 vertice;
	vec3 normal;
	vec4 color;
} VertexOut;

// For while, all transformations come from the Celer::Camera.
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void)
{

	mat3 normalMatrix = mat3(ViewMatrix);
	normalMatrix = inverse(normalMatrix);
	normalMatrix = transpose(normalMatrix);


        VertexOut.vertice  = ViewMatrix * vec4( scale*vertices.xyz,1.0);
	VertexOut.normal   = normalMatrix * normals;
	VertexOut.color    = vec4(1.0,0.0,0.0,1.0);

        gl_Position = ProjectionMatrix * ViewMatrix *vec4((scale*vertices.xyz),1.0);

}


