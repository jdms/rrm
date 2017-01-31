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

//#version 410 core
//
//layout( lines_adjance ) in;
//layout( triangle_strip, max_vertices = 3 ) out;
//
//uniform vec2 WIN_SCALE;
//
//noperspective out vec3 dist;
//
//in VertexData
//{
//	vec4 color;
//} VertexIn[4];
//
//out VertexData
//{
//   vec4 color;
//} VertexOut;
//
//
//
//void main(void)
//{
//	vec2 p0 = WIN_SCALE * (gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w);
//	vec2 p1 = WIN_SCALE * (gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w);
//	vec2 p2 = WIN_SCALE * (gl_in[2].gl_Position.xy / gl_in[2].gl_Position.w);
//
//
//	float a = length ( p1 - p2 );
//	float b = length ( p2 - p0 );
//	float c = length ( p1 - p0 );
//
//	float alpha = acos ( ( b * b + c * c - a * a ) / ( 2.0 * b * c ) );
//	float beta = acos ( ( a * a + c * c - b * b ) / ( 2.0 * a * c ) );
//
//	float ha = abs ( c * sin ( beta ) );
//	float hb = abs ( c * sin ( alpha ) );
//	float hc = abs ( b * sin ( alpha ) );
//
//
//	dist = vec3(ha, 0.0, 0.0);
//	VertexOut.color = VertexIn[0].color;
//	gl_Position = gl_in[0].gl_Position;
//	EmitVertex();
//
//	dist = vec3(0.0, hb, 0.0);
//	VertexOut.color = VertexIn[1].color;
//	gl_Position = gl_in[1].gl_Position;
//	EmitVertex();
//
//	dist = vec3(0.0, 0.0, hc);
//	VertexOut.color = VertexIn[2].color;
//	gl_Position = gl_in[2].gl_Position;
//	EmitVertex();
//
//
//	EndPrimitive();
//}


#version 410 core

layout(lines) in;
layout(line_strip, max_vertices=2) out;


// For while, all transformations come from the Celer::Camera.
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;


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

	VertexOut.vertice = VertexIn[0].vertice;
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	VertexOut.vertice = VertexIn[1].vertice;
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	EndPrimitive();
}
