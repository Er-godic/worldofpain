#include "shader.hpp"
#include "glad/glad.h"

// https://learnopengl.com/Getting-started/Shaders
Shader::Shader(const char* path)
{
	// parse
	
	std::ifstream infile(path);
	if (!infile)
	{
		const char* msg = "Unable to open shader file at path '{0}'";
		Log::print(Log::render, Log::critical, msg, path);
	}
			
	std::string line;
	std::stringstream ss[2];
	int index = -1;
	while (getline(infile, line))
	{
		if (line.find("#!vertex") != std::string::npos)        index = 0;
		else if (line.find("#!fragment") != std::string::npos) index = 1;
		else if (index == -1)                                  continue;
		else                                                   ss[index] << line << '\n';
	}

	std::string source[] = {ss[0].str(),  ss[1].str()};
	
	int success;
	char info_log[512];

	// compile

	uint shaders[] = {glCreateShader(GL_VERTEX_SHADER), glCreateShader(GL_FRAGMENT_SHADER)};
	
	for(uint i = 0; i < 2; i++)
	{
		const char* src = source[i].c_str();
		glShaderSource(shaders[i], 1, &src, NULL);
		glCompileShader(shaders[i]);
		glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &success);
		if (!success)
		{
			const char* msg = "{0} shader compilation failed \n {1} \n {2} ";
			const char* shdr = (i == 0) ? "Vertex" : "Fragment";
			glGetShaderInfoLog(shaders[i], 512, NULL, info_log);
			Log::print(Log::render, Log::critical, msg, shdr, info_log, source[i]);
		}
	}

	// create shader program and link

	m_shader_program = glCreateProgram();
	if (!glIsProgram(m_shader_program))
		Log::print(Log::render, Log::critical, "Failed to create shader program");

	for(uint i = 0; i < 2; i++) glAttachShader(m_shader_program, shaders[i]);

	glLinkProgram(m_shader_program);
	glGetProgramiv(m_shader_program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(m_shader_program, 512, NULL, info_log);
		const char* msg = "Shader program linking failed \n {0}";
		Log::print(Log::render, Log::critical, msg, info_log);
	}

	else 
	{
		const char* msg = "Shader program linking success at '{0}', shader program id : {1}";
		std::string shdr = std::to_string(m_shader_program);
		Log::print(Log::render, Log::info, msg, path, shdr);
	}	

	// don't need these after linking
	for(uint i = 0; i < 2; i++) glDeleteShader(shaders[i]);
}

Shader::~Shader() { glDeleteProgram(m_shader_program); }
uint Shader::id() const { return m_shader_program; }
void Shader::use() const { glUseProgram(m_shader_program); }

int Shader::getloc (const std::string& name)
{
	// check for the location first in the uniformloc map
	auto it = m_uniformloc.find(name);
	if (it != m_uniformloc.end())
		return it->second;

	int loc = glGetUniformLocation(m_shader_program, name.c_str());
	
	// this is if the uniform doesn't exist
	if (loc == -1)
	{
		const char* msg = "The uniform '{0}' is not valid, shader program id : {1}";
		std::string shdr = std::to_string(m_shader_program);
		Log::print(Log::render, Log::error, name, shdr);
	}

	// store location if it is not already stored
	else m_uniformloc[name] = loc;

	return loc;
}

void Shader::setUniform(const std::string& name, glm::mat4& mat4) { glUniformMatrix4fv(getloc(name), 1, GL_FALSE, &mat4[0][0]); }
