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

out vec4 out_Color;

in vec2 texcoord;
uniform ivec2 viewportSize;


void main(void)
{
        float y = gl_FragCoord.y/viewportSize.y;
        // Video
        out_Color = vec4(vec3(0.605,0.691,0.781) + (1-y)* vec3(0.1,0.1,0.1),1.0);
        //out_Color = vec4(vec3(0.5,0.5,0.5) + (y)* vec3(0.2,0.2,0.2),1.0);
        // Paper Figures
        //out_Color = vec4(1.0,1.0,1.0,1.0);
}

