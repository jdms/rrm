#version 410 core

in VertexData
{
	vec4 vertice;
	vec3 normal;
	vec4 color;
} VertexIn;


out vec4 outputColor;

void main ( void )
{

	outputColor = vec4(1.0,0.0,0.0,1.0);

}
