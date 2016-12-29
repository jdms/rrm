#version 410 core

layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;

uniform vec2 WIN_SCALE;
uniform vec4 ClipPlane;
uniform vec3 ClipPlaneCentre;
uniform bool isClip;

uniform mat4 ViewMatrix;

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


bool userClipImplicit(vec4 plane , vec3 p )
{
    float fx = plane.x*p.x + plane.y*p.y + plane.z*p.z + plane.w;

    if ( fx >= 0 )
    {
        return true;
    }
    else
    {
        return false;
    }

    return false;

}

bool userClip ( vec3 n , vec3 c , vec3 p )
{

    vec3 pc = p - c;

    float d = dot ( pc , n );

    if ( d >= 0 )
    {
        return true;
    }
    else
    {
        return false;
    }

    return false;
}


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

    bool clipped = false;

    if ( isClip )
    {
        if ( userClipImplicit(ClipPlane,VertexIn[0].vertice.xyz) ||
             userClipImplicit(ClipPlane,VertexIn[1].vertice.xyz) ||
             userClipImplicit(ClipPlane,VertexIn[2].vertice.xyz))
        {
            clipped = true;
        }else
        {
            clipped = false;
        }

    }else
    {
        clipped = false;
    }


    /// First Vertex
    dist = vec3(area/length(v0),0,0);
    VertexOut.vertice    = ViewMatrix * VertexIn[0].vertice;

    if ( clipped )
    {
        gl_Position = vec4(0.0,0.0,0.0,1.0);
    }else
    {
        gl_Position = gl_in[0].gl_Position;
    }


    EmitVertex();

    /// Second Vertex
    VertexOut.color    = VertexIn[1].color;

    dist = vec3(0,area/length(v1),0);
    VertexOut.vertice    = ViewMatrix * VertexIn[1].vertice;

    if ( clipped )
    {
        gl_Position = vec4(0.0,0.0,0.0,1.0);
    }else
    {
        gl_Position = gl_in[1].gl_Position;
    }


    EmitVertex();

    /// Third Vertex
    VertexOut.color    = VertexIn[2].color;

    dist = vec3(0,0,area/length(v2));
    VertexOut.vertice    = ViewMatrix * VertexIn[2].vertice;

    if ( clipped )
    {
        gl_Position = vec4(0.0,0.0,0.0,1.0);
    }else
    {
        gl_Position = gl_in[2].gl_Position;
    }


    EmitVertex();


    EndPrimitive();
}
