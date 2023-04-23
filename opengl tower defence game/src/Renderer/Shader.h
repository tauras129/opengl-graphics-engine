#pragma once
#include <string>
#include <unordered_map>
#include <GLM/glm.hpp>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID = 0;
	// caching for uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	explicit Shader(const std::string& filepath);
	Shader(const std::string& filepathVertex, const std::string& filepathFragment);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1iv(const std::string& name, const int value[], int size);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	ShaderProgramSource ParseShaderFiles(const std::string& filepath);
	ShaderProgramSource ParseShaderFiles(const std::string& filepathVertex, const std::string& filepathFragment);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int GetUniformLocation(const std::string& name);
};
