#version 430 compatibility

in VertexData
{
	vec4 vertice;
	vec3 normal;
	vec4 color;
}VertexIn;

noperspective in vec4 dist;

out vec4 outputColor;

void main ( void )
{

	vec3 lights[4];

	lights[0] = vec3 ( 0.5 , 0.5 , 1.0 );
	lights[1] = vec3 ( -0.5 , 0.5 , 1.0 );
	lights[2] = vec3 ( 0.0 , 0.5 , 1.0 );
	lights[3] = vec3 ( 0.0 , 1.0 , 0.0 );

	float num_lights = 4.0f;

	vec3 newNormal = VertexIn.normal;
	vec3 newVert = VertexIn.vertice.xyz;

	newNormal = normalize ( newNormal );

	vec3 toLight = normalize ( -newVert.xyz );

	vec3 light_dir = vec3 ( 0.0 , 0.0 , 1.0 );
	vec3 eye_dir = normalize ( -newVert.xyz );

	vec4 color_t = VertexIn.color;//vec4 ( 0.10 , 0.75 , 0.10 , 1.0 );

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

	float d = min ( dist[0] , min ( dist[1] , min ( dist[2] , dist[3] ) ) );
	float I = exp2 ( -0.1 * d * d );

	// I = 0.0;

	outputColor = I * vec4 ( 0.0 , 0.0 , 0.0 , 1.0 ) + ( 1.0 - I ) * vec4 ( la.rgb + ld.xyz + ls.rgb , VertexIn.color.a );
	//outputColor = vec4(0.0, 0.0, 0.0, 1.0);

}
