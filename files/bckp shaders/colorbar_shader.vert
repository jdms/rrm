#version 410 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 in_color;


uniform mat4 mmatrix;
uniform mat4 vmatrix;
uniform mat4 pmatrix;

out vec4 color;

void main( void )
{

    color = vec4( in_color, 1.0f );
    gl_Position = pmatrix * vmatrix /* mmatrix*/ * vec4( position, 1.0 );

}
