#!vertex
#version 460

uniform mat4 W;

layout (location = 0) in vec3 pos;
layout (location = 0) in vec3 normal;
layout (location = 0) in vec3 tex;

void main()
{
    gl_Position = W * vec4(pos, 1.0);
}

#!fragment
#version 460

out vec4 frag;

void main()
{
    frag = vec4(0.2f, 0.2f, 0.2f, 1.0f);
} 
