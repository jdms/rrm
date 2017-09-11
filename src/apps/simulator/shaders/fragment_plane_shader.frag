#version 410 core

layout ( location = 0 ) out vec4 color;

uniform vec3 in_color;


void main( void )
{

    color = vec4( in_color, 1.0f );

}
