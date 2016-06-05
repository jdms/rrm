#version 430 compatibility

layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;

uniform vec2 WIN_SCALE;

noperspective out vec3 dist;

in VertexData
{
	vec4 vertice;
	vec3 normal;
	vec4 color;
} VertexIn[];

out VertexData
{
	vec4 vertice;
	vec3 normal;
	vec4 color;
} VertexOut;



void main(void)
{
	vec2 p0 = WIN_SCALE * (gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w);
	vec2 p1 = WIN_SCALE * (gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w);
	vec2 p2 = WIN_SCALE * (gl_in[2].gl_Position.xy / gl_in[2].gl_Position.w);


	float a = length ( p1 - p2 );
	float b = length ( p2 - p0 );
	float c = length ( p1 - p0 );

	float alpha = acos ( ( b * b + c * c - a * a ) / ( 2.0 * b * c ) );
	float beta = acos ( ( a * a + c * c - b * b ) / ( 2.0 * a * c ) );

	float ha = abs ( c * sin ( beta ) );
	float hb = abs ( c * sin ( alpha ) );
	float hc = abs ( b * sin ( alpha ) );


	VertexOut.normal   = VertexIn[0].normal;
	VertexOut.color    = VertexIn[0].color;

	dist = vec3(ha, 0.0, 0.0);
	VertexOut.vertice    = VertexIn[0].vertice;
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	dist = vec3(0.0, hb, 0.0);
	VertexOut.vertice    = VertexIn[1].vertice;
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	dist = vec3(0.0, 0.0, hc);
	VertexOut.vertice    = VertexIn[2].vertice;
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();


	EndPrimitive();
}
