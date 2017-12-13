#version 410 core

in vec3 in_color;
out vec4 out_color;

in VS_OUT
{
    vec3 normal;
    vec3 light;
    vec3 view;
} fs_in;

uniform int index = 0;

//uniform vec3 ambient_albedo = vec3( 0.3f, 0.0f, 0.0f );
//uniform vec3 diffuse_albedo = vec3( 0.3f, 0.0f, 0.0f );
//uniform vec3 specular_albedo = vec3( 0.1f ); // color of specular light ( it attenuates or not the light on the surface )
//uniform float specular_power = 16.0f; // size of the light spot

//vec3 ambient_albedo = vec3( 0.3f, 0.0f, 0.0f );
//vec3 diffuse_albedo = vec3( 0.3f, 0.0f, 0.0f );
//vec3 specular_albedo = vec3( 0.1f ); // color of specular light ( it attenuates or not the light on the surface )
//float specular_power = 16.0f; // size of the light spot


void main( void )
{


//    ambient_albedo = in_color;
//    diffuse_albedo = in_color;

//    vec3 N = normalize( fs_in.normal );
//    vec3 L = normalize( fs_in.light );
//    vec3 V = normalize( fs_in.view );

//    vec3 R = reflect( -L, N ) ;

//    vec3 diffuse = max( dot( N, L ),  0.0f )*diffuse_albedo;
//    vec3 specular = pow( max( dot( R, V ),  0.0f ), specular_power )*specular_albedo;

    if( index == 0 )
        out_color = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
    else if( index == 1 )
        out_color = vec4( 0.0f, 1.0f, 0.0f, 1.0f );
    else if( index == 2 )
        out_color = vec4( 0.0f, 0.0f, 1.0f, 1.0f );
    else if( index == 3 )
        out_color = vec4( 0.0f, 1.0f, 1.0f, 1.0f );
    else if( index == 4 )
        out_color = vec4( 1.0f, 0.0f, 1.0f, 1.0f );
    else if( index == 5 )
        out_color = vec4( 1.0f, 1.0f, 0.0f, 1.0f );
    else if( index == 6 )
        out_color = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    else if( index == 7 )
        out_color = vec4( 1.0f, 1.0f, 1.0f, 1.0f );

    else if( index == 8 )
        out_color = vec4( 1.0f, 0.7f, 0.0f, 1.0f );
		
out_color = vec4(vec3(in_color),1.0);

//    out_color = vec4( /*ambient_albedo + diffuse + specular*/in_color, 1.0f );

}
