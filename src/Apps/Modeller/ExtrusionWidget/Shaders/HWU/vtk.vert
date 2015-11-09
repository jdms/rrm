#version 430 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 fragColor;

void main( void )
{
    gl_Position = ProjectionMatrix * ViewMatrix *vec4( position, 1.0f );
    fragColor = color;
}

