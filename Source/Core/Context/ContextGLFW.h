#include "Context.h"

struct GLFWwindow;

class ContextGLFW : public Context
{
private:
	static void HandleKeyEvent(GLFWwindow* window, int inKey, int inScancode, int inAction, int inMods);
	static void HandleMove(GLFWwindow* window, int x, int y);
	static void HandleResize(GLFWwindow* window, int width, int height);

public:
	ContextGLFW();
	void Run() override;

	void SetSize(int width, int height) override;
	void SetPosition(int x, int y) override;
	void SetAlwaysOnTop(bool alwaysOnTop) override;
	void Close() override;

private:
	void Initialize();
	void CleanUp();

	GLFWwindow* window = nullptr;
	bool isValid = false;
};