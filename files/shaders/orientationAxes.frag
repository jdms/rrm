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

in VertexData
{
    vec3 vertice;
    vec3 normal;
    vec4 color;
}VertexIn;

out vec4 outputColor;

noperspective in vec3 dist;

const vec3 lightPos = vec3 ( 1.0 , 1.0 , 1.0 );
const vec3 ambientColor = vec3 ( 0.1 , 0.0 , 0.0 );
const vec3 diffuseColor = vec3 ( 0.5 , 0.0 , 0.0 );
const vec3 specColor = vec3 ( 1.0 , 1.0 , 1.0 );

void main ( void )
{

    float d = min ( dist[0] , min ( dist[1] , dist[2] ) );
    float I = exp2 ( -1.0 * d * d );

    int num_lights = 1;

    vec3 lights[4];

    lights[0] = vec3 (0.0, 0.0,  -8.0 );
    lights[1] = vec3 ( 1.0 , 2.0 , -1.0 );
    lights[2] = vec3 ( -1.0 , 2.0 , -1.0 );
    lights[3] = vec3 ( -1.0 , 2.0 , 1.0 );

    vec3 normal = normalize (VertexIn.normal);

    vec3 viewDirection = normalize(-VertexIn.vertice.xyz);

    vec3 lightDirection = normalize(lights[0] - VertexIn.vertice.xyz );

    float lambertian = max(dot(lightDirection,normal),0.0);
    float specular = 0.0;

    if ( lambertian > 0.0)
    {
          // this is blinn phong
          vec3 halfDirection = normalize(lightDirection + viewDirection);
          float specularAngle = max(dot(halfDirection, normal), 0.0);
          specular = pow(specularAngle, 16.0);

          // this is phong (for comparison)
//          if(mode == 2) {
//            vec3 reflectionDirection = reflect(-lightDirection, normal);
//            float specularAngle = max(dot(reflectionDirection, viewDirection), 0.0);
//            // note that the exponent is different here
//            specular = pow(specularAngle, 4.0);
//          }
    }

    vec4 color_t = VertexIn.color; //vec4(0.7,0.5,0.0,1.0);//VertexIn.color;


    //outputColor = vec4(ambientColor +  lambertian *  diffuseColor +  specular *   specColor, 1.0);
    outputColor = vec4(color_t);

}

