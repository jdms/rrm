#version 410 core

layout ( location = 0 ) out vec4 FragColor;

uniform vec4 l_ambient = vec4( 0.1, 0.1, 0.1, 1.0 );
uniform vec4 l_diffuse  = vec4( 0.9, 0.9, 0.9, 1.0 );
uniform vec4 l_specular = vec4( 0.9, 0.9, 0.9, 1.0 );

uniform vec4 m_ambient = vec4( 1.0, 1.0, 1.0, 1.0 );
uniform vec4 m_diffuse  = vec4( 0.6, 0.6, 0.6, 1.0 );
uniform vec4 m_specular = vec4( 0.4, 0.4, 0.4, 1.0 );

uniform bool solid;
uniform bool testing;

in vec3 fN;
in vec3 fL;
in vec3 fE;
in vec3 fcolor;

void main(void)
{

    vec3 N = normalize( fN );
    vec3 L = normalize( fL );
    vec3 E = normalize( fE );
    vec3 R = reflect( -L, N );


    float shininess = 5.0f;


    float NdotL = dot( N, L );
    float Kd = max( abs( NdotL ), 0.0f );
    float Ks =  0.6f * pow ( max ( dot ( R , E ), 0.0f ) , shininess );


    vec4 diffuse  = Kd * vec4( fcolor.xyz, 1.0f );
    vec4 specular = Ks * vec4( fcolor.xyz, 1.0f );


    vec4 surface_color =     vec4( diffuse.xyz + specular.xyz, 1.0f );
    vec4 edge_color =  0.5f* vec4( diffuse.xyz + specular.xyz, 1.0f );

    if( solid )
        FragColor = surface_color;
    else
        FragColor = edge_color;

    if( testing == true )
        FragColor.a = 0.2f;
    else
        FragColor.a = 1.0f;

}
