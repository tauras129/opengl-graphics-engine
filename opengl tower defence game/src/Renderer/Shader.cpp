#include "Shader.h"

#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath)
{
	ShaderProgramSource source = ParseShaderFiles(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::Shader(const std::string& filepathVertex, const std::string& filepathFragment)
{
	ShaderProgramSource source = ParseShaderFiles(filepathVertex, filepathFragment);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

ShaderProgramSource Shader::ParseShaderFiles(const std::string& filepath)
{
	std::ifstream vertStream(filepath + ".vert");
	std::ifstream fragStream(filepath + ".frag");

	if (vertStream.fail())
	{
		//failed to load vertex shader file
		std::cout << "failed to load vertex shader from filepath " << filepath << ".vert" << std::endl;
		__debugbreak();
	}
	if (fragStream.fail())
	{
		//failed to load fragment shader file
		std::cout << "failed to load fragment shader from filepath " << filepath << ".frag" << std::endl;
		__debugbreak();
	}

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];

	//vertex
	ShaderType type = ShaderType::VERTEX;
	while (getline(vertStream, line))
	{
		ss[(int)type] << line << "\n";
	}
	//fragment
	type = ShaderType::FRAGMENT;
	while (getline(fragStream, line))
	{
		ss[(int)type] << line << "\n";
	}

	return {
		ss[0].str(), ss[1].str()
	};
}

ShaderProgramSource Shader::ParseShaderFiles(const std::string& filepathVertex, const std::string& filepathFragment)
{
	std::ifstream vertStream(filepathVertex);
	std::ifstream fragStream(filepathFragment);

	if (vertStream.fail())
	{
		//failed to load vertex shader file
		std::cout << "failed to load vertex shader from filepath " << filepathVertex << std::endl;
		__debugbreak();
	}
	if (fragStream.fail())
	{
		//failed to load fragment shader file
		std::cout << "failed to load fragment shader from filepath " << filepathFragment << std::endl;
		__debugbreak();
	}

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];

	//vertex
	ShaderType type = ShaderType::VERTEX;
	while (getline(vertStream, line))
	{
		ss[(int)type] << line << "\n";
	}
	//fragment
	type = ShaderType::FRAGMENT;
	while (getline(fragStream, line))
	{
		ss[(int)type] << line << "\n";
	}

	return {
		ss[0].str(), ss[1].str()
	};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//error handling in case of compilation error
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)malloc(sizeof(length));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	int result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glValidateProgram(program);
	if (result == GL_FALSE)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)malloc(sizeof(512));
		glGetProgramInfoLog(program, 512, nullptr, message);
		std::cout << "Failed to link shaders!" << std::endl;
		std::cout << message << std::endl;
		glDeleteProgram(program);
		return 0;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1iv(const std::string& name, const int value[], int size)
{
	glUniform1iv(GetUniformLocation(name), size, value);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
		std::cout << "WARNING: shader uniform '" << name << "' doesn't exist, this may happen if it's not used" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}
