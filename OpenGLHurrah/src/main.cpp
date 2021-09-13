
#include <iostream>
#include <glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


std::string vertShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos; \n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1,0);\n"
" }\0";

std::string fragShaderSource = "#version 330 core\n"
"out vec4 fragColor;\n"
"void main()\n"
"{\n"
"	fragColor = vec4(0.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";


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
		char* msg = (char*)_malloca(length* sizeof(char));
		glGetShaderInfoLog(shader, length, &length, msg);
		std::cout << "Failed to compile "<<(type == GL_VERTEX_SHADER? "vertex" : "fragment")<< " shader."<< std::endl;
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
	//TODO link status check
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
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Test", NULL, NULL);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwMakeContextCurrent(window);

	glewInit();

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	float colors[] = {
		1.0f, 0.0f, 0.0f
	};

	unsigned int shaderProgram = createShader(vertShaderSource, fragShaderSource);

	unsigned int vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glViewport(0, 0, width, height);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
}