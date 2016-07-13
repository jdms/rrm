#version 430 compatibility

layout(location = 0) in vec4 vertices;
layout(location = 1) in vec4 normals;
layout(location = 2) in vec4 colors;

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
uniform vec4 color_plane;
uniform float z;

void main(void)
{

	mat3 normalMatrix = mat3(ViewMatrix);
	normalMatrix = inverse(normalMatrix);
	normalMatrix = transpose(normalMatrix);


	VertexOut.vertice  = ViewMatrix * vec4(vertices.xyz,1.0);
	VertexOut.normal   = normalMatrix * normals.xyz;
	VertexOut.color    = color_plane;

	gl_Position = ProjectionMatrix * ViewMatrix *vec4((vertices.xy),z,1.0);

}

