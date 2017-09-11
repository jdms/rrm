#version 410 core

layout( location = 0 ) in vec3 position;
layout( location = 2 ) in vec3 normal;
layout( location = 3 ) in mat4 mmatrix;


uniform mat4 vmatrix;
uniform mat4 pmatrix;


uniform vec3 light_pos = vec3( 10.0f, 10.0f, 10.0f );

out VS_OUT
{
    vec3 normal;
    vec3 light;
    vec3 view;
} vs_out;

flat out int InstanceID;

void main( void )
{
    mat3 nmatrix = mat3( vmatrix*mmatrix );
    nmatrix = inverse( nmatrix );
    nmatrix = transpose( nmatrix );


    vec4 vs_position = vmatrix * mmatrix* vec4( position, 1.0 );

    vs_out.normal = normalize( nmatrix*( -1*normal ) );
    vs_out.light = light_pos - vs_position.xyz;
    vs_out.view = -vs_position.xyz;


    gl_Position = pmatrix * vmatrix * mmatrix *vec4( position, 1.0 );
    InstanceID = gl_InstanceID;


}
