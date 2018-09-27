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


layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;

uniform vec2 WIN_SCALE;
uniform vec4 ClipPlane;
uniform vec3 ClipPlaneCentre;
uniform bool isClip;

uniform mat4 ViewMatrix;

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


    vec2 v0 = p2-p1;
    vec2 v1 = p2-p0;
    vec2 v2 = p1-p0;

    float area = abs(v1.x*v2.y - v1.y * v2.x);

	vec4 vertices0 = ViewMatrix * VertexIn[0].vertice;
	vec4 vertices1 = ViewMatrix * VertexIn[1].vertice;
	vec4 vertices2 = ViewMatrix * VertexIn[2].vertice;

    vec3 face_normal = normalize (
               cross (vertices1.xyz - vertices0.xyz,
                      vertices2.xyz - vertices0.xyz));

    //VertexOut.normal   = -face_normal;
    VertexOut.color    = VertexIn[0].color;

    bool clipped = false;

    if ( isClip )
    {
        if ( userClipImplicit(ClipPlane,VertexIn[0].vertice.xyz) ||
             userClipImplicit(ClipPlane,VertexIn[1].vertice.xyz) ||
             userClipImplicit(ClipPlane,VertexIn[2].vertice.xyz))
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


    /// First Vertex
    dist = vec3(area/length(v0),0,0);
    VertexOut.vertice    = ViewMatrix * VertexIn[0].vertice;

    if ( clipped )
    {
        gl_Position = vec4(0.0,0.0,0.0,1.0);
    }else
    {
		VertexOut.normal   = VertexIn[0].normal;
        gl_Position = gl_in[0].gl_Position;
    }


    EmitVertex();

    /// Second Vertex
    VertexOut.color    = VertexIn[1].color;

    dist = vec3(0,area/length(v1),0);
    VertexOut.vertice    = ViewMatrix * VertexIn[1].vertice;

    if ( clipped )
    {
        gl_Position = vec4(0.0,0.0,0.0,1.0);
    }else
    {
		VertexOut.normal   = VertexIn[1].normal;
        gl_Position = gl_in[1].gl_Position;
    }


    EmitVertex();

    /// Third Vertex
    VertexOut.color    = VertexIn[2].color;

    dist = vec3(0,0,area/length(v2));
    VertexOut.vertice    = ViewMatrix * VertexIn[2].vertice;

    if ( clipped )
    {
        gl_Position = vec4(0.0,0.0,0.0,1.0);
    }else
    {
		VertexOut.normal   = VertexIn[2].normal;
        gl_Position = gl_in[2].gl_Position;
    }


    EmitVertex();


    EndPrimitive();
}