#pragma once
#include "Core/Tools/SingletonClass.h"
#include "Core/Delegate/Delegate.h"
#include "Core/Input/InputState.h"

class Context
{
public:
	static Context* Create();
	static Context* Get();

private:
	static Context* CurrentContext;

public:
	void MakeCurrent();

	Delegate<> OnFrame;

	Delegate<int, int> OnMoved;
	Delegate<int, int> OnResized;
	Delegate<Key, KeyAction> OnKeyAction;

	const InputState& GetInputState() const { return inputState; }
	virtual void Run() = 0;

	const int GetWidth() const { return width; }
	const int GetHeight() const { return height; }
	const int GetX() const { return x; }
	const int GetY() const { return y; }

	virtual void SetSize(int width, int height) = 0;
	virtual void SetPosition(int x, int y) = 0;
	virtual void SetAlwaysOnTop(bool alwaysOnTop) = 0;
	virtual void Close() = 0;

protected:
	int x;
	int y;
	int width;
	int height;

	InputState inputState;
};
