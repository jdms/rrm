#version 410 core

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


    vec2 v0 = p2-p1;
    vec2 v1 = p2-p0;
    vec2 v2 = p1-p0;

    float area = abs(v1.x*v2.y - v1.y * v2.x);

    VertexOut.normal   = VertexIn[0].normal;
    VertexOut.color    = VertexIn[0].color;

    dist = vec3(area/length(v0),0,0);
    VertexOut.vertice    = VertexIn[0].vertice;
    gl_Position = gl_in[0].gl_Position;

    gl_ClipDistance[0] = gl_in[0].gl_ClipDistance[0];
    EmitVertex();

    VertexOut.color    = VertexIn[1].color;

    dist = vec3(0,area/length(v1),0);
    VertexOut.vertice    = VertexIn[1].vertice;
    gl_Position = gl_in[1].gl_Position;

    gl_ClipDistance[0] = gl_in[1].gl_ClipDistance[0];
    EmitVertex();

    VertexOut.color    = VertexIn[2].color;

    dist = vec3(0,0,area/length(v2));
    VertexOut.vertice    = VertexIn[2].vertice;
    gl_Position = gl_in[2].gl_Position;

    gl_ClipDistance[0] = gl_in[2].gl_ClipDistance[0];
    EmitVertex();


    EndPrimitive();
}
