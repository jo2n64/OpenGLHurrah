#pragma once
#include <iostream>
#include <GL/glew.h>

class Shader
{
private:
	std::string m_vertexShaderSource, m_fragmentShaderSource;
	std::string parseShaderSource(const std::string shaderSource);
	unsigned int m_shaderProgramID;
	unsigned int compileShader(GLenum type, const GLchar* shaderSource);
	unsigned int bindShader(unsigned int& vertexShader, unsigned int& fragmentShader);
	void createShader(const std::string vertexShaderSource, const std::string fragmentShaderSource);

public:
	Shader(const std::string vertexShaderSource, const std::string fragmentShaderSource);
	~Shader();
	void useProgram();
	unsigned int getShaderProgramID() const { return m_shaderProgramID; }

	//TODO setting uniforms etc
	void SetUniform4f(const unsigned int location, float v0, float v1, float f2, float f3);
};

