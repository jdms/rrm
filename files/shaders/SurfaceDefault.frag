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


#version 410 core

in VertexData
{
    vec4 vertice;
    vec4 normal;
    vec4 color;
}VertexIn;

noperspective in vec3 dist;

// For while, all transformations come from the Celer::Camera.
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform float alfa;

layout(location = 0) out vec4 output_color;

void main ( void )
{

        int  number_lights = 2;
        vec4 lights_positions[2];

        lights_positions[0] = ViewMatrix * vec4(0.0,1.0,0.0,0.0);
        lights_positions[1] = ViewMatrix * vec4(0.0,-1.0,0.0,0.0);

        /// P - point on surface
        /// N - surface_normal
        /// L - light diretion   = Point Light = (light position - P)
        /// R - refletion vector = Reflection the negative light direction around the plane define by N
        /// V - viewer vector    = Negate the surface point (-P)

        vec3 surface_normal = normalize(VertexIn.normal.xyz);
        vec3 surface_point  = VertexIn.vertice.xyz;

        vec3 view_direction = normalize ( -surface_point.xyz );
        vec4 color_t = vec4 (  VertexIn.color.xyz , 1.0 );


        vec3 light_direction = vec3(0.0);
        vec3 reflection = vec3(0.0);
        vec4 la = vec4(0.0);
        vec4 ld = vec4(0.0);
        vec4 ls = vec4(0.0);

        float ambient_strenght  = 0.4;
        float difuse_strenght   = 0.5;
        float specular_strenght = 0.02;
        float distanceToLight   = 0.0;
        float distanceToCamera  = length(surface_point);


        for ( int i = 0 ; i < number_lights; i++)
        {
                if ( gl_FrontFacing ) // is the fragment part of a front face?
                {

                    //color_t = vec4 ( 0.737, 0.501, 0.741 , 1.0 );
                    //color_t = vec4 ( 0.701, 0.803, 0.890, 1.0 );
                    //surface_normal = -surface_normal;
                }
                else // fragment is part of a back face
                {
                        //color_t = vec4 ( 0.701, 0.803, 0.890, 1.0 );
                }

            light_direction =  normalize(lights_positions[i].xyz);
            reflection = normalize ( -reflect ( light_direction , surface_normal ) );
            la += ambient_strenght  * vec4 ( 0.2 / float(number_lights) );
            //ld += difuse_strenght   * color_t * (1.0 / float(number_lights)) * max ( 0.0 , dot ( surface_normal , light_direction ) );
            ld += difuse_strenght  * color_t * (1.0 / float(number_lights)) * abs( dot ( surface_normal , light_direction ) );
            ls += specular_strenght * vec4(1.0) * (1.0 / float(number_lights)) * pow ( max ( 0.0 , dot ( view_direction , reflection ) ) , 128.0 );
        }


        float d = min ( dist[0] , min ( dist[1] , dist[2] ) );
        float I = exp2 ( -1.0 * d * d );

        I = 0.0;

        output_color = I * vec4 ( 0.5*(la.rgb + ld.rgb), 1.0 ) + ( 1.0 - I ) * vec4 ( la.rgb + ld.rgb + ls.rgb, alfa);
		
		

}


