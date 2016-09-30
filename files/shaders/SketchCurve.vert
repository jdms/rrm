#version 410 core

layout(location = 0) in vec4 vertices;

out VertexData
{
	vec4 vertice;
	vec3 normal;
	vec4 color;
} VertexOut;

// For while, all transformations come from the Celer::Camera.
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void)
{

	VertexOut.vertice  = ViewMatrix * vertices;
	VertexOut.normal   = vertices.xyz;
	VertexOut.color    = vertices;

	gl_Position = ProjectionMatrix * ViewMatrix * vertices;
}


