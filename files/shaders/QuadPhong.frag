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

in VertexData
{
	vec3 vertice;
	vec3 normal;
	vec3 color;
}VertexIn;

noperspective in vec4 dist;

out vec4 outputColor;

void main ( void )
{


    float d = min ( dist[0] , min ( dist[1] , min ( dist[2] , dist[3] ) ) );
    float I = exp2(-1.0 * d * d);


    vec3 lights[1];

    lights[0] = vec3 ( 0.0 ,0.0 , 1.0 );
    //lights[1] = vec3 ( 1.0 , 2.0 , -1.0 );
    //lights[2] = vec3 ( -1.0 , 2.0 , -1.0 );
    //lights[3] = vec3 ( -1.0 , 2.0 , 1.0 );

    int num_lights = 1;

    vec3 newNormal = VertexIn.normal;
    vec3 newVert = VertexIn.vertice.xyz;

    newNormal = normalize ( newNormal );

    vec3 toLight = normalize ( -newVert.xyz );

    vec3 light_dir = vec3 ( 0.0 , 0.0 , 1.0 );
    vec3 eye_dir = normalize ( -newVert.xyz );

	
	vec4 color_t = vec4(0.7,0.5,0.0,1.0);//VertexIn.color;
	
	if (gl_FrontFacing)
	{
		color_t = vec4(0.4,0.4,0.4,1.0);//VertexIn.color;
	}else
	{
	  color_t = vec4(0.1,0.1,0.1,1.0);//VertexIn.color;}
	}
	   
    vec4 la = vec4 ( 0.0 );
    vec4 ld = vec4 ( 0.0 );
    vec4 ls = vec4 ( 0.0 );

    for ( int i = 0; i < num_lights; ++i )
    {
        vec3 light_dir = normalize ( lights[i] );
        vec3 ref = normalize ( -reflect ( light_dir , newNormal ) );
        la += vec4 ( 0.25 / float ( num_lights ) );
        ld += color_t * ( 1.0 / float ( num_lights ) ) * max ( 0.0 , abs ( dot ( newNormal , light_dir ) ) );
        ls += color_t * 0.6 * pow ( max ( 0.0 , dot ( eye_dir , ref ) ) , 5.0 );
    }

    outputColor = vec4 ( la.rgb + ld.rgb, 1.0 );
}
