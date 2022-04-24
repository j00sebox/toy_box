#include "pch.h"
#include "Input.h"

bool Input::is_key_pressed(GLFWwindow* window_handle, int key_code)
{
	auto state = glfwGetKey(window_handle, key_code);

	return (state == GLFW_PRESS) || (state == GLFW_REPEAT);
}

bool Input::is_button_pressed(GLFWwindow* window_handle, int button_code)
{
	auto state = glfwGetKey(window_handle, button_code);

	return (state == GLFW_PRESS) || (state == GLFW_REPEAT);
}
