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

layout(lines_adjacency) in;
layout(triangle_strip, max_vertices=4) out;

uniform vec2 WIN_SCALE;
uniform vec4 ClipPlane;
uniform vec3 ClipPlaneCentre;
uniform bool isClip;

noperspective out vec4 dist;


// For while, all transformations come from the Celer::Camera.
//uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
//uniform mat4 ProjectionMatrix;


in VertexData
{
	vec3 vertice;
	vec3 normal;
	vec3 color;
} VertexIn[];

out VertexData
{
	vec3 vertice;
	vec3 normal;
	vec3 color;
} VertexOut;


bool userClipImplicit(vec4 plane , vec3 p )
{
    float fx = plane.x*p.x + plane.y*p.y + plane.z*p.z + plane.w;

    if ( fx >= 0 )
    {
        return true;
    }
    else
    {
        return false;
    }

    return false;

}

bool userClip ( vec3 n , vec3 c , vec3 p )
{

    vec3 pc = p - c;

    float d = dot ( pc , n );

    if ( d >= 0 )
    {
        return true;
    }
    else
    {
        return false;
    }

    return false;
}



void main(void)
{


	vec2 p0 = WIN_SCALE * (gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w);
	vec2 p1 = WIN_SCALE * (gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w);
	vec2 p2 = WIN_SCALE * (gl_in[2].gl_Position.xy / gl_in[2].gl_Position.w);
	vec2 p3 = WIN_SCALE * (gl_in[3].gl_Position.xy / gl_in[3].gl_Position.w);

	vec2 v0 = p1 - p0;
	vec2 v1 = p1 - p2;
	vec2 v2 = p2 - p0;
	vec2 v3 = p1 - p3;
	vec2 v4 = p3 - p2;
	vec2 v5 = p3 - p0;
	vec2 v6 = p2 - p0;

	float area1 = abs(v1.x * v6.y - v1.y * v6.x);
	float area2 = abs(v1.x * v4.y - v1.y * v4.x);
	float area3 = abs(v0.x * v5.y - v0.y * v5.x);
	float area4 = abs(v2.x * v5.y - v2.y * v5.x);

	 vec3 face_normal = normalize (
	            cross (gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz,
	                gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz));


	mat3 normalMatrix = mat3(ViewMatrix);
	normalMatrix = inverse(normalMatrix);
	normalMatrix = transpose(normalMatrix);

	VertexOut.normal   =normalMatrix * face_normal;
	VertexOut.color    = VertexIn[0].color;


        bool clipped = false;

        if ( isClip )
        {
            if ( userClipImplicit(ClipPlane,VertexIn[0].vertice.xyz) ||
                 userClipImplicit(ClipPlane,VertexIn[1].vertice.xyz) ||
                 userClipImplicit(ClipPlane,VertexIn[2].vertice.xyz) ||
                 userClipImplicit(ClipPlane,VertexIn[3].vertice.xyz))
            {
                clipped = true;
            }else
            {
                clipped = false;
            }

        }else
        {
            clipped = false;
        }



	dist = vec4(area4/length(v4), area3/length(v3), 0, 0);
	VertexOut.vertice = VertexIn[0].vertice;

        if ( clipped )
        {
            gl_Position = vec4(0.0,0.0,0.0,1.0);
        }else
        {
            gl_Position = gl_in[0].gl_Position;
        }
	EmitVertex();

	dist = vec4(area2/length(v4), 0, 0, area1/length(v2));
	VertexOut.vertice = VertexIn[1].vertice;

        if ( clipped )
        {
            gl_Position = vec4(0.0,0.0,0.0,1.0);
        }else
        {
            gl_Position = gl_in[1].gl_Position;
        }
	EmitVertex();

	dist = vec4(0, area2/length(v3), area1/length(v0), 0);
	VertexOut.vertice = VertexIn[2].vertice;

        if ( clipped )
        {
            gl_Position = vec4(0.0,0.0,0.0,1.0);
        }else
        {
            gl_Position = gl_in[2].gl_Position;
        }
	EmitVertex();

	dist = vec4(0, 0, area3/length(v0), area4/length(v2));
	VertexOut.vertice = VertexIn[3].vertice;

        if ( clipped )
        {
            gl_Position = vec4(0.0,0.0,0.0,1.0);
        }else
        {
            gl_Position = gl_in[3].gl_Position;
        }
	EmitVertex();

	EndPrimitive();
}
