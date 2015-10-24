#version 330 core

in vec3 fragColor;
out vec4 color;

void main( void )
{
    color = vec4( 1.0f, 0.0f, 0.0f/*fragColor*/, 1.0f );
}
