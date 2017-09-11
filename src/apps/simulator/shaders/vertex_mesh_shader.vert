#version 410 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 normal;
layout( location = 2 ) in vec3 color;

out float gl_ClipDistance[1];
uniform vec4 ClipPlane;

uniform mat4 mmatrix;
uniform mat4 vmatrix;
uniform mat4 pmatrix;

uniform bool edge = false;
uniform float scale = 1.0f;
//uniform int index = 0;


uniform vec3 light_pos = vec3( 10.0f, 10.0f, 10.0f );
out vec3 in_color;

out VS_OUT
{
    vec3 normal;
    vec3 light;
    vec3 view;
} vs_out;


void main( void )
{
//    mat3 nmatrix = mat3( vmatrix*mmatrix );
//    nmatrix = inverse( nmatrix );
//    nmatrix = transpose( nmatrix );

    vec3 scaled_position = scale*position;
    vec4 vs_position = vmatrix * mmatrix* vec4( scaled_position, 1.0 );

//    vs_out.normal = normalize( nmatrix*( /*-1**/normal ) );
//    vs_out.light = light_pos - vs_position.xyz;
//    vs_out.view = -vs_position.xyz;


    if( edge == true )
        in_color = vec3( 0.0f, 0.0f, 0.0f );
    else
        in_color = color;




    gl_Position = pmatrix * vmatrix * mmatrix *vec4( scaled_position, 1.0 );


//    gl_ClipDistance[0] = dot( mmatrix * vec4( position, 1.0 ), ClipPlane);

}

