#version 410 core

layout( location = 0 ) in vec3 position;

uniform mat4 mmatrix;
uniform mat4 vmatrix;
uniform mat4 pmatrix;


void main( void )
{

    gl_Position = pmatrix * vmatrix * mmatrix * vec4( position, 1.0f );
}
