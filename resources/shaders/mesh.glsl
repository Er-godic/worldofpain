#!vertex
#version 460

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;

out vec2 Tex;

uniform mat4 W;

void main()
{
	Tex = tex;
	gl_Position = W * vec4(pos, 1.0);
}

#!fragment
#version 460

in vec2 Tex;
out vec4 frag;

struct Material 
{
	sampler2D ambient;
	sampler2D diffuse;
	sampler2D specular;    
	float shininess;
};

uniform Material material; 

void main()
{
	frag = texture(material.ambient, Tex);
} 
