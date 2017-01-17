#version 410 core

in VertexData
{
    vec4 vertice;
    vec3 normal;
    vec4 color;
} VertexIn;

out vec4 outputColor;

noperspective in vec3 dist;

void main ( void )
{

    float d = min(dist[0], min(dist[1], dist[2]));
    float I = exp2(-1.0 * d * d);


    vec3 lights[1];

    lights[0] = vec3 ( 1.0 ,0.0 , 0.0 );
//  lights[1] = vec3 ( 1.0 , 2.0 , -1.0 );
//  lights[2] = vec3 ( -1.0 , 2.0 , -1.0 );
//  lights[3] = vec3 ( -1.0 , 2.0 , 1.0 );

    float num_lights = 1.0f;

    vec3 newNormal = VertexIn.normal;
    vec3 newVert = VertexIn.vertice.xyz;

    newNormal = normalize ( newNormal );

    vec3 toLight = normalize ( -newVert.xyz );

    vec3 light_dir = vec3 ( 0.0 , 0.0 , 1.0 );
    vec3 eye_dir = normalize ( -newVert.xyz );

    vec4 color_t = VertexIn.color;//vec4(0.7,0.5,0.0,1.0);//VertexIn.color;

    vec4 la = vec4 ( 0.0 );
    vec4 ld = vec4 ( 0.0 );
    vec4 ls = vec4 ( 0.0 );

    for ( int i = 0; i < num_lights; ++i )
    {
        vec3 light_dir = normalize ( lights[i] );
        vec3 ref = normalize ( -reflect ( light_dir , newNormal ) );
        la += vec4 ( 0.5 / float ( num_lights ) );
        ld += color_t * ( 1.0 / float ( num_lights ) ) * max ( 0.0 , abs ( dot ( newNormal , light_dir ) ) );
        ls += color_t * 0.6 * pow ( max ( 0.0 , dot ( eye_dir , ref ) ) , 5.0 );
    }

    outputColor = vec4 (color_t );
}

