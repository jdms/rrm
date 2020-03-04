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


 
layout ( location = 0 ) out vec4 FragColor;

uniform vec4 l_ambient = vec4( 0.1, 0.1, 0.1, 1.0 );
uniform vec4 l_diffuse  = vec4( 0.9, 0.9, 0.9, 1.0 );
uniform vec4 l_specular = vec4( 0.9, 0.9, 0.9, 1.0 );

uniform vec4 m_ambient = vec4( 1.0, 1.0, 1.0, 1.0 );
uniform vec4 m_diffuse  = vec4( 0.6, 0.6, 0.6, 1.0 );
uniform vec4 m_specular = vec4( 0.4, 0.4, 0.4, 1.0 );

uniform bool is_face;
uniform bool is_preview;
uniform bool has_shading;

in vec3 fN;
in vec3 fL;
in vec3 fE;
in vec3 fcolor;


vec4 gourad_shading()
{

    vec3 N = normalize( fN );
    vec3 L = normalize( fL );
    vec3 E = normalize( fE );
    vec3 R = reflect( -L, N );


    float shininess = 5.0f;


    float NdotL = dot( N, L );
    float Kd = max( abs( NdotL ), 0.0f );
    float Ks =  0.6f * pow ( max ( dot ( R , E ), 0.0f ) , shininess );


    vec4 diffuse  = Kd * vec4( fcolor.xyz, 1.0f );
    vec4 specular = Ks * vec4( fcolor.xyz, 1.0f );

    return vec4( diffuse.xyz + specular.xyz, 1.0f );
}


void main(void)
{


    vec4 color = vec4( fcolor.xyz, 1.0 );

    if( has_shading == true )
        color = gourad_shading();


    vec4 surface_color =     color;
    vec4 edge_color =  0.5f* color;


    if( is_face == true )
        FragColor = surface_color;
    else
        FragColor = edge_color;

    if( is_preview == true )
        FragColor.a = 0.2f;
    else
        FragColor.a = 1.0f;

}
