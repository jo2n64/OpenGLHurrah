#include <iostream>
#include <glm.hpp>
#include <GL/glew.h>
#include <fstream>
#include <string>
#include <sstream>
#include <GLFW/glfw3.h>

static std::string parseShaderSource(const std::string& shaderSource) {
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

static void glClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static void glCheckError() {
	while (GLenum error = glGetError()) {
		std::cout << "opengl error code" << error << std::endl;
	}
}

static unsigned int loadShader(GLenum type, const GLchar* shaderSrc) {
	unsigned int shader;
	int compiled;
	shader = glCreateShader(type);
	if (shader == 0) return 0;
	glShaderSource(shader, 1, &shaderSrc, NULL);
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

static unsigned int bindShader(unsigned int& vertShader, unsigned int& fragShader) {
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
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

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		glDeleteProgram(shaderProgram);
		return 0;
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	return shaderProgram;
}

static int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int vertexShaderId = loadShader(GL_VERTEX_SHADER, vertexShader.c_str());
	unsigned int fragmentShaderId = loadShader(GL_FRAGMENT_SHADER, fragmentShader.c_str());
	unsigned int program = bindShader(vertexShaderId, fragmentShaderId);
	return program;

}

int main(void)
{
	std::cout << "Hey I made this tiny log lol" << std::endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Test", NULL, NULL);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glewInit();

	float vertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};


	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};

	std::string vertShaderSource = parseShaderSource("res/shaders/basic.vert");
	std::string fragShaderSource = parseShaderSource("res/shaders/basic.frag");
	unsigned int shaderProgram = createShader(vertShaderSource, fragShaderSource);

	//TODO abstracting the buffers into classes doesnt work for now...
	unsigned int vao, vbo, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	glViewport(0, 0, width, height);
	float time = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.5f, 1.0f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}