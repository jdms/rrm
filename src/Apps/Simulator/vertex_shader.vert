#version 410 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 color;

uniform mat4 mvp_matrix;
out vec3 fragColor;

void main( void )
{
    gl_Position = mvp_matrix*vec4( position, 1.0f );
    fragColor = color;
}

//#version 410 core

//layout( location = 0 ) in vec3 position;
//layout( location = 1 ) in vec3 color;
//layout( location = 2 ) in vec3 normal;

//uniform mat4 mvp_matrix;
//out vec3 fragColor;

//void main( void )
//{
//    gl_Position = mvp_matrix*vec4( position, 1.0f );
//    fragColor = color;
//}
