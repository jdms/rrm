#version 430 core

in vec3 fragColor;
out vec4 color;

void main( void )
{
    color = vec4(/* 1.0, 0.0, 0.0*/fragColor, 1.0 );
}

