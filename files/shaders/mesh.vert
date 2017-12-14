#version 410 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;
layout ( location = 2 ) in vec4 color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform vec4 lightPosition = vec4( 0.0, 2.0, 0.0, 0.0 );

out vec3 fN;
out vec3 fL;
out vec3 fE;
out vec3 fcolor;

void main(void)
{

    mat3 NormalMatrix = transpose( inverse( mat3 ( ViewMatrix ) ) );
    vec3 n = normalize( normal );

    vec4 eyePosition = ViewMatrix * ModelMatrix * vec4( position, 1.0 );
    fN = NormalMatrix * n;
    fL = lightPosition.xyz - eyePosition.xyz;
    fE = -eyePosition.xyz;

    fcolor = color.xyz;
    gl_Position = ProjectionMatrix * eyePosition;
}
