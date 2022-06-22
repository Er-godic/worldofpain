#!vertex
#version 460

layout (location = 0) in vec3 pos;

void main()
{
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}

#!fragment
#version 460

out vec4 frag;

void main()
{
    frag = vec4(0.2f, 0.2f, 0.2f, 1.0f);
} 
