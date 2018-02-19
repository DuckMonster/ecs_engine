#include "CorePCH.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include "Core/World/World.h"
#include "Core/Context/Context.h"

void HandleKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	printf("KEYEVENT: %d, %d, %d, %d\n", key, scancode, action, mods);

	if (key == GLFW_KEY_ESCAPE)
		glfwDestroyWindow(window);
}

void DoFrame(World* world);

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
	World world;

	while(!glfwWindowShouldClose(window))
	{
		DoFrame(&world);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}	

void DoFrame(World* world)
{
	using namespace std::chrono;
	using hr_clock = high_resolution_clock;
	static hr_clock::time_point LAST_FRAME;
	static bool IS_FIRST_FRAME = true;

	//--------------------------------------------------- Calculate frame time
	if (IS_FIRST_FRAME)
	{
		// We want to avoid huge deltas on the first frame
		LAST_FRAME = hr_clock::now();
		IS_FIRST_FRAME = false;
	}

	hr_clock::time_point frame = hr_clock::now();
	float delta = duration_cast<microseconds>(frame - LAST_FRAME).count() * 1.e-6f;
	LAST_FRAME = frame;
	//----

	world->DoFrame(delta);
}
