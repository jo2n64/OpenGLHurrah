#pragma once
#include <iostream>
#include<GL/glew.h>

class ShaderProgram
{
public:
	ShaderProgram(std::string vertShaderSource, std::string fragShaderSource);
	~ShaderProgram();
	unsigned int getShaderProgram();

private:
	std::string m_vertexShaderSource;
	std::string m_fragmentShaderSource;
	unsigned int m_shaderProgram;

	void createVertexShader(std::string source);
	void createFragmentShader(std::string source);

};

