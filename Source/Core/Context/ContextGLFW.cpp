#include "CorePCH.h"
#include "ContextGLFW.h"
#include <GLFW/glfw3.h>

/**	Handle Key Event
*******************************************************************************/
void ContextGLFW::HandleKeyEvent(GLFWwindow* window, int inKey, int inScancode, int inAction, int inMods)
{
	Key key = (Key)inScancode;
	KeyAction action = (KeyAction)inAction;

	ContextGLFW* context = (ContextGLFW*)glfwGetWindowUserPointer(window);
	context->inputState.HandleAction(key, action);
	context->OnKeyAction(key, action);
}

/**	Handle Move
*******************************************************************************/
void ContextGLFW::HandleMove(GLFWwindow* window, int x, int y)
{
	ContextGLFW* context = (ContextGLFW*)glfwGetWindowUserPointer(window);
	context->x = x;
	context->x = y;
	context->OnMoved(x, y);
}

/**	Handle Resize
*******************************************************************************/
void ContextGLFW::HandleResize(GLFWwindow* window, int width, int height)
{
	ContextGLFW* context = (ContextGLFW*)glfwGetWindowUserPointer(window);
	context->width = width;
	context->height = height;
	context->OnResized(width, height);
}

/**	Constructor
*******************************************************************************/
ContextGLFW::ContextGLFW()
{
	Initialize();
}

/**	Run
*******************************************************************************/
void ContextGLFW::Run()
{
	if (!Ensure(isValid))
		return;

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		OnFrame();

		glfwSwapBuffers(window);
		inputState.EndFrame();
	}

	CleanUp();
}

/**	Set Size
*******************************************************************************/
void ContextGLFW::SetSize(int width, int height)
{
	glfwSetWindowSize(window, width, height);
}

/**	Set Position
*******************************************************************************/
void ContextGLFW::SetPosition(int x, int y)
{
	glfwSetWindowPos(window, x, y);
}

/**	Set Always On Top
*******************************************************************************/
void ContextGLFW::SetAlwaysOnTop(bool alwaysOnTop)
{
}

/**	Close
*******************************************************************************/
void ContextGLFW::Close()
{
	glfwSetWindowShouldClose(window, 1);
}

/**	Initialize
*******************************************************************************/
void ContextGLFW::Initialize()
{
	if (!Ensure(glfwInit()))
		return;

	// Window hints
	glfwWindowHint(GLFW_RESIZABLE, 0);

	window = glfwCreateWindow(1024, 768, "ECS Engine", NULL, NULL);
	if (!Ensure(window))
	{
		glfwTerminate();
		return;
	}

	glfwSetKeyCallback(window, HandleKeyEvent);
	glfwSetWindowPosCallback(window, &ContextGLFW::HandleMove);
	glfwSetWindowSizeCallback(window, &ContextGLFW::HandleResize);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	glewInit();

	glfwGetWindowPos(window, &x, &y);
	glfwGetFramebufferSize(window, &width, &height);
	glfwSetWindowUserPointer(window, this);

	isValid = true;
}

/**	Clean Up
*******************************************************************************/
void ContextGLFW::CleanUp()
{
	glfwTerminate();
	window = nullptr;
	isValid = false;
}

