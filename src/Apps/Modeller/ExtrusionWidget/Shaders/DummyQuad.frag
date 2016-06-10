#version 430 compatibility


out vec4 out_Color;

in vec2 texcoord;
uniform ivec2 viewportSize;


void main(void)
{
        float y = gl_FragCoord.y/viewportSize.y;
        // Video
        out_Color = vec4(vec3(0.605,0.691,0.781) + (1-y)* vec3(0.1,0.1,0.1),1.0);
        //out_Color = vec4(vec3(0.5,0.5,0.5) + (y)* vec3(0.2,0.2,0.2),1.0);
        // Paper Figures
        //out_Color = vec4(1.0,1.0,1.0,1.0);
}

