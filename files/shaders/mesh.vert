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


layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;
layout ( location = 2 ) in vec4 color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform vec4 lightPosition = vec4( 0.0, 2.0, 0.0, 0.0 );

out vec3 fN;
out vec3 fL;
out vec3 fE;
out vec3 fcolor;

void main(void)
{

    mat3 NormalMatrix = transpose( inverse( mat3 ( ViewMatrix ) ) );
    vec3 n = normalize( normal );

    vec4 eyePosition = ViewMatrix * ModelMatrix * vec4( position, 1.0 );
    fN = NormalMatrix * n;
    fL = lightPosition.xyz - eyePosition.xyz;
    fE = -eyePosition.xyz;

    fcolor = color.xyz;
    gl_Position = ProjectionMatrix * eyePosition;
}
