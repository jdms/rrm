#version 410 core

layout( location = 0 ) in vec3 position;

uniform mat4 mmatrix;
uniform mat4 vmatrix;
uniform mat4 pmatrix;

uniform float scale = 1.0f;

void main( void )
{
    vec3 scaled_position = scale*position;
    gl_Position = pmatrix * vmatrix * mmatrix * vec4( scaled_position, 1.0 );
}
