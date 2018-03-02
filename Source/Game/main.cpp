#include "CorePCH.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include "Core/Context/Context.h"
#include "Core/Program/Program.h"

void HandleKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	printf("KEYEVENT: %d, %d, %d, %d\n", key, scancode, action, mods);

	if (key == GLFW_KEY_ESCAPE)
		glfwDestroyWindow(window);
}

void DoFrame(Program* program);

int main(int argv, char** argc)
{
	//--------------------------------------------------- Create and init window
	GLFWwindow* window;

	if (!Ensure(glfwInit()))
		return -1;

	// Window hints
	glfwWindowHint(GLFW_RESIZABLE, 0);

	window = glfwCreateWindow(1024, 768, "Hello World!", NULL, NULL);
	if (!Ensure(window))
	{
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, HandleKeyEvent);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	glewInit();

	Context* context = Context::GetInstance();
	context->width = 1024;
	context->height = 768;
	glfwGetWindowPos(window, &context->width, &context->height);
	glfwGetFramebufferSize(window, &context->width, &context->height);

	//--------------------------------------------------- Main loop
	Program program;

	while(!glfwWindowShouldClose(window))
	{
		DoFrame(&program);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}	

void DoFrame(Program* program)
{
	program->DoFrame();
}
