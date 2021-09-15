#include <iostream>
#include <glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"


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

	Shader sampleShader("res/shaders/basic.vert", "res/shaders/basic.frag");

	VertexArray va;
	VertexBuffer vbo(vertices, sizeof(vertices));
	VertexBufferLayout layout;
	layout.push<float>(3);
	va.addBuffer(vbo, layout);

	IndexBuffer ibo(indices, 6);

	sampleShader.useProgram();
	va.bind();
	ibo.bind();

	glViewport(0, 0, width, height);
	float time = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.5f, 1.0f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}