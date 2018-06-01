#include "CorePCH.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include "Core/Context/Context.h"
#include "Core/Program/Program.h"
#include "Core/Tools/File.h"

#ifdef TEST
#include "Core/Test/Test.h"
#endif

void HandleKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	printf("KEYEVENT: %d, %d, %d, %d\n", key, scancode, action, mods);

	if (key == GLFW_KEY_ESCAPE)
		glfwDestroyWindow(window);
}

void HandleWindowResizeEvent(GLFWwindow* window, int width, int height)
{
	Context* context = (Context*)glfwGetWindowUserPointer(window);
	context->width = width;
	context->height = height;
	context->OnResized.Broadcast(width, height);
}

void HandleWindowMoveEvent(GLFWwindow* window, int x, int y)
{
	Context* context = (Context*)glfwGetWindowUserPointer(window);
	context->x = x;
	context->y = y;
	context->OnWindowMoved.Broadcast(x, y);
}

void DoFrame(Program* program);

#ifdef TEST
// If running in test config, just run tests and quit
int main(int argv, char** argc)
{
	Test::Run();
	return 0;
}
#else
int main(int argv, char** argc)
{
	bool alwaysOnTop = false;

	for(int i=0; i<argv; ++i)
	{
		if (strcmp(argc[i], "top") == 0)
			alwaysOnTop = true;
	}

	//--------------------------------------------------- Create and init window
	GLFWwindow* window;

	if (!Ensure(glfwInit()))
		return -1;

	// Window hints
	glfwWindowHint(GLFW_RESIZABLE, 0);

	if (alwaysOnTop)
		glfwWindowHint(GLFW_FLOATING, 1);

	window = glfwCreateWindow(1024, 768, "ECS Engine", NULL, NULL);
	if (!Ensure(window))
	{
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, HandleKeyEvent);
	glfwSetWindowPosCallback(window, HandleWindowMoveEvent);
	glfwSetWindowSizeCallback(window, HandleWindowResizeEvent);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	glewInit();

	Context* context = Context::GetInstance();
	context->width = 1024;
	context->height = 768;
	glfwGetWindowPos(window, &context->width, &context->height);
	glfwGetFramebufferSize(window, &context->width, &context->height);
	glfwSetWindowUserPointer(window, context);

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
#endif

void DoFrame(Program* program)
{
	program->DoFrame();
}
