#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <malloc.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Shader.h"

extern "C"
{
	void glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "[%i] Error: %s\n", error, description);
	}

	void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

std::pair<std::string, std::string> load_shader(const std::string& path1, const std::string& path2)
{
	std::ifstream stream(path1);

	std::string line;
	std::stringstream ss[2];

	while (getline(stream, line))
	{
		ss[0] << line << "\n";
	}

	stream.close();

	stream.open(path2);

	while (getline(stream, line))
	{
		ss[1] << line << "\n";
	}

	stream.close();

	return { ss[0].str(), ss[1].str() };
}

int main()
{
	unsigned int vertex_array, vertex_buffer, program, basic_shader_v, basic_shader_f;
	unsigned int a_position = 0, a_colour = 1;

	if (!glfwInit())
		__debugbreak();

	GLFWwindow* window = glfwCreateWindow(640, 480, "gl_sandbox", NULL, NULL);

	if (!window)
		__debugbreak();

	glfwMakeContextCurrent(window);
	gladLoadGL();

	// v-sync on
	glfwSwapInterval(1);
	
	// setup callbacks
	glfwSetErrorCallback(glfw_error_callback);
	glfwSetKeyCallback(window, glfw_key_callback);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	float vertices[] =
	{
		-0.5f, -0.5f, 0.f, 1.f, 0.f, 0.f, 1.f,
		 0.5f, -0.5f, 0.f, 0.f, 1.f, 0.f, 1.f,
		 0.f,   0.5f, 0.f, 0.f, 0.f, 1.f, 1.f
	};

	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(a_position);
	glVertexAttribPointer(a_position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)0);

	glEnableVertexAttribArray(a_colour);
	glVertexAttribPointer(a_colour, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*) (sizeof(float) * 3));

	Shader basic_v("res/shaders/basic/basic_vertex.shader", ShaderType::Vertex);
	Shader basic_f("res/shaders/basic/basic_fragment.shader", ShaderType::Fragment);

	ShaderProgram basic_shader(basic_v, basic_f);

	basic_shader.bind();

	while (!glfwWindowShouldClose(window))
	{
		double time = glfwGetTime();
		//printf("Time: %f\n", time);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	
	glfwTerminate();
	return 0;
}