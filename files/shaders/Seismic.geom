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


layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;

uniform vec2 WIN_SCALE;

noperspective out vec3 dist;

in VertexData
{
	vec4 vertice;
	vec3 normal;
	vec4 color;
} VertexIn[];

out VertexData
{
	vec4 vertice;
	vec3 normal;
	vec4 color;
} VertexOut;



void main(void)
{
	vec2 p0 = WIN_SCALE * (gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w);
	vec2 p1 = WIN_SCALE * (gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w);
	vec2 p2 = WIN_SCALE * (gl_in[2].gl_Position.xy / gl_in[2].gl_Position.w);


	vec2 v0 = p2-p1;
	vec2 v1 = p2-p0;
	vec2 v2 = p1-p0;
	
	float area = abs(v1.x*v2.y - v1.y * v2.x);

	VertexOut.normal   = VertexIn[0].normal;
	VertexOut.color    = VertexIn[0].color;

	dist = vec3(area/length(v0),0,0);
	VertexOut.vertice    = VertexIn[0].vertice;
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	dist = vec3(0,area/length(v1),0);
	VertexOut.vertice    = VertexIn[1].vertice;
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	dist = vec3(0,0,area/length(v2));
	VertexOut.vertice    = VertexIn[2].vertice;
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();


	EndPrimitive();
}
