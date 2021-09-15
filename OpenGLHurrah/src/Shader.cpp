#include "Shader.h"
#include <fstream>
#include <sstream>

std::string Shader::parseShaderSource(const std::string shaderSource)
{
	std::ifstream stream(shaderSource);
	//TODO path error checking
	std::string line;
	std::stringstream strStream;
	while (getline(stream, line)) {
		strStream << line << '\n';
	}
	std::cout << "Shader loaded:\n" << strStream.str() << std::endl;
	return strStream.str();
}

unsigned int Shader::compileShader(GLenum type, const GLchar* shaderSource)
{
	unsigned int shader;
	int compiled;
	shader = glCreateShader(type);
	if (shader == 0) return 0;
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* msg = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(shader, length, &length, msg);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader." << std::endl;
		std::cout << msg << std::endl;
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

unsigned int Shader::bindShader(unsigned int& vertexShader, unsigned int& fragmentShader)
{
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	int result;
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
		char* msg = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(shaderProgram, length, &length, msg);
		std::cout << "Failed to link program" << std::endl;
		std::cout << msg << std::endl;

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(shaderProgram);
		return 0;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

Shader::Shader(const std::string vertexShaderSource, const std::string fragmentShaderSource)
{
	createShader(vertexShaderSource, fragmentShaderSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_shaderProgramID);
}

void Shader::useProgram()
{
	glUseProgram(m_shaderProgramID);
}

void Shader::SetUniform4f(const unsigned int location, float v0, float v1, float f2, float f3)
{
	glUniform4f(location, v0, v1, f2, f3);
}

void Shader::createShader(const std::string vertexShaderSource, const std::string fragmentShaderSource)
{
	const std::string vertexShaderFileContent = parseShaderSource(vertexShaderSource);
	const std::string fragmentShaderFileContent = parseShaderSource(fragmentShaderSource);
	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderFileContent.c_str());
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderFileContent.c_str());
	unsigned int shaderProgram = bindShader(vertexShader, fragmentShader);
	m_shaderProgramID = shaderProgram;
}
