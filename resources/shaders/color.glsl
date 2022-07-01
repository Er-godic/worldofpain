#!vertex
#version 460

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
out vec3 Color;

uniform mat4 W;

void main()
{
	Color = color;
	gl_Position = W * vec4(pos, 1.0);
}

#!fragment
#version 460

in vec3 Color; 
out vec4 frag;

void main()
{
	frag = vec4(Color, 1.0);
} 
