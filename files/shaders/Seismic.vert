#version 410 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 normal;
layout( location = 2 ) in vec3 color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform float scale = 1.0f;


out VertexData
{
	vec4 vertice;
	vec3 normal;
	vec4 color;
} VertexOut;


void main ( void )
{
	mat3 normalMatrix = mat3 ( ViewMatrix );
	normalMatrix = inverse ( normalMatrix );
	normalMatrix = transpose ( normalMatrix );

        VertexOut.vertice = ViewMatrix * vec4( scale*position,1.0f);
	VertexOut.normal = normalMatrix * normal;
	VertexOut.color = vec4 ( color , 1.0 );

        gl_Position = ProjectionMatrix * ViewMatrix * vec4 ( scale*position , 1.0f );
}



